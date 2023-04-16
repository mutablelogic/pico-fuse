#ifndef RP2040_TIMER_H
#define RP2040_TIMER_H

#include <fuse/fuse.h>
#include <stdint.h>

/*
 * Events
 */
#define EV_TIMER_INIT (fuse_event_t)(0x0401)
#define EV_TIMER (fuse_event_t)(0x0402)

/*
 * Define the GPIO driver
 */
extern fuse_driver_params_t rp2040_timer;

/*
 * Timer configuration, which is used to initialize a timer
 */
typedef struct
{
    int id;           // Timer ID
    int32_t delay_ms; // Delay in milliseconds
    bool repeat;      // Periodic timer (set to false to cancel the timer)
} fuse_timer_t;

#endif
