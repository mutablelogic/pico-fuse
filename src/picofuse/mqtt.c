#include <picofuse/picofuse.h>

#ifdef PICO_CYW43_SUPPORTED
#include <lwip/apps/mqtt.h>
#include <lwip/dns.h>
#endif

///////////////////////////////////////////////////////////////////////////////
// TYPES

struct mqtt_context
{
    mqtt_client_t *client;
    ip_addr_t remote_addr;
    u16_t port;
    struct mqtt_connect_client_info_t info;
};

void *mqtt_new(fuse_t *fuse, void *userdata);
void mqtt_destroy(fuse_t *fuse, void *context);

///////////////////////////////////////////////////////////////////////////////
// GLOBALS

fuse_device_t mqtt = {
    .name = "mqtt",
    .init = mqtt_new,
    .destroy = mqtt_destroy,
};

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

void *mqtt_new(fuse_t *fuse, void *userdata)
{
    assert(fuse);
    assert(userdata);

#ifndef PICO_CYW43_SUPPORTED
    fuse_debugf("mqtt_new: unsupported on this board\n");
    return NULL;
#else
    // Create the context
    struct mqtt_context *context = fuse_alloc(fuse, sizeof(struct mqtt_context), FUSE_MAGIC_MQTT);
    if (context == NULL)
    {
        return NULL;
    }

    // Initialize the context
    context->client = mqtt_client_new();
    if (context->client == NULL)
    {
        fuse_free(fuse, context);
        return NULL;
    }
    // Return success
    return context;
#endif
}

void mqtt_destroy(fuse_t *fuse, void *context)
{
    assert(fuse);
    assert(context);

    mqtt_client_free(((struct mqtt_context* )context)->client);
    fuse_free(fuse, context);
}

/*
void fuse_mqtt_connect_callback(mqtt_client_t *client, void *arg, mqtt_connection_status_t status) {
    if (status != 0) {
        fuse_debugf("fuse_mqtt_connect_callback: error code %d\n", status);
    } else {
        fuse_debugf("fuse_mqtt_connect_callback: connected.\n");
    }
}

void fuse_mqtt_connect(const char *name, const ip_addr_t *ipaddr, void *callback_arg)
{
    struct wifi_mqtt_context *ctx = (struct wifi_mqtt_context *)callback_arg;
    if (ipaddr != NULL)
    {
        ctx->remote_addr = *ipaddr;
    }
    fuse_debugf("fuse_mqtt_connect: %s => %s\n", name, ip4addr_ntoa(ipaddr));

    // Connect to the MQTT server
    err_t err = mqtt_client_connect(ctx->client, &(ctx->remote_addr), ctx->port,fuse_mqtt_connect_callback, ctx, &(ctx->info));
    if (err != ERR_OK) {
        fuse_debugf("fuse_mqtt_connect: error code %d\n", err);
    }
}

void fuse_mqtt_new(const char *host)
{
    if (ctx == NULL)
    {
        ctx = fuse_mqtt_context_new();
        assert(ctx);
    }

    err_t err = dns_gethostbyname(host, &(ctx->remote_addr), fuse_mqtt_connect, ctx);
    if (err == ERR_OK)
    {
        // No lookup needed, connect directly
        fuse_mqtt_connect(host, &(ctx->remote_addr), ctx);
    }
    else
    {
        fuse_debugf("fuse_mqtt_new: error code %d\n", err);
    }
}

void fuse_mqtt_destroy()
{
    if (ctx != NULL)
    {
        fuse_mqtt_context_destroy(ctx);
        ctx = NULL;
    }
}
*/
