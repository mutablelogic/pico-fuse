#include <fuse/fuse.h>
#include <picofuse/picofuse.h>
#include "gpio.h"
#include "led.h"
#include "spi.h"

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

void picofuse_init(fuse_t *fuse)
{
    assert(fuse);

    // Register picofuse types
    fuse_register_value_gpio(fuse);
    fuse_register_value_led(fuse);
    fuse_register_value_spi(fuse);
}
