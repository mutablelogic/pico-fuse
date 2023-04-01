
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "wifi.h"
#include "mqtt.h"
#include "opts.h"

int main() {
    stdio_init_all();
    int err = wifi_init(WIFI, WIFI_PASSWORD);
    if(err != PICO_OK) {
        printf("wifi init failed with error code=%d\n",err);
        return 0;
    }

    mqtt_client* client = mqtt_init();
    if(client == NULL) {
        printf("mqtt init failed\n");
        return 0;
    }

    while (true) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(250);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(250);
    }
}
