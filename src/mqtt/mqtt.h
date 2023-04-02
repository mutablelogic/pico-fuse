
#ifndef MQTT_H
#define MQTT_H

#include "lwip/apps/mqtt.h"

typedef struct
{
    mqtt_client_t *client;
    ip_addr_t addr;
    u16_t port;
    const char *client_id;
    const char *user;
    const char *password;
    const char *topic;
} mq;

// Initialize the client data structure with the broker address, credentials and topic to subscribe to
// Returns a pointer to the client data structure or NULL if there was an error
extern mq *mq_init(ip_addr_t addr, const char *user, const char *password, const char* topic);
extern void mq_free(mq *client);
extern int mq_connect(mq *client);
extern void mq_disconnect(mq *client);
extern int mq_is_connected(mq *client);
extern int mq_publish(mq *client, const char *topic, const char *message);

#endif
