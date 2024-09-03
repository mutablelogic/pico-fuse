/** @file wifi.h
 *  @brief Function prototypes which control the Wireless LAN module
 */
#ifndef PICOFUSE_WIFI_H
#define PICOFUSE_WIFI_H

/** @brief WiFi flags
 */
typedef enum
{
    FUSE_WIFI_NONE = 0,        ///< No mode
    FUSE_WIFI_STATION = 1,     ///< Station mode
    FUSE_WIFI_ACCESSPOINT = 2, ///< Access point mode
    FUSE_WIFI_SCAN = 4,        ///< Scan for wifi networks
} fuse_wifi_mode_t;

/** @brief User data for initializing the WiFi module
 */
typedef struct
{
    const fuse_wifi_mode_t mode; ///< (required) flags for initialization
    const char *country_code;    ///< (optional) two-letter country code for the WiFi region
} fuse_wifi_userdata_t;

#endif
