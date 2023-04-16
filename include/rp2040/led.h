#ifndef RP2040_LED_H
#define RP2040_LED_H

#include <fuse/fuse.h>

/*
 * Events
 */
#define EV_LED (fuse_event_t)(0x0201)

/*
 * Define the GPIO driver
 */
extern fuse_driver_params_t rp2040_led;

#endif
