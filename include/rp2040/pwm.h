#ifndef RP2040_PWM_H
#define RP2040_PWM_H

#include <stdint.h>
#include <fuse/fuse.h>

/*
 * Events
 */
#define EV_PWM_INIT (fuse_event_t)(0x0301)
#define EV_PWM (fuse_event_t)(0x0302)

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

    // The frequency of the PWM slice, in Hz
    // Set to zero to disable the PWM
    uint32_t freq;

    // The duty cycle percentage for the channel, set between
    // 0 and 100
    int duty_cycle;

    // Fire an event when the slice counter is wrapped
    bool irqwrap;
} pwm_t;

/*
 * Define the PWM driver
 */
extern fuse_driver_params_t rp2040_pwm;

#endif
