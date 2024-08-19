/** @file mqtt.h
 *  @brief Function prototypes which control the MQTT module
 */
#ifndef PICOFUSE_MQTT_H
#define PICOFUSE_MQTT_H

/** @brief User data for initializing the MQTT module
 */
typedef struct
{
    const char *host;   ///< (required) hostname of the MQTT broker
    const int16_t port; ///< (optional) port number of the MQTT broker
} fuse_mqtt_userdata_t;

#endif
