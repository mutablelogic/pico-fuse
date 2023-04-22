#include <fuse/fuse.h>
#include <rp2040/gpio.h>
#include <rp2040/pwm.h>
#include <pimoroni/picodisplay.h>

///////////////////////////////////////////////////////////////////////////////

typedef struct
{
    fuse_t *fuse;
} picodisplay_t;

///////////////////////////////////////////////////////////////////////////////

/*
 * Register GPIO pins
 */
static void picodisplay_register_pins(fuse_t *fuse, fuse_driver_t *driver, fuse_event_t event, void *data)
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
        pin->irqrise = true;
        break;
    case 6:
    case 7:
    case 8:
        pin->func = GPIO_PWM;
        break;
    }
}

/*
 * GPIO pin fired
 */
void picodisplay_pin_event(fuse_t *fuse, fuse_driver_t *driver, fuse_event_t event, void *data)
{
    gpio_pin_t *pin = (gpio_pin_t *)data;

    fuse_debugf(fuse, "picodisplay_pin_event: gpio=%d, irqfall=%d, irqrise=%d\n", pin->gpio, pin->irqfall, pin->irqrise);
}

/*
 * PWM init
 */
void picodisplay_pwm_init(fuse_t *fuse, fuse_driver_t *driver, fuse_event_t event, void *data)
{
    pwm_t *pwm = data;

    // Set 75% duty cycle at 1Hz
    pwm->freq = 10;
    pwm->duty_cycle = 50;
}

/*
 * Register picodisplay
 */
static void *picodisplay_register(fuse_t *fuse, void *data)
{
    assert(fuse);

    // Register the GPIO driver
    fuse_register_driver(fuse, &rp2040_gpio, NULL);
    fuse_register_driver(fuse, &rp2040_pwm, NULL);
    
    // Create a inkypack object
    picodisplay_t *self = fuse_malloc(fuse, sizeof(picodisplay_t));
    if (self == NULL)
    {
        fuse_debugf(fuse, "picodisplay_register: failed to allocate memory for driver\n");
        return NULL;
    }

    // Set fuse object
    self->fuse = fuse;

    fuse_register_action(fuse, EV_GPIO_INIT, self, picodisplay_register_pins);
    fuse_register_action(fuse, EV_GPIO, self, picodisplay_pin_event);
    fuse_register_action(fuse, EV_PWM_INIT, self, picodisplay_pwm_init);

    // Return success
    return self;
}

///////////////////////////////////////////////////////////////////////////////

fuse_driver_params_t pimoroni_picodisplay = {
    .name = "pimironi_picodisplay",
    .init = picodisplay_register,
    .events = {
        {.event = EV_PICODISPLAY_A, .name = "EV_PICODISPLAY_A"},
        {.event = EV_PICODISPLAY_B, .name = "EV_PICODISPLAY_B"},
        {.event = EV_PICODISPLAY_X, .name = "EV_PICODISPLAY_X"},
        {.event = EV_PICODISPLAY_Y, .name = "EV_PICODISPLAY_Y"},
        {}}};
