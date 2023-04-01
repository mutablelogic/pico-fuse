
#ifndef MQTT_H
#define MQTT_H
#include "lwip/apps/mqtt.h"

typedef struct {
    mqtt_client_t* client;
} mqtt_client;

extern mqtt_client* mqtt_init();

#endif
