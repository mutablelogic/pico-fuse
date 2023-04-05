
#include <picofuse/picofuse.h>

int main() {
    // Initialize the picofuse object with debugging enabled
    picofuse_t *picofuse = picofuse_init(PICOFUSE_DEBUG);

    // Register callbacks
    picofuse_register(picofuse, EV_INIT, main_init);
    picofuse_register(picofuse, EV_LED, main_led);

    // Call main loop
    int errorCode = picofuse_main(picofuse);

    // Free resources
    picofuse_free(picofuse);

    // Return error code
    return errorCode;
}
