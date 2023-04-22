#ifndef PIMORONI_INKY_H
#define PIMORONI_INKY_H

#include <fuse/fuse.h>

/*
 * Events - button A/B/C pressed or released
 */
#define EV_INKY_A (fuse_event_t)(0x1001) // GPIO12
#define EV_INKY_B (fuse_event_t)(0x1002) // GPIO13
#define EV_INKY_C (fuse_event_t)(0x1003) // GPIO14

/*
 * Define the GPIO driver
 */
extern fuse_driver_params_t pimoroni_inkypack;

#endif
