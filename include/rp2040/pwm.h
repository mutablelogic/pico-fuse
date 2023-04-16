#ifndef RP2040_PWM_H
#define RP2040_PWM_H

#include <stdint.h>
#include <fuse/fuse.h>

/*
 * Events
 */
#define EV_PWM_INIT (fuse_event_t)(0x0301)

/*
 * PWM configuration
 */
typedef struct
{
    // The GPIO pin associated with the PWM
    int gpio;

    // The PWM slice
    int slice;

    // The PWM channel, a=0 or b=1
    int channel;

    // The frequency of the PWM channel, in Hz
    // Set to zero to disable the PWM
    uint32_t freq;

    // The duty cycle percentage for channel A
    int duty_cycle;
} pwm_t;

/*
 * Define the PWM driver
 */
extern fuse_driver_params_t rp2040_pwm;

#endif
