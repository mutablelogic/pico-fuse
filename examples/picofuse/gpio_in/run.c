#include <picofuse/picofuse.h>

/* @brief Callback fro when there is a rising or falling edge on GPIO32
 */
void gpio23_callback(fuse_t *self, fuse_event_t *evt, void *user_data)
{
    assert(self);
    assert(evt);
    assert(user_data);

    // Print the event
    fuse_printf(self, "Event: evt=%q user_data=%p\n", evt, user_data);
}

int run(fuse_t *self)
{
    // Initialize picofuse
    picofuse_init(self);

    // GPIO Pin 26 input - BOOLSEL on the pico lipo
    fuse_gpio_t *pin = fuse_new_gpio(self, 23, FUSE_GPIO_IN);
    assert(pin);

    // Register a callback
    assert(fuse_register_callback(self, FUSE_EVENT_GPIO, 0, gpio23_callback));

    return 0;
}
