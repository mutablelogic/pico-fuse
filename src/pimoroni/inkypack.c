#include <fuse/fuse.h>
#include <rp2040/gpio.h>
#include <pimoroni/inkypack.h>

///////////////////////////////////////////////////////////////////////////////

typedef struct
{
    fuse_t *fuse;
} inkypack_t;

///////////////////////////////////////////////////////////////////////////////

/*
 * Register GPIO pins
 */
static void inkypack_register_pins(fuse_t *fuse, fuse_driver_t *driver, fuse_event_t event, void *data)
{
    gpio_pin_t *pin = (gpio_pin_t *)data;

    // Register pins
    switch (pin->gpio)
    {
    case 12:
    case 13:
    case 14:
    case 15:
        pin->func = GPIO_INPUT;
        pin->pullup = true;
        pin->irqfall = true;
        pin->irqrise = false;
        break;
    case 6:
    case 7:
    case 8:
        pin->func = GPIO_PWM;
    }
}

/*
 * GPIO pin fired
 */
void inkypack_pin_event(fuse_t *fuse, fuse_driver_t *driver, fuse_event_t event, void *data)
{
    gpio_pin_t *pin = (gpio_pin_t *)data;

    fuse_debugf(fuse, "inkypack_pin_event: gpio=%d, irqfall=%d, irqrise=%d\n", pin->gpio, pin->irqfall, pin->irqrise);

    switch (pin->gpio)
    {
    case 12:
        if (pin->irqfall || pin->irqrise)
        {
            fuse_event_fire_bool(fuse, EV_INKY_A, pin->irqfall ? true : false);
        }
        break;
    case 13:
        if (pin->irqfall || pin->irqrise)
        {
            fuse_event_fire_bool(fuse, EV_INKY_B, pin->irqfall ? true : false);
        }
        break;
    case 14:
        if (pin->irqfall || pin->irqrise)
        {
            fuse_event_fire_bool(fuse, EV_INKY_C, pin->irqfall ? true : false);
        }
        break;
    }
}

/*
 * Register inkypack
 */
static void *inkypack_register(fuse_t *fuse, void *data)
{
    assert(fuse);

    // Register the GPIO driver
    if (!fuse_register_driver(fuse, &rp2040_gpio, NULL))
    {
        fuse_debugf(fuse, "inkypack_register: failed to register GPIO driver\n");
        return NULL;
    }

    // Create a inkypack object
    inkypack_t *self = fuse_malloc(fuse, sizeof(inkypack_t));
    if (self == NULL)
    {
        fuse_debugf(fuse, "inkypack_register: failed to allocate memory for driver\n");
        return NULL;
    }

    // Set fuse object
    self->fuse = fuse;

    // When EV_GPIO_INIT is fired register pins
    if (!fuse_register_action(fuse, EV_GPIO_INIT, self, inkypack_register_pins))
    {
        fuse_debugf(fuse, "inkypack_register: failed to register EV_GPIO_INIT action\n");
        return NULL;
    }

    // When EV_GPIO is fired then call inkypack_pin_event
    if (!fuse_register_action(fuse, EV_GPIO, self, inkypack_pin_event))
    {
        fuse_debugf(fuse, "inkypack_register: failed to register EV_GPIO action\n");
    }

    // Return success
    return self;
}

///////////////////////////////////////////////////////////////////////////////

fuse_driver_params_t pimoroni_inkypack = {
    .name = "pimironi_inkypack",
    .init = inkypack_register,
    .events = {
        {.event = EV_INKY_A, .name = "EV_INKY_A"},
        {.event = EV_INKY_B, .name = "EV_INKY_B"},
        {.event = EV_INKY_C, .name = "EV_INKY_C"},
        {}}};
