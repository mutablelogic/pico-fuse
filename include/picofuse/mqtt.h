/** @file mqtt.h
 *  @brief Function prototypes which control the MQTT module
 */
#ifndef PICOFUSE_MQTT_H
#define PICOFUSE_MQTT_H

#define MQTT_SERVER_PORT 1883

/** @brief Initialize the mqtt module
 */
void fuse_mqtt_new(const char* host);

/** @brief Release the mqtt module resources
 */
void fuse_mqtt_destroy();

#endif
