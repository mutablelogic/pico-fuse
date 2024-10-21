#include <picofuse/picofuse.h>
#include <hardware/pwm.h>
#include <hardware/clocks.h>
#include <hardware/irq.h>
#include "fuse.h"
#include "pwm.h"
#include "printf.h"

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

///////////////////////////////////////////////////////////////////////////////
// GLOBALS

static fuse_pwm_t *fuse_pwm[NUM_PWM_SLICES];
static fuse_t *fuse_pwm_instance = NULL;

///////////////////////////////////////////////////////////////////////////////
// LIFECYCLE

/** @brief Register type for PWM
 */
void fuse_register_value_pwm(fuse_t *self)
{
    assert(self);

    fuse_value_desc_t fuse_pwm_type = {
        .size = sizeof(struct pwm_context),
        .name = "PWM",
        .init = fuse_pwm_init,
        .destroy = fuse_pwm_destroy,
        .str = fuse_pwm_str,
    };
    fuse_register_value_type(self, FUSE_MAGIC_PWM, fuse_pwm_type);

    // Register IRQ
    irq_set_exclusive_handler(PWM_DEFAULT_IRQ_NUM(), fuse_pwm_callback_global);
    irq_set_enabled(PWM_DEFAULT_IRQ_NUM(), true);

    // Clear PWM instances
    for (size_t i = 0; i < NUM_PWM_SLICES; i++)
    {
        fuse_pwm[i] = NULL;
    }

    // Set the callback instance
    fuse_pwm_instance = self;
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE METHODS

/** @brief Initialize a PWM interface
 */
static bool fuse_pwm_init(fuse_t *self, fuse_value_t *value, const void *user_data)
{
    assert(self);
    assert(value);
    assert(user_data);

    // Check parameters
    fuse_pwm_t *pwm = (fuse_pwm_t *)value;
    fuse_pwm_config_t *data = (fuse_pwm_config_t *)user_data;
    assert(data->gpio_a < fuse_gpio_count());
    assert(data->gpio_a < fuse_gpio_count());
    assert(data->freq > 0);

    // Channel A
    uint8_t slice = 0xFF;
    if (data->duty_cycle_a > 0)
    {
        slice = pwm_gpio_to_slice_num(data->gpio_a);
        uint8_t ch = pwm_gpio_to_channel(data->gpio_a);
        if (ch != PWM_CHAN_A)
        {
            fuse_debugf(self, "fuse_pwm_init: invalid GPIO%d for Channel A\n", data->gpio_a);
            return false;
        }
    }

    // Channel B
    if (data->duty_cycle_b > 0)
    {
        uint8_t slice_b = pwm_gpio_to_slice_num(data->gpio_b);
        uint8_t ch = pwm_gpio_to_channel(data->gpio_b);
        if (ch != PWM_CHAN_B)
        {
            fuse_debugf(self, "fuse_pwm_init: invalid GPIO%d for Channel B\n", data->gpio_b);
            return false;
        }
        if (slice != 0xFF && slice != slice_b)
        {
            fuse_debugf(self, "fuse_pwm_init: GPIO%d and GPIO%d different slices\n", data->gpio_a, data->gpio_b);
            return false;
        }
        slice = slice_b;
    }

    // If neither channel is initialized, return false
    if (slice > NUM_PWM_SLICES)
    {
        fuse_debugf(self, "fuse_pwm_init: neither channel initialized\n");
        return false;
    }
    else if (fuse_pwm[slice] != NULL)
    {
        fuse_debugf(self, "fuse_pwm_init: slice %d already in use\n", slice);
        return false;
    }

    // Set slice
    pwm->slice = slice;
    fuse_pwm[slice] = pwm;

    // Initialize GPIO A
    if (data->duty_cycle_a > 0)
    {
        pwm->a = fuse_new_gpio(self, data->gpio_a, FUSE_GPIO_PWM);
        if (pwm->a == NULL)
        {
            return false;
        }
    }
    else
    {
        pwm->a = NULL;
    }

    // Initialize GPIO B
    if (data->duty_cycle_b > 0)
    {
        pwm->b = fuse_new_gpio(self, data->gpio_b, FUSE_GPIO_PWM);
        if (pwm->b == NULL)
        {
            return false;
        }
    }
    else
    {
        pwm->b = NULL;
    }

    // Get configuration for PWM
    fuse_pwm_set_freq(pwm, data->freq);

    pwm_config cfg = pwm_get_default_config();
    pwm_config_set_clkdiv_mode(&cfg, PWM_DIV_FREE_RUNNING);
    pwm_config_set_clkdiv_int_frac(&cfg, pwm->integer, pwm->fraction);
    pwm_config_set_wrap(&cfg, pwm->wrap);
    pwm_init(pwm->slice, &cfg, false);

    // Set duty cycles
    if (pwm->a)
    {
        fuse_pwm_set_duty_cycle_a(self, pwm, data->duty_cycle_a);
    }
    if (pwm->b)
    {
        fuse_pwm_set_duty_cycle_b(self, pwm, data->duty_cycle_b);
    }

    // Clear IRQ, then set it
    pwm_clear_irq(pwm->slice);
    pwm_set_irq_enabled(pwm->slice, data->event);

    // Enable PWM
    pwm_set_enabled(pwm->slice, !data->disabled);

    // Retain the GPIO pins
    fuse_retain(self, pwm->a);
    fuse_retain(self, pwm->b);

    // Return true
    return true;
}

/** @brief Destroy the PWM interface
 */
static void fuse_pwm_destroy(fuse_t *self, fuse_value_t *value)
{
    assert(self);
    assert(value);
    fuse_pwm_t *pwm = (fuse_pwm_t *)value;

    // Clear IRQ, then set it
    pwm_clear_irq(pwm->slice);
    pwm_set_irq_enabled(pwm->slice, false);

    // Disable the PWM
    pwm_set_enabled(pwm->slice, false);

    // Release GPIO pins
    fuse_release(self, pwm->a);
    fuse_release(self, pwm->b);

    // Remove the slice from the globals
    fuse_pwm[pwm->slice] = NULL;
}

/** @brief Append a JSON representation of a PWM interface
 */
static size_t fuse_pwm_str(fuse_t *self, char *buf, size_t sz, size_t i, fuse_value_t *value, bool json)
{
    assert(self);
    assert(buf == NULL || sz > 0);
    assert(value);
    fuse_pwm_t *pwm = (fuse_pwm_t *)value;

    // Add prefix
    i = chtostr_internal(buf, sz, i, '{');

    // Add slice number
    i = qstrtostr_internal(buf, sz, i, "slice");
    i = chtostr_internal(buf, sz, i, ':');
    i = utostr_internal(buf, sz, i, pwm->slice, 0);
    i = chtostr_internal(buf, sz, i, ',');

    // Add Channel A
    i = qstrtostr_internal(buf, sz, i, "a");
    i = chtostr_internal(buf, sz, i, ':');
    i = vtostr_internal(self, buf, sz, i, (fuse_value_t *)pwm->a, true);
    i = chtostr_internal(buf, sz, i, ',');

    // Add Channel B
    if (pwm->b)
    {
        i = qstrtostr_internal(buf, sz, i, "b");
        i = chtostr_internal(buf, sz, i, ':');
        i = vtostr_internal(self, buf, sz, i, (fuse_value_t *)pwm->b, true);
        i = chtostr_internal(buf, sz, i, ',');
    }

    // Add integer
    i = qstrtostr_internal(buf, sz, i, "integer");
    i = chtostr_internal(buf, sz, i, ':');
    i = utostr_internal(buf, sz, i, pwm->integer, 0);
    i = chtostr_internal(buf, sz, i, ',');

    // Add fraction
    i = qstrtostr_internal(buf, sz, i, "fraction");
    i = chtostr_internal(buf, sz, i, ':');
    i = utostr_internal(buf, sz, i, pwm->fraction, 0);
    i = chtostr_internal(buf, sz, i, ',');

    // Add wrap
    i = qstrtostr_internal(buf, sz, i, "wrap");
    i = chtostr_internal(buf, sz, i, ':');
    i = utostr_internal(buf, sz, i, pwm->wrap, 0);
    i = chtostr_internal(buf, sz, i, ',');

    // Add frequency
    i = qstrtostr_internal(buf, sz, i, "freq");
    i = chtostr_internal(buf, sz, i, ':');
    i = utostr_internal(buf, sz, i, fuse_pwm_get_freq(pwm), 0);

    // Add suffix
    i = chtostr_internal(buf, sz, i, '}');

    return i;
}

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

/** @brief Initialize a PWM interface
 *
 * @param self The fuse application
 * @param data The PWM configuration
 * @return The PWM interface, or NULL if the PWM interface could not be initialized
 */
fuse_pwm_t *fuse_new_pwm_ex(fuse_t *self, fuse_pwm_config_t data, const char *file, const int line)
{
    assert(self);

    // Register type
    if (!fuse_is_registered_value(self, FUSE_MAGIC_PWM))
    {
        fuse_register_value_pwm(self);
    }

    return (fuse_pwm_t *)fuse_new_value_ex(self, FUSE_MAGIC_PWM, &data, file, line);
}

/** @brief Set duty cycle for Channel A
 *
 * @param self The fuse application
 * @param data The PWM configuration
 * @param duty_cycle The duty cycle for Channel A, 1-255
 */
void fuse_pwm_set_duty_cycle_a(fuse_t *self, fuse_pwm_t *pwm, uint8_t duty_cycle)
{
    assert(self);
    assert(pwm);

    if (pwm->a)
    {
        uint16_t level = (pwm->wrap * duty_cycle) >> 8;
        pwm_set_chan_level(pwm->slice, PWM_CHAN_A, level);
    }
}

/** @brief Set duty cycle for Channel B
 *
 * @param self The fuse application
 * @param data The PWM configuration
 * @param duty_cycle The duty cycle for Channel B, 1-255
 */
void fuse_pwm_set_duty_cycle_b(fuse_t *self, fuse_pwm_t *pwm, uint8_t duty_cycle)
{
    assert(self);
    assert(pwm);

    if (pwm->b)
    {
        uint16_t level = (pwm->wrap * duty_cycle) >> 8;
        pwm_set_chan_level(pwm->slice, PWM_CHAN_B, level);
    }
}

/** @brief Enable or disable the PWM instance
 *
 * @param self The fuse application
 * @param pwm The PWM context
 * @param enabled Set to true to enable the PWM
 */
void fuse_pwm_set_enabled(fuse_t *self, fuse_pwm_t *pwm, bool enabled)
{
    assert(self);
    assert(pwm);
    pwm_set_enabled(pwm->slice, enabled);
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE METHODS

static void fuse_pwm_set_freq(fuse_pwm_t *pwm, uint32_t freq)
{
    assert(pwm);
    assert(freq > 0);

    uint32_t clock = clock_get_hz(clk_sys);
    uint32_t divider16 = (clock / freq) >> 12;
    if (clock % (freq << 12) != 0)
    {
        divider16++;
    }
    pwm->integer = divider16 >> 4;
    pwm->fraction = divider16 & 0x0F;
    pwm->wrap = ((clock << 4) / divider16 / freq) - 1;
}

/** @brief Return PWM frequency
 *
 * @param pwm The PWM interface
 * @return The PWM frequency
 */
static inline uint32_t fuse_pwm_get_freq(fuse_pwm_t *pwm)
{
    assert(pwm);

    uint32_t period = (pwm->wrap + 1) * pwm->integer + (((pwm->wrap + 1) * pwm->fraction) >> 4);
    return clock_get_hz(clk_sys) / period;
}

/** @brief IRQ callback function - for a PWM slice
 *
 * @param self The fuse application
 * @param pwm The PWM context
 */
static inline void fuse_pwm_callback(fuse_t *self, fuse_pwm_t *pwm)
{
    assert(self);
    assert(pwm);
    fuse_event_t *evt = fuse_new_event(self, (fuse_value_t *)pwm, FUSE_EVENT_PWM, pwm);
    assert(evt);
}

/** @brief PWM interrupt callback - wrap
 */
static void fuse_pwm_callback_global()
{
    uint32_t irq = pwm_get_irq_status_mask();

    // Clear interrupts
    for (uint8_t slice = 0; slice < NUM_PWM_SLICES; slice++)
    {
        if (irq & (1 << slice))
        {
            fuse_pwm_t *pwm = fuse_pwm[slice];
            if (pwm)
            {
                fuse_pwm_callback(fuse_pwm_instance, pwm);
            }
            pwm_clear_irq(slice);
        }
    }
}
