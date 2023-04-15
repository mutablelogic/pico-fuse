#ifndef RP2040_GPIO_H
#define RP2040_GPIO_H

#include <fuse/fuse.h>

/*
 * Register the GPIO driver
 */
void* rp2040_gpio_register(fuse_t *fuse, void *data);

/*
 * Define the GPIO driver
 */
const fuse_driver_params_t rp2040_gpio = {
    .name = "rp2040_gpio",
    .init = rp2040_gpio_register,
};

#endif
