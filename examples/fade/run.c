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
        if (fade > 255)
        {
            fade = 255;
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
    fuse_pwm_config_t config = {
        .b = PICO_DEFAULT_LED_PIN,
        .freq = 256 * 1000,
    };

    // PWM
    fuse_pwm_t *pwm = (fuse_pwm_t *)fuse_retain(self, fuse_new_pwm(self, config));
    assert(pwm);

    // Register a callback on Core 0
    fuse_register_callback(self, FUSE_EVENT_PWM, 0, pwm_callback);

    fuse_debugf(self, "PWM: %v\n", pwm);

    return 0;
}
