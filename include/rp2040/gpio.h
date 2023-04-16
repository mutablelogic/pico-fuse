#ifndef RP2040_GPIO_H
#define RP2040_GPIO_H

#include <fuse/fuse.h>

/*
 * Events
 */
#define EV_GPIO_INIT (fuse_event_t)(0x0101)
#define EV_GPIO (fuse_event_t)(0x0102)
#define EV_GPIO_PWM_INIT (fuse_event_t)(0x0103)

/*
 * GPIO functions
 */
typedef enum
{
    GPIO_NONE,
    GPIO_INPUT,
    GPIO_OUTPUT,
    GPIO_PWM,
} gpio_func_t;

/*
 * GPIO configuration, which is used to initialize a GPIO pin
 * with the EV_GPIO_INIT event
 */
typedef struct
{
    // The GPIO number
    int gpio;

    // The GPIO function
    gpio_func_t func;

    // Enable pullup
    bool pullup;

    // Enable pulldown (setting both pulls enables a "bus keep" function)
    bool pulldown;

    // Enable event on GPIO_IRQ_LEVEL_RISE
    bool irqrise;

    // Enable event on GPIO_IRQ_LEVEL_FALL
    bool irqfall;
} gpio_pin_t;

/*
 * Define the GPIO driver
 */
extern fuse_driver_params_t rp2040_gpio;

#endif
