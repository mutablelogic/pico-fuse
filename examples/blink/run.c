#include <picofuse/picofuse.h>

#define LED_DELAY_MS 250

/* @brief LED Callback
 */
void led_callback(fuse_t *self, fuse_event_t *evt, void *user_data)
{
    assert(self);
    assert(evt);

    // LED is the user_data field
    fuse_led_t *led = (fuse_led_t *)user_data;

    // Blink the LED
    fuse_led_set(led, !fuse_led_get(led));
}

int run(fuse_t *self)
{
    // LED output
    fuse_led_t *led = (fuse_led_t *)fuse_retain(self, fuse_new_led(self));
    assert(led);

    // Register timer callback on core 0
    assert(fuse_register_callback(self, FUSE_EVENT_TIMER, 0, led_callback));

    // Schedule timer to run every second
    assert(fuse_timer_schedule(self, LED_DELAY_MS, true, led));

    return 0;
}
