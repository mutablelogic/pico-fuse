
#include <picofuse/picofuse.h>
#include <stdio.h>

picofuse_state_t main_init(picofuse_t *self, picofuse_event_t event, void *data)
{
    printf("main_init: deviceIdentifier=%s\n", ((picofuse_init_t* )data)->deviceIdentifier);

    ((picofuse_init_t* )data)->countryCode = "DE";

    return ANY;
}

picofuse_state_t main_led_init(picofuse_t *self, picofuse_event_t event, void *data)
{
    printf("main_led_init: gpio=%d value=%d\n", ((picofuse_led_t* )data)->gpio,((picofuse_led_t* )data)->value);

    // Turn on the LED
    ((picofuse_led_t* )data)->value = 1;
    picofuse_fire(self, EV_LED, data);

    return ANY;
}

int main()
{
    // Initialize the picofuse object with debugging enabled
    picofuse_t *picofuse = picofuse_init(PICOFUSE_DEBUG);

    // Register callbacks
    picofuse_register(picofuse, ANY, EV_INIT, main_init);
    picofuse_register(picofuse, ANY, EV_LED_INIT, main_led_init);

    // Call main loop
    int errorCode = picofuse_main(picofuse);

    // Free resources
    picofuse_free(picofuse);

    // Return error code
    return errorCode;
}
