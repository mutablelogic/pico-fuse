/** @file led.h
 *  @brief Private function prototypes and structure definitions for on-board LED
 */
#ifndef FUSE_PRIVATE_LED_H
#define FUSE_PRIVATE_LED_H

#include <fuse/fuse.h>
#include "gpio.h"

/** @brief LED context
 */
struct led_context
{
    struct gpio_context *pin;
    unsigned int cyw43_pin;
};

/** @brief Register value type for LED
 */
void fuse_register_value_led(fuse_t *self);

#endif
