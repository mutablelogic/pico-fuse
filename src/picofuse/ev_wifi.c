#include <picofuse/picofuse.h>
#include <pico/stdlib.h>

#ifdef PICO_CYW43_SUPPORTED
#include <pico/cyw43_arch.h>
#endif

#include "ev.h"
#include "debug.h"

///////////////////////////////////////////////////////////////////////////////
// Return the WiFi status string

#ifdef PICO_CYW43_SUPPORTED
static const char *picofuse_link_status_str(int status)
{
    switch (status)
    {
    case CYW43_LINK_DOWN:
        return "link down";
    case CYW43_LINK_JOIN:
        return "joining";
    case CYW43_LINK_NOIP:
        return "no ip";
    case CYW43_LINK_UP:
        return "link up";
    case CYW43_LINK_FAIL:
        return "link fail";
    case CYW43_LINK_NONET:
        return "network fail";
    case CYW43_LINK_BADAUTH:
        return "bad auth";
    }
    return "unknown";
}
#endif

///////////////////////////////////////////////////////////////////////////////
// Poll WiFi driver

void picofuse_wifi_poll(picofuse_t *self)
{
#if PICO_CYW43_SUPPORTED
    cyw43_arch_poll();
#endif
}

///////////////////////////////////////////////////////////////////////////////
// Periodically poll the WiFi status

#ifdef PICO_CYW43_SUPPORTED
bool picofuse_handle_wifi_status(picofuse_t *self, void *data)
{
    cyw43_arch_lwip_begin();
    int status = cyw43_wifi_link_status(&cyw43_state, CYW43_ITF_STA);
    cyw43_arch_lwip_end();
    picofuse_debug("picofuse_handle_wifi_status: ssid=%s status=%s\n", ((picofuse_wifi_t *)data)->ssid, picofuse_link_status_str(status));
    return true;
}
#endif

///////////////////////////////////////////////////////////////////////////////
// Handle the EVENT_WIFI_INIT event

void picofuse_handle_wifi_init(picofuse_t *self, picofuse_wifi_t *data)
{
    data->ssid = NULL;
    data->password = NULL;

    // Call the registered event handler
    picofuse_callback(self, picofuse_event(self), (void *)(data));

    //  Connect
    if (data->ssid != NULL)
    {
#ifdef PICO_CYW43_SUPPORTED
        // TODO: Allow AP mode
        cyw43_arch_enable_sta_mode();

        cyw43_arch_lwip_begin();
        int err = cyw43_arch_wifi_connect_async(data->ssid, data->password, CYW43_AUTH_WPA2_MIXED_PSK);
        cyw43_arch_lwip_end();
        if (err != 0)
        {
            picofuse_debug("picofuse_handle_wifi_init: Failed to connect to wifi ssid=%s err=%d\n", data->ssid, err);
        }
        else if (!picofuse_schedule_ms(self, 1000, picofuse_handle_wifi_status, data))
        {
            picofuse_debug("picofuse_handle_wifi_init: Failed to schedule picofuse_handle_wifi_status\n");
        }
#else
        picofuse_debug("picofuse_handle_wifi_init: PICO_CYW43_SUPPORTED not defined\n");
#endif
    }
}
