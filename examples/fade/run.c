#include <picofuse/picofuse.h>

int run(fuse_t *self)
{
    // Assumes the LED is on GPIO 25
    // Blink every 1 second
    fuse_pwm_config_t config = {
        .a = 25,
        .freq = 125 * 1000 * 1000 / 4
    };

    // PWM
    fuse_debugf(self, "Create pwm\n");
    fuse_pwm_t *pwm = (fuse_pwm_t *)fuse_retain(self, fuse_new_pwm(self, config));
    assert(pwm);

    fuse_debugf(self, "PWM: %v\n", pwm);

    return 0;
}
