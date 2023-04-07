#include <picofuse/picofuse.h>
#include <pico/stdlib.h>

#include "ev.h"
#include "debug.h"

///////////////////////////////////////////////////////////////////////////////
// Handle the EVENT_GPIO_INIT event

void picofuse_handle_gpio_init(picofuse_t *self, picofuse_gpio_t *data)
{
    // Call the registered event handler
    picofuse_callback(self, picofuse_event(self), (void *)(data));
}
