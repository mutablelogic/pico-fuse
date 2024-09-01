#if defined(TARGET_DARWIN)
#include <dispatch/dispatch.h>
#include <fuse/fuse.h>

// Private includes
#include "alloc.h"
#include "fuse.h"
#include "printf.h"
#include "timer.h"

///////////////////////////////////////////////////////////////////////////////
// DEFINITIONS

static bool timer_init(fuse_t *self, fuse_value_t *value, const void *user_data);
static void timer_destroy(fuse_t *self, fuse_value_t *value);
static void fuse_timer_callback(fuse_timer_t *timer);

struct timer_context
{
    dispatch_source_t timer;
    struct fuse_application *self;
    const void *data;
};

// Custom dispatch queue for background execution
static dispatch_queue_t queue;

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
        .cstr = fuse_qstr_timer,
        .qstr = fuse_qstr_timer
    };
    fuse_register_value_type(self, FUSE_MAGIC_TIMER, fuse_timer_type);

    // Create a custom dispatch queue for background execution
    queue = dispatch_queue_create("com.mutablelogic.picofuse", DISPATCH_QUEUE_CONCURRENT);
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

    // Create the timer
    ctx->timer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, queue);
    if (!ctx->timer)
    {
        return false;
    }

    // Return success
    return true;
}

static void timer_destroy(fuse_t *self, fuse_value_t *value)
{
    assert(self);
    assert(value);

    // Delete the timer
    dispatch_source_cancel(((struct timer_context *)value)->timer);
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

    // Initialize the timer
    uint64_t interval_nsec = (uint64_t)(ms) * 1000000; // 1 millisecond = 1E6 nanoseconds
    if (periodic)
    {
        dispatch_source_set_timer(timer->timer, dispatch_time(DISPATCH_TIME_NOW, interval_nsec), interval_nsec, 0);
    }
    else
    {
        dispatch_source_set_timer(timer->timer, dispatch_time(DISPATCH_TIME_NOW, interval_nsec), DISPATCH_TIME_FOREVER, 0);
    }

    // Set the event handler
    dispatch_source_set_event_handler(timer->timer, ^{
      fuse_timer_callback(timer);
      if (!periodic)
      {
          dispatch_source_cancel(timer->timer);
      }
    });

    // Start the timer
    dispatch_resume(timer->timer);

    // Return the timer
    return timer;
}

/** @brief Cancel a timer
 */
void fuse_timer_cancel(fuse_t *self, fuse_timer_t *timer)
{
    assert(self);
    assert(timer);

    // Release the timer
    fuse_release(self, (fuse_value_t *)timer);
}

/** @brief Place a timer event onto the event queue
 */
static void fuse_timer_callback(fuse_timer_t *timer)
{
    fuse_event_t* evt = fuse_new_event(timer->self, (fuse_value_t* )timer, FUSE_EVENT_TIMER, (void* )timer->data);
    assert(evt);
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS

/** @brief Append a quoted string representation of a timer
 */
size_t fuse_qstr_timer(fuse_t *self, char *buf, size_t sz, size_t i, fuse_value_t *v) {
    assert(self);
    assert(buf == NULL || sz > 0);
    assert(v);
    assert(fuse_allocator_magic(self->allocator, v) == FUSE_MAGIC_TIMER);

    // Get the event properties
    struct timer_context *timer = (struct timer_context *)v;

    // Add prefix
    i = chtoa_internal(buf, sz, i, '{');

    // Add user data
    if(timer->data) {
        i = qstrtoa_internal(buf, sz, i, "data");
        i = chtoa_internal(buf, sz, i, ':');
        i = ptoa_internal(buf, sz, i, (void* )timer->data);
    }

    // Add suffix
    i = chtoa_internal(buf, sz, i, '}');

    // Return the index
    return i;
}

#endif // TARGET_DARWIN
