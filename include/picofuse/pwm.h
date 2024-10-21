/** @file pwm.h
 *  @brief Function prototypes for the PWM interface
 *
 *  This file contains the function prototypes for accessing the PWM hardware
 */
#ifndef PICOFUSE_PWM_H
#define PICOFUSE_PWM_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#ifdef DEBUG
#define fuse_new_pwm(self, data) \
    ((fuse_pwm_t *)fuse_new_pwm_ex((self), (data), __FILE__, __LINE__))
#else
#define fuse_new_pwm(self, data) \
    ((fuse_pwm_t *)fuse_new_pwm_ex((self), (data), 0, 0))
#endif

/** @brief An opaque PWM object
 */
typedef struct pwm_context fuse_pwm_t;

/** @brief PWM data used to initialize
 *
 *  This structure is used to initialize a PWM interface.
 *
 *  The frequency is requested in Hz, and must be greater than zero. The actual frequency
 *  may differ from that requested.
 *
 *  The duty cycle is the proportion that the signal is in high state compared to low
 *  state on each channel. If the duty cycle is zero, the channel is not initialized.
 *
 *  The GPIO pins are used to control the PWM output, and must both be allocated for the same
 *  "slice" or PWM instance.
 *
 *  If the PWM should not be enabled after initialization, set the "disabled" flag to true.
 *  If the PWM should generate a FUSE_EVENT_PWM event when the counter wraps around, set 
 *  the "event" flag to true.
 */
typedef struct
{
    uint32_t freq;        ///< Requested frequency (actual frequency may be different)
    uint8_t duty_cycle_a; ///< Initial duty cycle for Channel A (1-255), or zero if channel unused
    uint8_t duty_cycle_b; ///< Initial duty cycle for Channel B (1-255), or zero if channel unused
    uint8_t gpio_a;       ///< GPIO pin for Channel A
    uint8_t gpio_b;       ///< GPIO pin for Channel B
    bool disabled;        ///< True if the PWM should not be enabled after initialization
    bool event;           ///< True if the PWM should generate an IRQ
} fuse_pwm_config_t;

/** @brief Initialize a PWM interface
 *
 * @param self The fuse application
 * @param data The PWM configuration
 * @return The PWM interface, or NULL if the PWM interface could not be initialized
 */
fuse_pwm_t *fuse_new_pwm_ex(fuse_t *self, fuse_pwm_config_t data, const char *file, const int line);

/** @brief Set duty cycle for Channel A
 *
 *  This function sets the duty cycle for Channel A. If Channel A is not initialized, this
 *  function has no effect.
 * 
 * @param self The fuse application
 * @param pwm The PWM context
 * @param duty_cycle The duty cycle for Channel A, 1-255
 */
void fuse_pwm_set_duty_cycle_a(fuse_t *self, fuse_pwm_t *pwm, uint8_t duty_cycle);

/** @brief Set duty cycle for Channel B
 *
 *  This function sets the duty cycle for Channel B. If Channel B is not initialized, this
 *  function has no effect.
 * 
 * @param self The fuse application
 * @param pwm The PWM context
 * @param duty_cycle The duty cycle for Channel B, 1-255
 */
void fuse_pwm_set_duty_cycle_b(fuse_t *self, fuse_pwm_t *pwm, uint8_t duty_cycle);

/** @brief Enable or disable the PWM instance
 *
 * @param self The fuse application
 * @param pwm The PWM context
 * @param enabled Set to true to enable the PWM
 */
void fuse_pwm_set_enabled(fuse_t *self, fuse_pwm_t *pwm, bool enabled);

#endif
