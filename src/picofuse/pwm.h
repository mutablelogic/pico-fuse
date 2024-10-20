/** @file pwm.h
 *  @brief Private function prototypes and structure definitions for PWM interface.
 */
#ifndef FUSE_PRIVATE_PWM_H
#define FUSE_PRIVATE_PWM_H

#include <picofuse/picofuse.h>
#include <hardware/pwm.h>

/** @brief PWM context
 */
struct pwm_context
{
    fuse_gpio_t *a;   ///< PWM GPIO Channel A
    fuse_gpio_t *b;   ///< PWM GPIO Channel B
    uint8_t slice;    ///< PWM slice
    uint8_t integer;  ///< Integer part of the frequency
    uint8_t fraction; ///< Fractional part of the frequency
    uint16_t wrap;    ///< Wrap value
};

#endif
