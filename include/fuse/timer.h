/** @file timer.h
 *  @brief Fuse timers
 *
 *  This file contains methods for creating and releasing timers which are either one-shot
 *  or periodic.
 */
#ifndef FUSE_TIMER_H
#define FUSE_TIMER_H

#include "fuse.h"
#include <stdint.h>

/** @brief Create a new timer
 * 
 *  A timer is created and added to the event loop. If the timer fires once, it is removed
 *  from the event loop after it has been fired. If the timer is periodic, it will continue to fire until
 *  it is removed from the event loop (TODO).
 *
 * @param self The fuse instance
 * @param ms The number of milliseconds to fire the timer
 * @param periodic If true, the timer will fire periodically rather than once
 * @param data The data to pass to the timer callback
 * @param file The file name of the caller
 * @param line The line number of the caller
 * @return The timer value or NULL if the timer could not be created
 */
fuse_value_t *fuse_timer_new_ex(fuse_t *self, uint32_t ms, bool periodic, void* data, const char *file, int line);

#endif /* FUSE_TIMER_H */
