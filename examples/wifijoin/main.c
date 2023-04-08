#include <stdio.h>
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


// WiFi status
picofuse_state_t main_wifi_status(picofuse_t *self, picofuse_event_t event, void *data)
{
    picofuse_wifi_t *wifi = (picofuse_wifi_t *)data;
    printf("WiFi status=%s addr=%s netmask=%s gateway=%s \n", picofuse_wifi_status_string(wifi->status),wifi->addr,wifi->netmask,wifi->gateway);
    return ANY;
}

int main()
{
    // Initialize the picofuse object with debugging enabled
    picofuse_t *picofuse = picofuse_init(PICOFUSE_DEBUG);

    // Register callbacks
    picofuse_register(picofuse, ANY, EV_INIT, main_init);
    picofuse_register(picofuse, ANY, EV_WIFI_INIT, main_wifi_init);
    picofuse_register(picofuse, ANY, EV_WIFI, main_wifi_status);

    // Call main loop
    picofuse_main(picofuse);
}
