
// Public includes
#include <fuse/fuse.h>

// Private includes
#include "fuse.h"

///////////////////////////////////////////////////////////////////////////////
// DEFINITIONS

static fuse_context_t *timer_new(fuse_t *fuse, void *data);
static void timer_destroy(fuse_t *fuse, fuse_context_t *context);

struct timer_context
{
};

////////////////////////////////////////////////////////////////////a///////////
// GLOBALS

fuse_device_t fuse_timer = {
    .name = "timer",
    .init = timer_new,
    .destroy = timer_destroy,
};

///////////////////////////////////////////////////////////////////////////////
// LIFECYCLE

/** @brief Create a new timer context
 */
static fuse_context_t *timer_new(fuse_t *self, void *data)
{
    assert(self);

    // Register the timer type
    if (self->desc[FUSE_MAGIC_TIMER].size == 0)
    {
        self->desc[FUSE_MAGIC_TIMER] = (struct fuse_value_desc){
            .size = sizeof(struct timer_context),
            .name = "TIMER",
        };
    }

    // Allocate the context
    return fuse_alloc_ex(self, FUSE_MAGIC_TIMER, data, NULL, 0);
}

static void timer_destroy(fuse_t *fuse, fuse_context_t *context)
{
    assert(fuse);
    assert(context);

    mqtt_client_free(((struct mqtt_context *)context)->client);
    fuse_free(fuse, context);
}

///////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS

/** @brief Schedule a new timer event
 */
void fuse_timer_schedule(fuse_t *self, fuse_context_t *timer, uint32_t ms, bool periodic, void *data)
{
    assert(self);
    assert(timer);
    assert(ms > 0);
}
