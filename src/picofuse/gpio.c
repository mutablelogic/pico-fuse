#include <picofuse/picofuse.h>
#include <hardware/gpio.h>
#include <stdio.h>
#include "gpio.h"
#include "printf.h"

///////////////////////////////////////////////////////////////////////////////
// DECLARATIONS

static void fuse_gpio_setfunc(uint8_t pin, fuse_gpio_func_t func);

/** @brief Append a string representation of a GPIO pin
 */
static size_t fuse_gpio_cstr(fuse_t *self, char *buf, size_t sz, size_t i, fuse_value_t *v);

/** @brief Callback for interrupt
 */
static void fuse_gpio_callback(uint pin, uint32_t events);

///////////////////////////////////////////////////////////////////////////////
// GLOBALS

static fuse_gpio_t * fuse_gpio_pin[NUM_BANK0_GPIOS];
static fuse_t* fuse_gpio_instance = NULL;

///////////////////////////////////////////////////////////////////////////////
// LIFECYCLE

/** @brief Register value type for gpio
 */
void fuse_register_value_gpio(fuse_t *self)
{
    assert(self);

    // Register mutex type
    fuse_value_desc_t fuse_gpio_type = {
        .size = sizeof(struct gpio_context),
        .name = "GPIO",
        .destroy = NULL, // TODO: We meed to have a destroy function
        .cstr = fuse_gpio_cstr,
        .qstr = fuse_gpio_cstr // TODO: we need to implement a qstr function
    };
    fuse_register_value_type(self, FUSE_MAGIC_GPIO, fuse_gpio_type);

    // Reset the GPIO pins
    for(size_t i = 0; i < NUM_BANK0_GPIOS; i++) {
        fuse_gpio_pin[i] = NULL;
    }

    // Set the callback instance
    fuse_gpio_instance = self;
}

//////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

/** @brief Initialize a GPIO pin with a function
 *
 *  @param pin The GPIO pin number
 *  @param func The function of the GPIO pin
 */
fuse_gpio_t *fuse_new_gpio_ex(fuse_t *self, uint8_t pin, fuse_gpio_func_t func, const char *file, const int line) {
    assert(self);
    assert(pin < fuse_gpio_count());
    assert(func < FUSE_GPIO_FUNC_COUNT);

    // If pin alreadt initialized, return
    if(fuse_gpio_pin[pin] != NULL) {
        return NULL;
    }

    // Create a new GPIO context
    fuse_gpio_t *ctx = (fuse_gpio_t *)fuse_new_value_ex(self, FUSE_MAGIC_GPIO, (void* )pin, file, line);
    if(ctx == NULL) {
        return NULL;
    }

    // Set the GPIO pin
    ctx->pin = pin;

    // Indicate the GPIO pin is initialized
    fuse_gpio_pin[pin] = ctx;

    // Set up the interrupt for RISE and FALL events
    gpio_set_irq_enabled_with_callback(pin, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &fuse_gpio_callback);

    // Set the GPIO function
    fuse_gpio_setfunc(pin, func);

    // Return the GPIO context
    return ctx;
}

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

/** @brief Return the GPIO cound (GPIO[0]...GPIO[count-1]) 
 */
inline uint8_t fuse_gpio_count()
{
    return NUM_BANK0_GPIOS;
}

/** @brief Set the GPIO pin to be low or high
 */
inline void fuse_gpio_set(fuse_gpio_t *pin, bool value) {
    assert(pin);
    assert(pin->pin < fuse_gpio_count());
    gpio_put(pin->pin, value);
}

/** @brief Get the binary value of a GPIO pin
 */
inline bool fuse_gpio_get(fuse_gpio_t *pin, bool value) {
    assert(pin);
    assert(pin->pin < fuse_gpio_count());
    return gpio_get(pin->pin);
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE METHODS

static void fuse_gpio_setfunc(uint8_t pin, fuse_gpio_func_t func)
{
    assert(pin < fuse_gpio_count());
    assert(func < FUSE_GPIO_FUNC_COUNT);

    // Set GPIO pin to SIO
    gpio_init(pin);

    // Set function based on function
    switch (func)
    {
    case FUSE_GPIO_IN:
        gpio_set_dir(pin, GPIO_IN);
        gpio_set_pulls(pin, false, false);
        break;
    case FUSE_GPIO_OUT:
        gpio_set_dir(pin, GPIO_OUT);
        break;
    case FUSE_GPIO_PULLDN:
        gpio_set_dir(pin, GPIO_IN);
        gpio_set_pulls(pin, false, true);
        break;
    case FUSE_GPIO_PULLUP:
        gpio_set_dir(pin, GPIO_IN);
        gpio_set_pulls(pin, true, false);
        break;
    case FUSE_GPIO_PWM:
        gpio_set_function(pin, GPIO_FUNC_PWM);
        break;
    case FUSE_GPIO_SPI:
        gpio_set_function(pin, GPIO_FUNC_SPI);
        break;
    case FUSE_GPIO_ADC:
        // TODO
        //fuse_adc_new(pin);
        break;
    case FUSE_GPIO_I2C:
        gpio_set_function(pin, FUSE_GPIO_I2C);
        break;
    case FUSE_GPIO_UART:
        gpio_set_function(pin, FUSE_GPIO_UART);
        break;
    default:
        assert(false);
    }
}

/** @brief Append a string representation of a GPIO pin
 */
static size_t fuse_gpio_cstr(fuse_t *self, char *buf, size_t sz, size_t i, fuse_value_t *v) {
    assert(self);
    assert(buf == NULL || sz > 0);
    assert(v);

    i = cstrtostr_internal(buf, sz, i, self->desc[FUSE_MAGIC_GPIO].name);
    i = utostr_internal(buf, sz, i, ((fuse_gpio_t *)v)->pin, 0);

    return i;
}

/** @brief GPIO callback - place event on the queues
 */
static void fuse_gpio_callback(uint pin, uint32_t events) {
    fuse_t* self = fuse_gpio_instance;
    fuse_gpio_t* source = fuse_gpio_pin[pin];
    if(self && pin) {
        fuse_event_t* evt = fuse_new_event(self, (fuse_value_t* )source, FUSE_EVENT_GPIO, (void* )events);
        assert(evt);
    }
}

/*
picofuse_func_t fuse_gpio_func(uint8_t pin) {
    assert(pin < fuse_gpio_count());

    // Get the GPIO function
    uint gpio_func = gpio_get_function(pin);
    switch(gpio_func) {
        default:
            assert(false);
    }
}

void fuse_gpio_destroy(uint8_t pin)
{
    assert(pin < fuse_gpio_count());

    // If the pin is set to ADC, then disable it
    if(fuse_adc_channel(pin) != FUSE_ADC_INVALID) {
        fuse_adc_destroy(pin);
    }

    gpio_deinit(pin);
}

void fuse_gpio_event_enable(fuse_t *fuse, uint8_t pin, bool rising, bool falling) {

}
*/
