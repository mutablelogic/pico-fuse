
#include <picofuse/picofuse.h>

picofuse_state_t main_init(picofuse_t *self,
                           picofuse_state_t state,
                           picofuse_event_t event,
                           void *data)
{
    // TODO: Initialize
}

picofuse_state_t main_led(picofuse_t *self,
                           picofuse_state_t state,
                           picofuse_event_t event,
                           void *data)
{
    // TODO: LED changed state
}

int main()
{
    // Initialize the picofuse object with debugging enabled
    picofuse_t *picofuse = picofuse_init(PICOFUSE_DEBUG);

    // Register callbacks
    picofuse_register(picofuse, ANY, EV_INIT, main_init);
    picofuse_register(picofuse, ANY, EV_LED, main_led);

    // Call main loop
    int errorCode = picofuse_main(picofuse);

    // Free resources
    picofuse_free(picofuse);

    // Return error code
    return errorCode;
}
