#if defined(TARGET_LINUX)

// Public includes
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <fuse/fuse.h>

// Private includes
#include "fuse.h"
#include "timer.h"

///////////////////////////////////////////////////////////////////////////////
// DEFINITIONS

static bool timer_init(fuse_t *self, fuse_value_t *value, const void *user_data);
static void timer_destroy(fuse_t *self, fuse_value_t *value);
static void fuse_timer_callback(union sigval timer_data);

struct timer_context
{
    timer_t timer;
    struct fuse_application *self;
    const void* data;
};

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
    };
    fuse_register_value_type(self, FUSE_MAGIC_TIMER, fuse_timer_type);
}

/** @brief Create a new timer context
 */
static bool timer_init(fuse_t *self, fuse_value_t *value, const void *user_data)
{
    assert(self);
    assert(value);

    // Initialize the timer context
    struct timer_context* ctx = (struct timer_context*)value;
    ctx->self = self;
    ctx->data = user_data;    

    // Create the timer
    struct sigevent event = {
        .sigev_notify = SIGEV_THREAD,
        .sigev_value.sival_ptr = value,
        .sigev_notify_function = fuse_timer_callback,
    };
    if (timer_create(CLOCK_MONOTONIC, &event, &ctx->timer) != 0) {
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
    timer_delete(((struct timer_context*)value)->timer);
}

///////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS

/** @brief Schedule a new timer event
 */
fuse_timer_t* fuse_timer_schedule(fuse_t *self, uint32_t ms, bool periodic, void *user_data)
{
    assert(self);
    assert(ms > 0);

    // Create the timer and retain it
    fuse_timer_t* timer = (fuse_timer_t* )fuse_retain(self, fuse_new_timer(self, user_data));
    if (timer == NULL)
    {
        return NULL;
    }

    // Schedule the timer
    struct itimerspec timer_period = {
        .it_value.tv_sec = ms / 1000,
        .it_value.tv_nsec = (ms % 1000) * 1000000,
        .it_interval.tv_sec = periodic ? ms / 1000 : 0,
        .it_interval.tv_nsec = periodic ? (ms % 1000) * 1000000 : 0,
    };
    timer_settime(timer->timer, 0, &timer_period, NULL);

    // Return the timer
    return timer;
}

/** @brief Cancel a timer
 */
void fuse_timer_cancel(fuse_t *self, fuse_timer_t* timer) {
    assert(self);
    assert(timer);

    // Release the timer
    fuse_release(self, (fuse_value_t* )timer);
}

static void fuse_timer_callback(union sigval timer_data)
{
    struct timer_context* ctx = (struct timer_context*)(timer_data.sival_ptr);
    fuse_debugf("fuse_timer_callback: fuse=%p data=%p\n",ctx->self,ctx->data);
}

#endif // TARGET_LINUX
