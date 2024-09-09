#include <picofuse/picofuse.h>

/* @brief Callback when there is a rising or falling edge on GPIO pin
 */
void gpio_callback(fuse_t *self, fuse_event_t *evt, void *user_data)
{
    assert(self);
    assert(evt);
    assert(user_data);

    // Print the event
    fuse_printf(self, "Event: evt=%v user_data=%p\n", evt, user_data);
}

int run(fuse_t *self)
{
    // Initialize picofuse
    picofuse_init(self);

    // GPIO inputs
    assert(fuse_retain(self, fuse_new_gpio(self, 23, FUSE_GPIO_IN)));
    assert(fuse_retain(self, fuse_new_gpio(self, 12, FUSE_GPIO_PULLUP)));
    assert(fuse_retain(self, fuse_new_gpio(self, 13, FUSE_GPIO_PULLUP)));
    assert(fuse_retain(self, fuse_new_gpio(self, 14, FUSE_GPIO_PULLUP)));

    // Register a callback
    assert(fuse_register_callback(self, FUSE_EVENT_GPIO, 0, gpio_callback));

    return 0;
}
