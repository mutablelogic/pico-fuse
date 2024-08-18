/** @file wifi.h
 *  @brief Function prototypes which control the Wireless LAN module
 */
#ifndef PICOFUSE_WIFI_H
#define PICOFUSE_WIFI_H

/** @brief Wifi mode
 */
typedef enum
{
    FUSE_WIFI_NONE = 0,        ///< No mode
    FUSE_WIFI_STATION = 1,     ///< Station mode
    FUSE_WIFI_ACCESSPOINT = 2, ///< Access point mode
} fuse_wifi_mode_t;

/** @brief WiFi scan context
 */
typedef struct wifi_scan_context fuse_wifi_scan_context;

/** @brief Initialize the wifi module
 */
void fuse_wifi_new(const fuse_wifi_mode_t mode, const char *code);

/** @brief Release the wifi module resources
 */
void fuse_wifi_destroy();


/** @brief Perform a wifi scan
 */
fuse_wifi_scan_context *fuse_wifi_scan();

#endif
