

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

int wifi_init(const char *ssid, const char *password)
{
    if (cyw43_arch_init())
    {
        return PICO_ERROR_GENERIC;
    }

    // Enable station (AP client) mode.
    cyw43_arch_enable_sta_mode();

    // Connect and return any errors
    return cyw43_arch_wifi_connect_timeout_ms(ssid, password, CYW43_AUTH_WPA2_AES_PSK, 30000);
}

