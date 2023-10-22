#include <pico/stdlib.h>
#include <fuse/fuse.h>
#include <picofuse/picofuse.h>

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

    // Set function based on function
    switch (func)
    {
    case FUSE_GPIO_IN:
        gpio_set_dir(pin, GPIO_IN);
        gpio_set_pulls(pin, false, false);
    case FUSE_GPIO_OUT:
        gpio_set_dir(pin, GPIO_OUT);
    case FUSE_GPIO_PULLDN:
        gpio_set_dir(pin, GPIO_IN);
        gpio_set_pulls(pin, false, true);
    case FUSE_GPIO_PULLUP:
        gpio_set_dir(pin, GPIO_IN);
        gpio_set_pulls(pin, true, false);
    case FUSE_GPIO_PWM:
        gpio_set_function(pin,GPIO_FUNC_PWM);
    }
}

void fuse_gpio_destroy(uint8_t pin)
{
}

void fuse_gpio_set(uint8_t pin, bool value)
{
}

bool fuse_gpio_get(uint8_t pin)
{
}
