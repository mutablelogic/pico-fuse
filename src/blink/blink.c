
#include "pico/stdlib.h"

/* Definitions */
const uint LED_PIN = PICO_DEFAULT_LED_PIN;

int main() {
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    while (true) {
        gpio_put(LED_PIN, 1);
        sleep_ms(250);
        gpio_put(LED_PIN, 0);
        sleep_ms(250);
    }
}
