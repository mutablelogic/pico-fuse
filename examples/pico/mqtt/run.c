#include <picofuse/picofuse.h>

int run(fuse_t *fuse)
{
    // Register the wifi module for DE country code and station mode,
    // scanning for WiFi networks
    fuse_wifi_userdata_t wifi_params = {
        .country_code = "DE",
        .mode = FUSE_WIFI_STATION | FUSE_WIFI_SCAN,
        .ssid = "10967"};
    fuse_context_t *wifi = fuse_register(fuse, &fuse_wifi, &wifi_params);
    assert(wifi);

    // Register MQTT module to connect to 192.168.86.12
    fuse_mqtt_userdata_t mqtt_params = {
        .host = "192.168.86.12",
        .port = 1883};
    fuse_context_t *mqtt = fuse_register(fuse, &fuse_mqtt, &mqtt_params);
    assert(mqtt);

    // Run fuse
    return fuse_runloop(fuse);
}
