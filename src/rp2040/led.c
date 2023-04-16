#include <pico/stdlib.h>
#include <fuse/fuse.h>
#include <rp2040/led.h>
#include <rp2040/gpio.h>

///////////////////////////////////////////////////////////////////////////////

typedef struct
{
    int gpio;
    bool cyw43_arch;
} led_t;

///////////////////////////////////////////////////////////////////////////////
// PRIVATE METHODS

/*
 * Initialize all GPIO pins
 */
static void rp2040_led_init(fuse_t *fuse, fuse_driver_t *driver, fuse_event_t event, void *data)
{
    assert(fuse);
    assert(driver);
    assert(data);
    assert(event == EV_GPIO_INIT);

    gpio_pin_t *pin = (gpio_pin_t *)data;
    led_t *self = (led_t *)driver;

    if (pin->gpio == self->gpio && !self->cyw43_arch)
    {
        fuse_debugf(fuse, "rp2040_led_init: pin %d\n");
        pin->func = GPIO_OUTPUT;
    }
}

/*
 * Switch LED on or off
 */
static void rp2040_led_action(fuse_t *fuse, fuse_driver_t *driver, fuse_event_t event, void *data)
{
    assert(fuse);
    assert(driver);
    assert(event == EV_LED);

    led_t *self = (led_t *)driver;
    if (self->cyw43_arch)
    {
#ifdef PICO_CYW43_SUPPORTED
        cyw43_arch_gpio_put(self->gpio, data ? 1 : 0);
#else
        fuse_debugf(fuse, "rp2040_led_action: PICO_CYW43_SUPPORTED not defined\n");
#endif
    }
    else if (self->gpio != -1)
    {
        gpio_put(self->gpio, data ? 1 : 0);
    }
}

/*
 * Register LED
 */
void *rp2040_led_register(fuse_t *fuse, void *data)
{
    led_t *self = fuse_malloc(fuse, sizeof(led_t));
    if (self == NULL)
    {
        fuse_debugf(fuse, "rp2040_led_register: failed to allocate memory for driver\n");
        return NULL;
    }

    self->cyw43_arch = false;
#if defined(PICO_DEFAULT_LED_PIN)
    self->gpio = PICO_DEFAULT_LED_PIN;
#elif defined(CYW43_WL_GPIO_LED_PIN)
    self->cyw43_arch = true;
    self->gpio = CYW43_WL_GPIO_LED_PIN;
#else
    self->gpio = -1;
#endif
    // Register actions
    if (!fuse_register_action(fuse, EV_GPIO_INIT, self, rp2040_led_init))
    {
        return NULL;
    }
    if (!fuse_register_action(fuse, EV_LED, self, rp2040_led_action))
    {
        return NULL;
    }

    // Return success
    return self;
}

///////////////////////////////////////////////////////////////////////////////

fuse_driver_params_t rp2040_led = {
    .name = "rp2040_led",
    .init = rp2040_led_register,
    .events = {
        {.event = EV_LED, .name = "EV_LED"},
        {.event = 0, .name = NULL, .finalizer = NULL}}};
