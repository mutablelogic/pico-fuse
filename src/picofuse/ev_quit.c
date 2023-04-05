#include <stdlib.h>
#include <picofuse/picofuse.h>
#include "ev.h"

///////////////////////////////////////////////////////////////////////////////
// Handle the EVENT_INIT event

void picofuse_handle_quit(picofuse_t *self, picofuse_init_t *data)
{
    // Set error code
    data->errorCode = -1;

    // Call the registered event handler
    picofuse_callback(self, EV_QUIT, (void *)(data));
}

