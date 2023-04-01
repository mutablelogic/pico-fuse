
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"


#ifndef WIFI_SSID
#define WIFI_SSID "10967b"
#endif
#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD ""
#endif


/* Note this version works with the Pico W and not the Pico */
int main() {
    stdio_init_all();
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed");
        return 1;
    }

    cyw43_arch_enable_sta_mode();
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("failed to connect\n");
        return 1;
    }
    while (true) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(250);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(250);
    }
}
