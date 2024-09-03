#include <picofuse/picofuse.h>

int run(fuse_t *fuse)
{
    // Initialize picofuse
    picofuse_init(fuse);

    // GPIO Pin 26 input
    fuse_gpio_t *pin = fuse_new_gpio(fuse, 23, FUSE_GPIO_IN);
    assert(pin);

    fuse_printf(fuse,"gpio %v", pin);

    return 0;
}
