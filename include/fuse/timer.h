/** @file timer.h
 *  @brief Fuse timers
 *
 *  This file contains methods for creating timer events
 */
#ifndef FUSE_TIMER_H
#define FUSE_TIMER_H

#include "fuse.h"
#include <stdint.h>

/** @brief Timer context
 */
typedef struct timer_context fuse_timer_t;

/** @brief Create a new single-shot or periodic timer
 *
 *  A timer is created and retained by the fuse application until cancelled
 *
 * @param self The fuse instance
 * @param timer The timer to add the alarm to
 * @param ms The timer interval, in number of milliseconds
 * @param periodic If true, the timer will fire periodically rather than once
 * @param data The data to pass to the timer callback
 * @return The timer
 */
fuse_timer_t* fuse_timer_schedule(fuse_t *self, uint32_t ms, bool periodic, void *user_data);

/** @brief Cancel a timer
 *
 * A timer is cancelled, and the timer value is released
 *
 * @param self The fuse instance
 * @param timer The timer to cancel
 */
void fuse_timer_cancel(fuse_t *self, fuse_timer_t* timer);

#endif /* FUSE_TIMER_H */
