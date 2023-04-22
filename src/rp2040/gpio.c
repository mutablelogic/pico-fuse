#include <pico/stdlib.h>
#include <hardware/gpio.h>

#include <fuse/fuse.h>
#include <rp2040/gpio.h>
#include <rp2040/pwm.h>

///////////////////////////////////////////////////////////////////////////////

typedef struct
{
    fuse_t *fuse;
    gpio_pin_t *pins;
} gpio_t;

// The number of GPIO pins in bank 0 (GPIO0-29)
#define GPIO_COUNT NUM_BANK0_GPIOS

// The global GPIO driver
gpio_t *_gpio = NULL;

///////////////////////////////////////////////////////////////////////////////
// PRIVATE METHODS

/*
 * Handle GPIO IRQ
 */
void rp2040_gpio_irq(uint gpio, uint32_t events)
{
    assert(_gpio);
    assert(_gpio->fuse);
    gpio_pin_t *pin = &_gpio->pins[gpio];
    pin->irqrise = events & GPIO_IRQ_EDGE_RISE ? true : false;
    pin->irqfall = events & GPIO_IRQ_EDGE_FALL ? true : false;
    fuse_event_fire(_gpio->fuse, EV_GPIO, pin);
}

/*
 * Initialize all GPIO pins
 */
static void rp2040_gpio_init_pins(fuse_t *fuse, fuse_driver_t *driver, fuse_event_t event, void *data)
{
    assert(fuse);
    assert(driver);
    assert(event == EV_INIT);

    // Create pins
    gpio_t *self = (gpio_t *)driver;
    self->pins = fuse_malloc(fuse, sizeof(gpio_pin_t) * GPIO_COUNT);
    if (self->pins == NULL)
    {
        fuse_debugf(fuse, "rp2040_gpio_init_pins: failed to allocate memory for pins\n");
        return;
    }

    // Call EV_GPIO_INIT for each pin
    for (int i = 0; i < GPIO_COUNT; i++)
    {
        gpio_pin_t *pin = &self->pins[i];
        pin->gpio = i;
        pin->func = GPIO_NONE;
        pin->pullup = false;
        pin->pulldown = false;
        pin->irqrise = false;
        pin->irqfall = false;
        if (!fuse_event_fire(fuse, (fuse_event_t)EV_GPIO_INIT, pin))
        {
            fuse_debugf(fuse, "rp2040_gpio_init_pins: failed to fire EV_GPIO_INIT for pin %d\n", i);
        }
    }
}

/*
 * Finalize a pin after EV_GPIO_INIT
 */
static void rp2040_gpio_init_pin_finalizer(fuse_t *fuse, fuse_driver_t *driver, fuse_event_t event, void *data)
{
    assert(fuse);
    assert(driver);
    assert(event == EV_GPIO_INIT);
    assert(data);

    // gpio_init
    gpio_pin_t *pin = (gpio_pin_t *)data;
    if (pin->gpio >= 0 && pin->gpio < GPIO_COUNT && pin->func != GPIO_NONE)
    {
        gpio_init(pin->gpio);
    }
    else
    {
        return;
    }

    // set function and direction
    uint32_t events = 0;
    switch (pin->func)
    {
    case GPIO_INPUT:
        gpio_set_function(pin->gpio, GPIO_FUNC_SIO);
        gpio_set_dir(pin->gpio, GPIO_IN);
        gpio_set_pulls(pin->gpio, pin->pullup, pin->pulldown);

        // set interrupts
        if (pin->irqrise)
        {
            events |= GPIO_IRQ_EDGE_RISE;
        }
        if (pin->irqfall)
        {
            events |= GPIO_IRQ_EDGE_FALL;
        }
        break;
    case GPIO_OUTPUT:
        gpio_set_function(pin->gpio, GPIO_FUNC_SIO);
        gpio_set_dir(pin->gpio, GPIO_OUT);
        break;
    case GPIO_PWM:
        gpio_set_function(pin->gpio, GPIO_FUNC_PWM);
        fuse_event_fire(fuse, EV_GPIO_PWM_INIT, pin);
        break;
    case GPIO_SPI:
        gpio_set_function(pin->gpio, GPIO_FUNC_SPI);
        fuse_event_fire(fuse, EV_GPIO_SPI_INIT, pin);
        break;
    default:
        gpio_set_function(pin->gpio, GPIO_FUNC_NULL);
    }

    // Set GPIO interrupts
    if (events)
    {
        gpio_set_irq_enabled_with_callback(pin->gpio, events, true, rp2040_gpio_irq);
    }
}

/*
 * Register GPIO
 */
static void *rp2040_gpio_register(fuse_t *fuse, void *data)
{
    assert(fuse);

    // Creat a gpio object
    gpio_t *self = fuse_malloc(fuse, sizeof(gpio_t));
    if (self == NULL)
    {
        fuse_debugf(fuse, "rp2040_gpio_register: failed to allocate memory for driver\n");
        return NULL;
    }

    // Set fuse object
    self->fuse = fuse;
    _gpio = self;

    // When EV_INIT is fired, call rp2040_gpio_init_pins
    if (!fuse_register_action(fuse, EV_INIT, self, rp2040_gpio_init_pins))
    {
        fuse_debugf(fuse, "rp2040_gpio_init: failed to register EV_INIT action\n");
        return NULL;
    }

    // Return success
    return self;
}

///////////////////////////////////////////////////////////////////////////////

fuse_driver_params_t rp2040_gpio = {
    .name = "rp2040_gpio",
    .init = rp2040_gpio_register,
    .events = {
        {.event = EV_GPIO_INIT, .name = "EV_GPIO_INIT", .finalizer = rp2040_gpio_init_pin_finalizer},
        {.event = EV_GPIO, .name = "EV_GPIO"},
        {.event = EV_GPIO_PWM_INIT, .name = "EV_GPIO_PWM_INIT"},
        {.event = EV_GPIO_SPI_INIT, .name = "EV_GPIO_SPI_INIT"},
        {}}};
