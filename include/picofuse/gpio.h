/** @file gpio.h
 *  @brief Function prototypes which control the GPIO pins
 */
#ifndef PICOFUSE_GPIO_H
#define PICOFUSE_GPIO_H

#include <stdint.h>

/** @brief Defines the function of a GPIO pin
 */
typedef enum
{
    FUSE_GPIO_IN,     ///< The GPIO pin is an input which is floating
    FUSE_GPIO_PULLUP, ///< The GPIO pin is an input with a pull-up resistor
    FUSE_GPIO_PULLDN, ///< The GPIO pin is an input with a pull-down resistor
    FUSE_GPIO_OUT,    ///< The GPIO pin is an output
    FUSE_GPIO_PWM,    ///< The GPIO pin is used for PWM
    FUSE_GPIO_SPI,    ///< The GPIO pin is used for SPI communication
    FUSE_GPIO_ADC,    ///< The GPIO pin is used for ADC
    FUSE_GPIO_I2C,    ///< The GPIO pin is used for I2C communication
    FUSE_GPIO_UART,   ///< The GPIO pin is used for UART communication
} picofuse_func_t;

/** @brief Return the number of GPIO pins
 *
 *  The pins are referenced from index 0, so the maximum pin argument is
 *  one less than the count.
 *
 *  @return Returns the pin count
 */
uint8_t fuse_gpio_count();

/** @brief Initialize a GPIO pin with a function
 *
 *  @param pin The GPIO pin number
 *  @param func The function of the GPIO pin
 */
void fuse_gpio_new(uint8_t pin, picofuse_func_t func);

/** @brief Reset a GPIO pin to its default state
 *
 *  @param pin The GPIO pin number
 */
void fuse_gpio_destroy(uint8_t pin);

/** @brief Set the GPIO pin to be low or high
 *
 *  @param pin The GPIO pin number
 *  @param value The binary value of the GPIO pin
 */
void fuse_gpio_set(uint8_t pin, bool value);

/** @brief Get the binary value of a GPIO pin
 *
 *  @param pin The GPIO pin number
 *  @return The binary value of the GPIO pin, assuming the pin is an input or output pin
 */
bool fuse_gpio_get(uint8_t pin);

#endif
