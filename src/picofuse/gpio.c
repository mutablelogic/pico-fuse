#include <fuse/fuse.h>
#include <picofuse/picofuse.h>

#include <pico/stdlib.h>
#include <hardware/gpio.h>
#include <hardware/adc.h>

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

inline uint8_t fuse_gpio_count()
{
#if TARGET == pico
    return NUM_BANK0_GPIOS;
#else
    return 0;
#endif
}

void fuse_gpio_new(uint8_t pin, picofuse_func_t func)
{
    assert(pin < fuse_gpio_count());

    // Set GPIO pin to SIO
    gpio_init(pin);

    // If true, adc_init has been called
    static bool adc = false;

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
        fuse_adc_new(pin);
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

void fuse_gpio_destroy(uint8_t pin)
{
    assert(pin < fuse_gpio_count());

    // TODO: If the pin is the temperature sensor, then disable it

    gpio_deinit(pin);
}

void fuse_gpio_set(uint8_t pin, bool value)
{
    assert(pin < fuse_gpio_count());
    gpio_put(pin, value);
}

bool fuse_gpio_get(uint8_t pin)
{
    assert(pin < fuse_gpio_count());
    return gpio_get(pin);
}
