#include <picofuse/picofuse.h>

#define LED_DELAY_MS 250

int run(fuse_t* fuse) {
    fuse_led_new();

    while (true) {
        fuse_led_set(1);
        sleep_ms(LED_DELAY_MS);
        fuse_led_set(0);
        sleep_ms(LED_DELAY_MS);
    }

    fuse_led_destroy();
    return 0;
}
