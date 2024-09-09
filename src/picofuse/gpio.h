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


/** @brief Initialize a GPIO pin with a function
 *
 *  @param pin The GPIO pin number
 *  @param func The function of the GPIO pin
 *  @return The GPIO pin, or NULL if the GPIO pin could not be initialized, or was already initialized
 */
fuse_gpio_t *fuse_new_gpio_ex(fuse_t *self, uint8_t pin, fuse_gpio_func_t func, const char *file, const int line);

#endif
