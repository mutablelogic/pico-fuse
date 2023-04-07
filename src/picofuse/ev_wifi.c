#include <picofuse/picofuse.h>
#include <pico/stdlib.h>

#ifdef PICO_CYW43_SUPPORTED
#include <pico/cyw43_arch.h>
#endif

#include "ev.h"
#include "debug.h"

///////////////////////////////////////////////////////////////////////////////
// Handle the EVENT_WIFI_INIT event

void picofuse_handle_wifi_init(picofuse_t * self, picofuse_wifi_t * data) {
    data->ssid = NULL;
    data->password = NULL;
    
    // Call the registered event handler
    picofuse_callback(self, picofuse_event(self), (void *)(data));

#ifdef PICO_CYW43_SUPPORTED
    // Connect
    if (data->ssid != NULL)
    {
        // TODO: Allow AP mode
        cyw43_arch_enable_sta_mode();

        int err = cyw43_arch_wifi_connect_async(data->ssid, data->password, CYW43_AUTH_WPA2_MIXED_PSK);
        if (err != 0)
        {
            picofuse_debug("picofuse_handle_wifi_init: Failed to connect to wifi ssid=%s err=%d\n", data->ssid, err);
        }
        else
        {
            // TODO start repeating timer for cyw43_wifi_link_status
            picofuse_debug("picofuse_handle_wifi_init: Connecting to wifi ssid=%s\n", data->ssid);
        }
    }
#else
    picofuse_debug("picofuse_handle_wifi_init: PICO_CYW43_SUPPORTED not defined\n");
#endif
}
