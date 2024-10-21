#include "watchdog.h"
#include "printf.h"
#include <hardware/watchdog.h>

///////////////////////////////////////////////////////////////////////////////
// GLOBALS

static fuse_watchdog_t *fuse_watchdog_instance = NULL;

///////////////////////////////////////////////////////////////////////////////
// LIFECYCLE

/** @brief Register type for Watchdog
 */
void fuse_register_value_watchdog(fuse_t *self)
{
    assert(self);

    fuse_value_desc_t fuse_watchdog_type = {
        .size = sizeof(struct watchdog_context),
        .name = "WATCHDOG",
        .init = fuse_watchdog_init,
        .destroy = fuse_watchdog_destroy,
        .str = fuse_watchdog_str,
    };
    fuse_register_value_type(self, FUSE_MAGIC_WATCHDOG, fuse_watchdog_type);
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE METHODS

/** @brief Initialize a watchdog interface
 */
static bool fuse_watchdog_init(fuse_t *self, fuse_value_t *value, const void *user_data)
{
    assert(self);
    assert(value);
    assert(user_data);

    // Check parameters
    fuse_watchdog_t *watchdog = (fuse_watchdog_t *)value;
    fuse_watchdog_config_t *data = (fuse_watchdog_config_t *)user_data;

    // Check for existing watchdog - only one allowed
    if (fuse_watchdog_instance)
    {
        fuse_debugf(self, "fuse_watchdog_init: Watchdog already initialized\n");
        return false;
    }

    // Set the period
    if (data->period_ms == 0 || data->period_ms > FUSE_WATCHDOG_MAX_PERIOD_MS)
    {
        data->period_ms = FUSE_WATCHDOG_MAX_PERIOD_MS;
    }
    if (data->period_ms < FUSE_WATCHDOG_MIN_PERIOD_MS)
    {
        data->period_ms = FUSE_WATCHDOG_MIN_PERIOD_MS;
    }

    // Create a timer - double the period
    watchdog->timer = fuse_timer_schedule(self, data->period_ms >> 1, true, watchdog);
    if (!watchdog->timer)
    {
        fuse_debugf(self, "fuse_watchdog_init: Unable to schedule timer\n");
        return false;
    }

    // Callback for timer event
    bool success = fuse_register_callback(self, FUSE_EVENT_TIMER, 0, fuse_watchdog_callback);
    assert(success);

    // Enable the watchdog, requiring the watchdog to be updated by callback
    // or the chip will reboot
    watchdog_enable(data->period_ms, true);

    // Set the watchdog instance
    fuse_watchdog_instance = watchdog;

    // Return true
    return true;
}

/** @brief Destroy the watchdog
 */
static void fuse_watchdog_destroy(fuse_t *self, fuse_value_t *value)
{
    assert(self);
    assert(value);
    fuse_watchdog_t *watchdog = (fuse_watchdog_t *)value;

    // Cancel the timer
    fuse_timer_cancel(self, watchdog->timer);

    // Remove instance
    fuse_watchdog_instance = NULL;
}

/** @brief Append a JSON representation of a watchdog
 */
static size_t fuse_watchdog_str(fuse_t *self, char *buf, size_t sz, size_t i, fuse_value_t *value, bool json)
{
    assert(self);
    assert(buf == NULL || sz > 0);
    assert(value);
    fuse_watchdog_t *watchdog = (fuse_watchdog_t *)value;

    // Add prefix
    i = chtostr_internal(buf, sz, i, '{');

    // TODO: Add period and caused_reboot

    // Add suffix
    i = chtostr_internal(buf, sz, i, '}');

    return i;
}

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

/** @brief Initialize a watchdog interface
 *
 * @param self The fuse application
 * @param data The watchdog configuration
 * @return The watchdog context, or NULL if it could not be initialized
 */
fuse_watchdog_t *fuse_new_watchdog_ex(fuse_t *self, fuse_watchdog_config_t data, const char *file, const int line)
{
    assert(self);

    // Register type
    if (!fuse_is_registered_value(self, FUSE_MAGIC_WATCHDOG))
    {
        if (watchdog_caused_reboot())
        {
            fuse_printf(self, "fuse_new_watchdog: watchdog caused reboot\n");
        }
        fuse_register_value_watchdog(self);
    }

    return (fuse_watchdog_t *)fuse_new_value_ex(self, FUSE_MAGIC_WATCHDOG, &data, file, line);
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE METHODS

/* @brief Timer callback for watchdog
 */
static void fuse_watchdog_callback(fuse_t *self, fuse_event_t *evt, void *user_data)
{
    assert(self);
    assert(evt);
    assert(user_data);

    if (user_data == fuse_watchdog_instance)
    {
        watchdog_update();
    }
}
