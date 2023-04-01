
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/apps/mqtt.h"
#include "wifiopts.h"

int main() {
    stdio_init_all();

    // Initialize WiFi
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed\n");
        return 1;
    }
    cyw43_arch_enable_sta_mode();

    // Get MQTT broker IP address
    ip_addr_t addr;
    if (!ip4addr_aton(MQTT, &addr)) {
        printf("ip error\n");
        return 0;
    }

    // Connect to WiFI
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("failed to connect to %s\n",WIFI);
        return 1;
    }
    printf("connected to %s\n",WIFI);

    // Connect to MQTT broker


    while (true) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(250);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(250);
    }
}
