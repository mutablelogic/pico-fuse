
#include <picofuse/picofuse.h>
#include "opts.h"

// Set countrycode
picofuse_state_t main_init(picofuse_t *self, picofuse_event_t event, void *data)
{
#ifdef WIFI_COUNTRYCODE
    ((picofuse_init_t *)data)->countryCode = WIFI_COUNTRYCODE;
#endif
    return ANY;
}

// WiFi initialization
picofuse_state_t main_wifi_init(picofuse_t *self, picofuse_event_t event, void *data)
{
    picofuse_wifi_t *wifi = (picofuse_wifi_t *)data;
#ifdef WIFI_SSID
    wifi->ssid = WIFI_SSID;
#endif
#ifdef WIFI_PASSWORD
    wifi->password = WIFI_PASSWORD;
#endif
    return ANY;
}

int main()
{
    // Initialize the picofuse object with debugging enabled
    picofuse_t *picofuse = picofuse_init(PICOFUSE_DEBUG);

    // Register callbacks
    picofuse_register(picofuse, ANY, EV_INIT, main_init);
    picofuse_register(picofuse, ANY, EV_WIFI_INIT, main_wifi_init);

    // Call main loop
    picofuse_main(picofuse);
}
