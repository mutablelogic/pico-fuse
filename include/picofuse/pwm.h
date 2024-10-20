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
 */
typedef struct
{
    uint8_t a;            ///< GPIO pin for Channel A
    uint8_t b;            ///< GPIO pin for Channel B, or zero if not used
    uint32_t freq;        ///< Requested frequency (actual frequency may be different)
    bool disabled;        ///< True if the PWM should not be enabled after initialization
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
 * @param self The fuse application
 * @param pwm The PWM context
 * @param duty_cycle The duty cycle for Channel A, 1-255
 */
void fuse_pwm_set_duty_cycle_a(fuse_t *self, fuse_pwm_t *pwm, uint8_t duty_cycle);

/** @brief Set duty cycle for Channel B
 *
 * @param self The fuse application
 * @param pwm The PWM context
 * @param duty_cycle The duty cycle for Channel B, 1-255
 */
void fuse_pwm_set_duty_cycle_b(fuse_t *self, fuse_pwm_t *pwm, uint8_t duty_cycle);

#endif
