#include <fuse/fuse.h>
#include <picofuse/picofuse.h>

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

int fuse_main(fuse_flag_t flags, int main(fuse_t *fuse))
{
#if TARGET == pico
    fuse_stdio_init();
#endif
    fuse_debugf(NULL, "picofuse_main: flags=%d\n", flags);

    // Create a fuse application
    fuse_t *fuse = fuse_new(flags);
    assert(fuse);

    // Initialize the LED
    fuse_led_new();

    // Call the main function
    // If the exit code is non-zero then print
    int exit_code = main(fuse);

    // De-initialize the LED
    fuse_led_destroy();

    // Release the fuse application
    fuse_destroy(fuse);

    // Report terminating the application
    fuse_debugf(fuse, "picofuse_main: exit code %d\n", exit_code);

    // Endless loop on the pico
#if TARGET == pico
    while (1)
    {
        sleep_ms(1000);
    }
#endif

    // Return success
    return exit_code;
}
