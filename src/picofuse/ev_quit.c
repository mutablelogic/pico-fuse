#include <stdlib.h>

#include <picofuse/picofuse.h>

#ifdef PICO_CYW43_SUPPORTED
#include <pico/cyw43_arch.h>
#endif

#include "ev.h"

///////////////////////////////////////////////////////////////////////////////
// Handle the EVENT_INIT event

void picofuse_handle_quit(picofuse_t *self, picofuse_init_t *data)
{
    // Set error code
    data->errorCode = -1;

    // Call the registered event handler
    picofuse_callback(self, picofuse_event(self), (void *)(data));

#ifdef PICO_CYW43_SUPPORTED
    // Deinitialize WiFi driver
    if (data->errorCode)
    {
        cyw43_arch_deinit();
    }
#endif

    // Destroy timer pool, cancel timers
    if (data->errorCode)
    {
        picofuse_timer_deinit(self);
    }
}
