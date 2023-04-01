#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/apps/mqtt.h"

// MQTT initialization
mqtt_client_t *mqtt_init(const char* broker, const char* client_id, const char* user, const char* password, u16_t port) {
    ip_addr_t addr;
    err_t err;
    mqtt_client_t *client = mqtt_client_new();
    struct mqtt_connect_client_info_t mqtt_client_info =
    {
        client_id,
        user,
        password,
        120,  /* keep alive */
        NULL, /* will_topic */
        NULL, /* will_msg */
        0,    /* will_qos */
        0     /* will_retain */
#if LWIP_ALTCP && LWIP_ALTCP_TLS
        ,
        NULL
#endif
    };

    if (!ip4addr_aton(broker, &addr)) {
        return NULL;
    }

    mqtt_set_inpub_callback(mqtt_client,
                            mqtt_incoming_publish_cb,
                            mqtt_incoming_data_cb,
                            LWIP_CONST_CAST(void *, &mqtt_client_info));

    err = mqtt_client_connect(mqtt_client,
                              &mqtt_ip, port,
                              mqtt_connection_cb, LWIP_CONST_CAST(void *, &mqtt_client_info),
                              &mqtt_client_info);
    if (err != ERR_OK)
    {
        return NULL;
    }
    return client;
}
