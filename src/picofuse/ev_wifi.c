#include <picofuse/picofuse.h>
#include <pico/stdlib.h>

#ifdef PICO_CYW43_SUPPORTED
#include <pico/cyw43_arch.h>
#endif

#include "ev.h"
#include "debug.h"

///////////////////////////////////////////////////////////////////////////////
// Return the WiFi status

#ifdef PICO_CYW43_SUPPORTED
static picofuse_wifi_status_t picofuse_wifi_link_status(int status)
{
    switch (status)
    {
    case CYW43_LINK_DOWN:
        return WIFI_DOWN;
    case CYW43_LINK_JOIN:
        return WIFI_JOINING;
    case CYW43_LINK_NOIP:
        return WIFI_NOIP;
    case CYW43_LINK_UP:
        return WIFI_UP;
    case CYW43_LINK_FAIL:
        return WIFI_FAIL;
    case CYW43_LINK_NONET:
        return WIFI_DOWN;
    case CYW43_LINK_BADAUTH:
        return WIFI_BADAUTH;
    }
    return WIFI_UNKNOWN;
}
#endif

const char *picofuse_wifi_status_string(picofuse_wifi_status_t status)
{
    switch (status)
    {
    case WIFI_DOWN:
        return "WIFI_DOWN";
    case WIFI_JOINING:
        return "WIFI_JOINING";
    case WIFI_NOIP:
        return "WIFI_NOIP";
    case WIFI_FAIL:
        return "WIFI_FAIL";
    case WIFI_BADAUTH:
        return "WIFI_BADAUTH";
    case WIFI_UP:
        return "WIFI_UP";
    default:
        return "WIFI_UNKNOWN";
    }
}

///////////////////////////////////////////////////////////////////////////////
// Poll WiFi driver - returns false if the queue is empty and nothing more
// to do

bool picofuse_wifi_poll(picofuse_t *self, int delay_ms)
{
#if PICO_CYW43_ARCH_POLL
    cyw43_arch_poll();
#endif
    if (picofuse_is_empty(self))
    {
#if PICO_CYW43_ARCH_POLL
        cyw43_arch_wait_for_work_until(make_timeout_time_ms(delay_ms));
#else
        sleep_ms(delay_ms);
#endif
        return false;
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////////
// Periodically poll the WiFi status

#ifdef PICO_CYW43_SUPPORTED
bool picofuse_handle_wifi_status(picofuse_t *self, void *data)
{
    picofuse_wifi_t *wifi = (picofuse_wifi_t *)data;

    // If the status is joined, then set the IP address
    picofuse_wifi_status_t status = picofuse_wifi_link_status(cyw43_tcpip_link_status(&cyw43_state, CYW43_ITF_STA));
    if (status == WIFI_UP)
    {
        struct netif *n = &cyw43_state.netif[CYW43_ITF_STA];
        ipaddr_ntoa_r(netif_ip_addr4(n),wifi->addr, sizeof(wifi->addr));
        ipaddr_ntoa_r(netif_ip_netmask4(n),wifi->netmask, sizeof(wifi->netmask));
        ipaddr_ntoa_r(netif_ip_gw4(n),wifi->gateway, sizeof(wifi->gateway));
    }

    // If the status has changed, then notify the application
    if (status != ((picofuse_wifi_t *)data)->status)
    {
        ((picofuse_wifi_t *)data)->status = status;
        picofuse_callback(self, EV_WIFI, data);
    }

    // Return true if the status is not joined yet (or failed) so continue to check
    if (status == WIFI_UP || status == WIFI_FAIL || status == WIFI_BADAUTH)
    {
        return false;
    }
    else
    {
        return true;
    }
}
#endif

///////////////////////////////////////////////////////////////////////////////
// Handle the EVENT_WIFI_INIT event

void picofuse_handle_wifi_init(picofuse_t *self, picofuse_wifi_t *data)
{
    data->ssid = NULL;
    data->password = NULL;
    data->status = WIFI_UNKNOWN;
    data->addr[0] = 0;
    data->netmask[0] = 0;
    data->gateway[0] = 0;

    // Call the registered event handler
    picofuse_callback(self, picofuse_event(self), (void *)(data));

    //  Connect
    if (data->ssid != NULL)
    {
#ifdef PICO_CYW43_SUPPORTED
        // TODO: Allow AP mode
        cyw43_arch_enable_sta_mode();

        cyw43_arch_lwip_begin();
        int err = cyw43_arch_wifi_connect_async(data->ssid, data->password, CYW43_AUTH_WPA2_AES_PSK);
        // int err = cyw43_arch_wifi_connect_timeout_ms(data->ssid, data->password, CYW43_AUTH_WPA2_AES_PSK, 3000);
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
