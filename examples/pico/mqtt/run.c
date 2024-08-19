#include <picofuse/picofuse.h>

int run(fuse_t *fuse)
{
    // Register the wifi module for DE country code and station mode
    fuse_wifi_userdata_t params = {
        .country_code = "DE",
        .mode = FUSE_WIFI_STATION | FUSE_WIFI_SCAN};
    fuse_context_t *wifi = fuse_register(fuse, &wifi, &params);
    assert(wifi);

    // Register MQTT module to connect to 192.168.86.12
    fuse_mqtt_userdata_t params = {
        .host = "192.168.86.12"};
    fuse_context_t *mqtt = fuse_register(fuse, &mqtt, &params);
    assert(mqtt);

    // Return success
    return 0;
}
