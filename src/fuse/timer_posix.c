#ifndef TARGET_PICO

// Public includes
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <fuse/fuse.h>

// Private includes
#include "fuse.h"

///////////////////////////////////////////////////////////////////////////////
// DEFINITIONS

static bool timer_init(fuse_t *self, fuse_value_t *value, const void *user_data);
static void timer_destroy(fuse_t *self, fuse_value_t *value);
static void fuse_timer_callback(union sigval timer_data);

struct timer_value
{
};

////////////////////////////////////////////////////////////////////a///////////
// GLOBALS

size_t size;
const char *name;
bool (*init)(struct fuse_application *self, fuse_value_t *value, const void *user_data);
void (*destroy)(struct fuse_application *self, fuse_value_t *value);
size_t (*cstr)(struct fuse_application *self, char *buf, size_t sz, size_t i, fuse_value_t *v);
size_t (*qstr)(struct fuse_application *self, char *buf, size_t sz, size_t i, fuse_value_t *v);

fuse_value_desc_t fuse_timer = {
    .size = sizeof(struct timer_value),
    .name = "TIMER",
    .init = timer_init,
    .destroy = timer_destroy,
};

///////////////////////////////////////////////////////////////////////////////
// LIFECYCLE

/** @brief Register value primitive for a timer context
 *
 * @param self The fuse instance
 */
void fuse_register_value_timer(fuse_t *self)
{
    assert(self);

    // Register timer type
    fuse_register_value_type(self, FUSE_MAGIC_TIMER, fuse_timer);
}

/** @brief Create a new timer context
 */
static bool timer_init(fuse_t *self, fuse_value_t *value, const void *user_data)
{
    assert(self);
    fuse_printf(self, "timer_init: Creating a new timer context\n");
    return true;
}

static void timer_destroy(fuse_t *self, fuse_value_t *value)
{
    assert(self);
    assert(value);
    fuse_printf(self, "timer_destroy: Destroy timer context\n");
}

///////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS

/** @brief Schedule a new timer event
 */
void fuse_timer_schedule(fuse_t *self, uint32_t ms, bool periodic, void *user_data)
{
    assert(self);
    assert(ms > 0);

    // Create the timer
    timer_t timer;
    struct sigevent event = {
        .sigev_notify = SIGEV_THREAD,
        .sigev_value.sival_ptr = data,
        .sigev_notify_function = fuse_timer_callback,
    };
    timer_create(CLOCK_MONOTONIC, &event, &timer);

    // Start the timer
    struct itimerspec timer_period = {
        .it_value.tv_sec = ms / 1000,
        .it_value.tv_nsec = (ms % 1000) * 1000000,
        .it_interval.tv_sec = periodic ? ms / 1000 : 0,
        .it_interval.tv_nsec = periodic ? (ms % 1000) * 1000000 : 0,
    };
    timer_settime(timer, 0, &timer_period, NULL);
}

static void fuse_timer_callback(union sigval timer_data)
{
    printf("fuse_timer_callback: %p\n", timer_data.sival_ptr);
}

#endif // TARGET_PICO