#include <stdio.h>
#include <assert.h>

#include <fuse/fuse.h>
#include <rp2040/gpio.h>
#include <pico/stdlib.h>

int main()
{
    // Create application
    fuse_t *app = fuse_init(1024, FUSE_FLAG_DEBUG);

    // Debug
    fuse_debugf(app,"fusetest: starting run loop\n");

    // Register GPIO
    void* gpio = fuse_register_driver(app, &rp2040_gpio, NULL);
    assert(gpio);

    // Runloop until exit
    fuse_main(app);

    // Exit
    return fuse_exit(app);
}
