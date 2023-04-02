#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "mqtt.h"
#include "unique_id.h"

///////////////////////////////////////////////////////////////////////////////
// CALLBACKS

void mqtt_request_cb(void *arg, err_t err)
{
    switch (err)
    {
    case ERR_OK:
        break;
    case ERR_TIMEOUT:
        LWIP_PLATFORM_DIAG(("MQTT client \"%s\" request: timeout\n", ((const mq *)arg)->client_id));
        break;
    case ERR_ABRT:
        LWIP_PLATFORM_DIAG(("MQTT client \"%s\" request: sub or unsub failed\n", ((const mq *)arg)->client_id));
        break;
    default:
        LWIP_PLATFORM_DIAG(("MQTT client \"%s\" request: error %d\n", ((const mq *)arg)->client_id, err));
    }
}

void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status)
{
    LWIP_PLATFORM_DIAG(("MQTT client %s connection cb: status %d\n", ((const mq *)arg)->client_id, (int)status));
    if (status == MQTT_CONNECT_ACCEPTED && ((const mq *)arg)->topic != NULL)
    {
        int err = mqtt_sub_unsub(client, ((const mq *)arg)->topic, 1, mqtt_request_cb, arg, 1);
        if(err) {
            LWIP_PLATFORM_DIAG(("MQTT client \"%s\" subscribe failed with error %d", ((const mq *)arg)->client_id, err));
        }
    }
}

void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags)
{
    LWIP_PLATFORM_DIAG(("MQTT client \"%s\" incoming: len %d, flags %d\n", ((const mq *)arg)->client_id, (int)len, (int)flags));
}

void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len)
{
    LWIP_PLATFORM_DIAG(("MQTT client \"%s\" publish: topic %s, len %d\n", ((const mq *)arg)->client_id, topic, (int)tot_len));
}

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

// Initalize the client data structure
mq *mq_init(ip_addr_t addr, const char *user, const char *password, const char *topic)
{
    mq *client = malloc(sizeof(mq));
    if (client == NULL)
    {
        return NULL;
    }
    client->client = mqtt_client_new();
    client->client_id = get_unique_board_id();
    client->addr = addr;
    client->port = MQTT_PORT;
    client->user = user;
    client->password = password;
    client->topic = topic;

    // Return success
    return client;
}

// Disconnect and free the client
void mq_free(mq *client)
{
    if (client->client != NULL && mqtt_client_is_connected(client->client))
    {
        cyw43_arch_lwip_begin();
        mqtt_disconnect(client->client);
        cyw43_arch_lwip_end();
    }
    if (client->client != NULL)
    {
        mqtt_client_free(client->client);
    }
    free(client);
}

// Connect to the broker
int mq_connect(mq *client)
{
    if (client == NULL || client->client == NULL)
    {
        return ERR_ARG;
    }

    if (mq_is_connected(client))
    {
        return ERR_ISCONN;
    }

    // Set callbacks
    mqtt_set_inpub_callback(client->client, mqtt_incoming_publish_cb, mqtt_incoming_data_cb, LWIP_CONST_CAST(void *, client));

    // Create a client_id
    char client_id[256];
    snprintf(client_id, sizeof(client_id), "pico-%s", client->client_id);

    // Connection info
    struct mqtt_connect_client_info_t client_info = {
        .client_id = client_id,
        .client_user = client->user,
        .client_pass = client->password,
        .keep_alive = 120,
    };

    // Connect to the broker
    cyw43_arch_lwip_begin();
    int err = mqtt_client_connect(client->client, &client->addr, client->port, mqtt_connection_cb, LWIP_CONST_CAST(void *, client), &client_info);
    cyw43_arch_lwip_end();

    // If error, return
    if (err != ERR_OK)
    {
        return err;
    }
    else
    {
        err = ERR_CONN;
    }

    // Wait for client connect
    int retries = 100;
    while (retries--)
    {
        cyw43_arch_lwip_begin();
        int connected = mqtt_client_is_connected(client->client);
        cyw43_arch_lwip_end();
        if (connected)
        {
            err = ERR_OK;
            break;
        }
        sleep_ms(100);
    }

    // Return ERR_OK or ERR_CONN
    return err;
}

// Disconnect from the broker
void mq_disconnect(mq *client)
{
    if (client != NULL && client->client != NULL)
    {
        cyw43_arch_lwip_begin();
        mqtt_disconnect(client->client);
        cyw43_arch_lwip_end();
    }
}

// Return true if connected to the broker
int mq_is_connected(mq *client)
{
    if (client == NULL || client->client == NULL)
    {
        return 0;
    }
    cyw43_arch_lwip_begin();
    int connected = mqtt_client_is_connected(client->client);
    cyw43_arch_lwip_end();
    return connected;
}

// Publish a message to the broker
int mq_publish(mq *client, const char *topic, const char *message)
{
    if (topic == NULL || message == NULL)
    {
        return ERR_ARG;
    }
    cyw43_arch_lwip_begin();
    int err = mqtt_publish(client->client, topic, message, strlen(message), 0, 0, mqtt_request_cb, LWIP_CONST_CAST(void *, client));
    cyw43_arch_lwip_end();
    return err;
}
