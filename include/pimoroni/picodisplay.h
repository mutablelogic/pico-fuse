#ifndef PIMORONI_PICODISPLAY_H
#define PIMORONI_PICODISPLAY_H

#include <fuse/fuse.h>

/*
 * Events - button A/B/C pressed or released
 */
#define EV_PICODISPLAY_A (fuse_event_t)(0x1101) // GPIO16
#define EV_PICODISPLAY_B (fuse_event_t)(0x1102) // GPIO17
#define EV_PICODISPLAY_X (fuse_event_t)(0x1103) // GPIO19
#define EV_PICODISPLAY_Y (fuse_event_t)(0x1104) // GPIO20

/*
 * Define the GPIO driver
 */
extern fuse_driver_params_t pimoroni_picodisplay;

#endif
