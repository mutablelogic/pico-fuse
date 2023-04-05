#include <stdlib.h>
#include <stdio.h>
#include <picofuse/picofuse.h>
#include <pico/stdlib.h>
#include "ev.h"

///////////////////////////////////////////////////////////////////////////////
// Handle the EVENT_INIT event

void picofuse_handle_init(picofuse_t *self, picofuse_init_t *data)
{
    // Initialize STDIO
    stdio_init_all();

    // Wait for 1s
    sleep_ms(1000);

    // TODO: set the deviceIdentifier

    // Call the registered event handler
    picofuse_callback(self, EV_INIT, (void *)(data));

    // TODO: init cyw43 with the counrty code setting

    // set the appName if not NULL
    if (data->appName != NULL)
    {
        printf("TODO: Set appname=%s\n", data->appName);
    }
}
