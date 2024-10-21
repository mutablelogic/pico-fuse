#include <picofuse/picofuse.h>
#include <pico/stdlib.h>

/* @brief Callback when there is a wrap-around on the PWM counter
 */
void pwm_callback(fuse_t *self, fuse_event_t *evt, void *user_data)
{
    assert(self);
    assert(evt);

    // Print the event
    fuse_printf(self, "Event: evt=%v\n", evt);
}

int run(fuse_t *self)
{
    // Assumes the LED is on GPIO 25
    // Blink every 1 second
    fuse_pwm_config_t config = {
        .a = PICO_DEFAULT_LED_PIN,
        .freq = 16
    };

    // PWM
    fuse_debugf(self, "Create pwm\n");
    fuse_pwm_t *pwm = (fuse_pwm_t *)fuse_retain(self, fuse_new_pwm(self, config));
    assert(pwm);

    // Register a callback on Core 0
    assert(fuse_register_callback(self, FUSE_EVENT_PWM, 0, pwm_callback));

    fuse_debugf(self, "PWM: %v\n", pwm);

    return 0;
}
