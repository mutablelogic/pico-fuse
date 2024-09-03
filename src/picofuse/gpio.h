/** @file gpio.h
 *  @brief Private function prototypes and structure definitions for gpio pins.
 */
#ifndef FUSE_PRIVATE_GPIO_H
#define FUSE_PRIVATE_GPIO_H

#include <fuse/fuse.h>
#include <stdint.h>

/** @brief GPIO context
 */
struct gpio_context
{
    uint8_t pin;
};

/** @brief Register value type for gpio pins
 */
void fuse_register_value_gpio(fuse_t *self);

#endif
