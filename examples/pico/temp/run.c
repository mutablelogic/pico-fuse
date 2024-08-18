#include <picofuse/picofuse.h>

#define LED_DELAY_MS 2500

int run(fuse_t* fuse) {
    fuse_adc_new(30);
    while (true) {
        float temp = fuse_adc_get_temp(fuse_adc_channel_temp());
        fuse_debugf("Temperature: %.2f\n", temp);
        sleep_ms(LED_DELAY_MS);
    }
    fuse_adc_destroy(30);

    return 0;
}
