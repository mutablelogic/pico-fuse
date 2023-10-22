#include <pico/stdlib.h>
#include <fuse/fuse.h>
#include <picofuse/picofuse.h>

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

void picofuse_stdio_init()
{
    // Initialize STDIO
    stdio_init_all();

    // Wait for 1500ms
    sleep_ms(1500);

    // Output to stdio
#ifdef DEBUG
    fuse_debugf(NULL, "picofuse_stdio_init\n");
#endif
}
