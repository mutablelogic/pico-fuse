#include <picofuse/picofuse.h>
#include <pico/stdlib.h>

/* @brief Callback when there is a wrap-around on the PWM counter
 */
void pwm_callback(fuse_t *self, fuse_event_t *evt, void *user_data)
{
    assert(self);
    assert(evt);

    static int fade = 0;
    static bool going_up = true;

    if (going_up)
    {
        ++fade;
        if (fade > 0xFF)
        {
            fade = 0xFF;
            going_up = false;
        }
    }
    else
    {
        --fade;
        if (fade < 1)
        {
            fade = 1;
            going_up = true;
        }
    }

    fuse_pwm_set_duty_cycle_b(self, (fuse_pwm_t *)user_data, fade);
}

int run(fuse_t *self)
{
    fuse_pwm_config_t pwm_config = {
        .gpio_b = PICO_DEFAULT_LED_PIN,
        .duty_cycle_b = 0xFF,
        .freq = 512 * 1000,
        .event = true,
    };
    fuse_watchdog_config_t watchdog_config = {};

    // Watchdog
    fuse_watchdog_t *watchdog = (fuse_watchdog_t *)fuse_retain(self, fuse_new_watchdog(self, watchdog_config));
    assert(watchdog);
    fuse_debugf(self, "WATCHDOG: %v\n", watchdog);

    // PWM
    fuse_pwm_t *pwm = (fuse_pwm_t *)fuse_retain(self, fuse_new_pwm(self, pwm_config));
    assert(pwm);
    fuse_register_callback(self, FUSE_EVENT_PWM, 0, pwm_callback);
    fuse_debugf(self, "PWM: %v\n", pwm);

    return 0;
}
