#include <fuse/fuse.h>

typedef struct
{
} gpio_t;

/*
 * Register GPIO
 */
void *rp2040_gpio_register(fuse_t *fuse, void *data)
{
    gpio_t *self = fuse_malloc(fuse, sizeof(gpio_t));
    if (self == NULL)
    {
        fuse_debugf(fuse, "rp2040_gpio_register: failed to allocate memory for driver\n");
        return NULL;
    }

    // Handle EV_INIT event to initialize each pin
    if (!fuse_register_action(fuse, self, EV_INIT, rp2040_gpio_init_pins))
    {
        fuse_debugf(fuse, "rp2040_gpio_init: failed to register EV_INIT action\n");
        return NULL;
    }

    // Return success
    return self;
}

/*
 * Initialize all GPIO pins
 */
void rp2040_gpio_init_pins(fuse_t *fuse, fuse_event_t event, void *data) {
    assert(fuse);
    assert(event == EV_INIT);

    // TODO: Fire EV_GPIO_INIT for each pin
}
