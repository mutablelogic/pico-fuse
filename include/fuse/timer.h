/** @file timer.h
 *  @brief Fuse timers
 *
 *  This file contains methods for creating timer events
 */
#ifndef FUSE_TIMER_H
#define FUSE_TIMER_H

#include "fuse.h"
#include <stdint.h>

/** @brief Create a new alarm
 *
 *  An alarm is created and added to the timer device
 *
 * @param self The fuse instance
 * @param timer The timer to add the alarm to
 * @param ms The number of milliseconds to wait to fire the timer
 * @param periodic If true, the timer will fire periodically rather than once
 * @param data The data to pass to the timer callback
 */
void fuse_timer_schedule(fuse_t *self, uint32_t ms, bool periodic, void *data);

#endif /* FUSE_TIMER_H */
