/** @file pwm.h
 *  @brief Private function prototypes and structure definitions for PWM interface.
 */
#ifndef FUSE_PRIVATE_PWM_H
#define FUSE_PRIVATE_PWM_H
#include <picofuse/picofuse.h>

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

///////////////////////////////////////////////////////////////////////////////
// DECLARATIONS

/** @brief Initialize a PWM interface
 */
static bool fuse_pwm_init(fuse_t *self, fuse_value_t *value, const void *user_data);

/** @brief Destroy the PWM interface
 */
static void fuse_pwm_destroy(fuse_t *self, fuse_value_t *value);

/** @brief Append a string representation of a PWM interface
 */
static size_t fuse_pwm_str(fuse_t *self, char *buf, size_t sz, size_t i, fuse_value_t *v, bool json);

/** @brief Calculate PWM configuration from frequency
 */
static void fuse_pwm_set_freq(fuse_pwm_t *pwm, uint32_t freq);

/** @brief Return PWM frequency
 */
static uint32_t fuse_pwm_get_freq(fuse_pwm_t *pwm);

/** @brief IRQ callback function - for all PWM
 */
static void fuse_pwm_callback_global();

#endif
