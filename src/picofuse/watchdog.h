/** @file watchdog.h
 *  @brief Private function prototypes and structure definitions for watchdog timer.
 */
#ifndef FUSE_PRIVATE_WATCHDOG_H
#define FUSE_PRIVATE_WATCHDOG_H
#include <picofuse/picofuse.h>

/** @brief Default watchdog period in milliseconds
 */
#define FUSE_WATCHDOG_MAX_PERIOD_MS 8388

/** @brief Minimum watchdog period in milliseconds
 */
#define FUSE_WATCHDOG_MIN_PERIOD_MS 100

/** @brief watchdog context
 */
struct watchdog_context
{
    fuse_timer_t* timer; ///< Timer source
};

///////////////////////////////////////////////////////////////////////////////
// DECLARATIONS

/** @brief Initialize a watchdog timer
 */
static bool fuse_watchdog_init(fuse_t *self, fuse_value_t *value, const void *user_data);

/** @brief Destroy the Watchdog timer
 */
static void fuse_watchdog_destroy(fuse_t *self, fuse_value_t *value);

/** @brief Append a string representation of a watchdog timer
 */
static size_t fuse_watchdog_str(fuse_t *self, char *buf, size_t sz, size_t i, fuse_value_t *v, bool json);

/* @brief Timer callback for watchdog
 */
static void fuse_watchdog_callback(fuse_t *self, fuse_event_t *evt, void *user_data);

#endif
