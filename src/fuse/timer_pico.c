#if defined(TARGET_PICO)
#include <fuse/fuse.h>
#include <pico/time.h>
#include "alloc.h"
#include "fuse.h"
#include "printf.h"
#include "timer.h"

///////////////////////////////////////////////////////////////////////////////
// DEFINITIONS

static bool timer_init(fuse_t *self, fuse_value_t *value, const void *user_data);
static void timer_destroy(fuse_t *self, fuse_value_t *value);
static bool fuse_timer_callback(repeating_timer_t *rt);

struct timer_context
{
    bool periodic;
    repeating_timer_t alarm;
    struct fuse_application *self;
    const void *data;
};

static alarm_pool_t *pool = NULL;

///////////////////////////////////////////////////////////////////////////////
// LIFECYCLE

/** @brief Register value primitive for a timer context
 */
void fuse_register_value_timer(fuse_t *self)
{
    assert(self);

    // Register timer type
    fuse_value_desc_t fuse_timer_type = {
        .size = sizeof(struct timer_context),
        .name = "TIME",
        .init = timer_init,
        .destroy = timer_destroy,
        .str = fuse_str_timer,
    };
    fuse_register_value_type(self, FUSE_MAGIC_TIMER, fuse_timer_type);

    // Get alarm pool
    pool = alarm_pool_get_default();
    assert(pool);
}

/** @brief Create a new timer context
 */
static bool timer_init(fuse_t *self, fuse_value_t *value, const void *user_data)
{
    assert(self);
    assert(value);

    // Initialize the timer context
    struct timer_context *ctx = (struct timer_context *)value;
    ctx->self = self;
    ctx->data = user_data;

    // Return success
    return true;
}

static void timer_destroy(fuse_t *self, fuse_value_t *value)
{
    assert(self);
    assert(value);

    // Cancel the timer
    struct timer_context *timer = (struct timer_context *)value;
    cancel_repeating_timer(&timer->alarm);
}

///////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS

/** @brief Schedule a new timer event
 */
fuse_timer_t *fuse_timer_schedule(fuse_t *self, uint32_t ms, bool periodic, void *user_data)
{
    assert(self);
    assert(ms > 0);

    // Create the timer and retain it
    fuse_timer_t *timer = (fuse_timer_t *)fuse_retain(self, fuse_new_timer(self, user_data));
    if (timer == NULL)
    {
        return NULL;
    }

    // Schedule the timer
    timer->periodic = periodic;
    if (!alarm_pool_add_repeating_timer_ms(pool, ms, fuse_timer_callback, timer, &timer->alarm))
    {
        fuse_release(self, (fuse_value_t *)timer);
        return NULL;
    }

    // Return the timer
    return timer;
}

/** @brief Cancel a timer
 */
void fuse_timer_cancel(fuse_t *self, fuse_timer_t *timer)
{
    assert(self);
    assert(timer);

    // Cancel the timer
    cancel_repeating_timer(&timer->alarm);

    // Release the timer
    fuse_release(self, (fuse_value_t *)timer);
}

/** @brief Place a timer event onto the event queue
 */
static bool fuse_timer_callback(repeating_timer_t *rt)
{
    struct timer_context *timer = (struct timer_context *)(rt->user_data);
    assert(timer);
    assert(timer->self);
    fuse_event_t* evt = fuse_new_event(timer->self, (fuse_value_t* )timer, FUSE_EVENT_TIMER, (void* )timer->data);
    assert(evt);
    return timer->periodic;
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS

/** @brief Append a quoted string representation of a timer
 */
size_t fuse_str_timer(fuse_t *self, char *buf, size_t sz, size_t i, fuse_value_t *v, bool json)
{
    assert(self);
    assert(buf == NULL || sz > 0);
    assert(v);
    assert(fuse_allocator_magic(self->allocator, v) == FUSE_MAGIC_TIMER);

    // Get the event properties
    struct timer_context *timer = (struct timer_context *)v;

    // Add prefix
    i = chtostr_internal(buf, sz, i, '{');

    // Add user data
    if (timer->data)
    {
        i = qstrtostr_internal(buf, sz, i, "data");
        i = chtostr_internal(buf, sz, i, ':');
        i = ptostr_internal(buf, sz, i, (void *)timer->data);
    }

    // Add suffix
    i = chtostr_internal(buf, sz, i, '}');

    // Return the index
    return i;
}

#endif // TARGET_PICO
