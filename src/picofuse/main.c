#include <fuse/fuse.h>
#include <picofuse/picofuse.h>

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

int fuse_main(int main(fuse_t *fuse))
{
    fuse_stdio_init();
    fuse_debugf("picofuse_main: run\n");

    // Create a fuse application
    fuse_t *fuse = fuse_new(0);
    assert(fuse);

    // Call the main function
    // If the exit code is non-zero then print
    int exit_code = main(fuse);

    // Release the fuse application
    fuse_destroy(fuse);

    // Report terminating the application
    fuse_debugf("picofuse_main: exit code %d\n", exit_code);

    // Endless loop on the pico
#if defined(TARGET_PICO)
    fuse_debugf("picofuse_main: halt\n");
    while (1)
    {
        sleep_ms(1000);
    }
#endif

    // Return success
    return exit_code;
}
