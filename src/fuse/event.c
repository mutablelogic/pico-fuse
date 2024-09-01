#include <fuse/fuse.h>

// Private includes
#include "fuse.h"
#include "event.h"

///////////////////////////////////////////////////////////////////////////////
// LIFECYCLE

/** @brief Register value primitive for a mutex
 */
void fuse_register_value_event(fuse_t *self)
{
    assert(self);

    // Register mutex type
    fuse_value_desc_t fuse_event_type = {
        .size = sizeof(struct event_context),
        .name = "EVT"
    };
    fuse_register_value_type(self, FUSE_MAGIC_EVENT, fuse_event_type);
}

//////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

/** @brief Place a new event on the event queues
 */
fuse_event_t *fuse_new_event_ex(fuse_t *self, fuse_value_t *source, uint8_t type, void *user_data, const char *file, const int line)
{
    assert(self);
    assert(source);
    assert(type < FUSE_EVENT_COUNT);

    // Create a new event
    fuse_event_t *evt = (fuse_event_t *)fuse_new_value_ex(self, FUSE_MAGIC_EVENT, 0, file, line);
    if (evt == NULL)
    {
        return NULL;
    }

    // Set the event properties
    evt->type = type;
    evt->source = source;
    evt->user_data = user_data;

    // Place the event on the event queues
    if (self->core0 != NULL)
    {
        if (fuse_list_push(self, self->core0, (fuse_value_t*)evt) == NULL) {
            return NULL;
        }
    }
    if (self->core1 != NULL)
    {
        if (fuse_list_push(self, self->core1,  (fuse_value_t*)evt) == NULL) {
            return NULL;
        }
    }

    // Return the event
    return evt;
}

/** @brief Retrieve an event from the event queue
 */
fuse_event_t *fuse_next_event(fuse_t *self, uint8_t q) {
    assert(self);
    assert(q < 2);

    // Get the event queue
    fuse_list_t *queue = (q == 0) ? self->core0 : self->core1;
    if (queue == NULL) {
        return NULL;
    }

    // Pop the event from the queue
    return (fuse_event_t *)fuse_list_pop(self, queue);
}

