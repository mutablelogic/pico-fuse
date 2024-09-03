#include <picofuse/picofuse.h>

int run(fuse_t *fuse)
{
    // GPIO Pin 26 input
    fuse_gpio_t *pin = fuse_new_gpio(fuse, 26, FUSE_GPIO_IN);
    assert(pin);

    return 0;
}
