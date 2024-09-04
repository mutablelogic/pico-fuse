#include <picofuse/picofuse.h>
#include <pico/stdlib.h>
#ifdef PICO_CYW43_SUPPORTED
#include <pico/cyw43_arch.h>
#endif
#include "led.h"
#include "fuse.h"
#include "printf.h"

///////////////////////////////////////////////////////////////////////////////
// DECLARATIONS

/** @brief Initialize a LED pin
 */
static bool fuse_led_init(fuse_t *self, fuse_value_t *value, const void *user_data);

/** @brief Disable the LED pin
 */
static void fuse_led_destroy(fuse_t *self, fuse_value_t *value);

/** @brief Append a string representation of the LED
 */
static size_t fuse_led_qstr(fuse_t *self, char *buf, size_t sz, size_t i, fuse_value_t *v);

///////////////////////////////////////////////////////////////////////////////
// LIFECYCLE

/** @brief Register value type for LED
 */
void fuse_register_value_led(fuse_t *self)
{
    assert(self);

    // Register gpio type
    fuse_value_desc_t fuse_led_type = {
        .size = sizeof(struct led_context),
        .name = "LED",
        .init = fuse_led_init,
        .destroy = fuse_led_destroy,
        .cstr = fuse_led_qstr,
        .qstr = fuse_led_qstr // TODO: we need to implement a cstr function
    };
    fuse_register_value_type(self, FUSE_MAGIC_LED, fuse_led_type);
}

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

/** @brief Initialize the on-board LED
 */
fuse_led_t *fuse_new_led_ex(fuse_t *self, const char *file, const int line)
{
    assert(self);

    // Create a new LED context
#if defined(PICO_DEFAULT_LED_PIN)
    fuse_led_t *ctx = (fuse_led_t *)fuse_new_value_ex(self, FUSE_MAGIC_LED, (void *)PICO_DEFAULT_LED_PIN, file, line);
#else
    fuse_led_t *ctx = (fuse_led_t *)fuse_new_value_ex(self, FUSE_MAGIC_LED, 0, file, line);
#endif
    if (ctx == NULL)
    {
        return NULL;
    }

    // Return the GPIO context
    return ctx;
}

#include <stdio.h>

/** @brief Set the LED binary value
 */
void fuse_led_set(fuse_led_t *pin, bool value)
{
    assert(pin);

    // Set via GPIO
    if (pin->pin)
    {
        fuse_gpio_set(pin->pin, value);
    }
#ifdef PICO_CYW43_SUPPORTED
    if (pin->cyw43_pin >= 0)
    {
        cyw43_arch_gpio_put(pin->cyw43_pin, value ? 1 : 0);
    }
#endif
}

/** @brief Get the binary value of the LED
 */
bool fuse_led_get(fuse_led_t *pin)
{
    assert(pin);

    // Get via GPIO
    if (pin->pin)
    {
        return fuse_gpio_get(pin->pin);
    }
#ifdef PICO_CYW43_SUPPORTED
    if (pin->cyw43_pin >= 0)
    {
        return cyw43_arch_gpio_get(pin->cyw43_pin);
    }
#endif

    // Return false
    return false;
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE METHODS

/** @brief Initialize a LED pin
 */
static bool fuse_led_init(fuse_t *self, fuse_value_t *value, const void *user_data)
{
    assert(self);
    assert(value);

    // If pin already initialized, return
    fuse_led_t *ctx = (fuse_led_t *)value;
    size_t pin = (size_t)user_data;
    if (pin)
    {
        // Capture the on-board LED
        ctx->pin = (fuse_gpio_t *)fuse_retain(self, fuse_new_gpio(self, pin, FUSE_GPIO_OUT));
        if (ctx->pin == NULL)
        {
            return false;
        }
    }
    else
    {
        ctx->pin = NULL;
    }

#if defined(CYW43_WL_GPIO_LED_PIN)
    ctx->cyw43_pin = CYW43_WL_GPIO_LED_PIN;
#else
    ctx->cyw43_pin = -1;
#endif

    // Return success
    return true;
}

/** @brief Release the GPIO pin
 */
static void fuse_led_destroy(fuse_t *self, fuse_value_t *value)
{
    assert(self);
    assert(value);

    // Release the gpio
    fuse_release(self, ((fuse_led_t *)value)->pin);
}

/** @brief Append a string representation of the LED
 */
static size_t fuse_led_qstr(fuse_t *self, char *buf, size_t sz, size_t i, fuse_value_t *v)
{
    assert(self);
    assert(buf == NULL || sz > 0);
    assert(v);

    i = cstrtostr_internal(buf, sz, i, self->desc[FUSE_MAGIC_LED].name);

    return i;
}
