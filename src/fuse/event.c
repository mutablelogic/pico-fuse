#include <fuse/fuse.h>

// Private includes
#include "alloc.h"
#include "event.h"
#include "fuse.h"
#include "printf.h"

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
        .name = "EVT",
        .cstr = fuse_qstr_event,
        .qstr = fuse_qstr_event,
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
        if (fuse_list_push(self, self->core0, (fuse_value_t *)evt) == NULL)
        {
            return NULL;
        }
    }
    if (self->core1 != NULL)
    {
        if (fuse_list_push(self, self->core1, (fuse_value_t *)evt) == NULL)
        {
            return NULL;
        }
    }

    // Return the event
    return evt;
}

/** @brief Retrieve an event from the event queue
 */
fuse_event_t *fuse_next_event(fuse_t *self, uint8_t q)
{
    assert(self);
    assert(q < 2);

    // Get the event queue
    fuse_list_t *queue = (q == 0) ? self->core0 : self->core1;
    if (queue == NULL)
    {
        return NULL;
    }

    // Pop the event from the queue
    return (fuse_event_t *)fuse_list_pop(self, queue);
}

/** @brief Register a callback for an event
 */
bool fuse_register_callback(fuse_t *self, uint8_t type, uint8_t q, fuse_callback_t callback)
{
    assert(self);
    assert(type < FUSE_EVENT_COUNT);
    assert(q < 2);
    assert(callback);

    // Get the event callbacks
    struct event_callbacks callbacks;
    switch (q)
    {
    case 0:
        if (self->core0 == NULL)
        {
            return false;
        }
        callbacks = self->callbacks0[type];
        break;
    case 1:
        if (self->core1 == NULL)
        {
            return false;
        }
        callbacks = self->callbacks1[type];
        break;
    default:
        return false;
    }

    fuse_printf(self, "TODO: Registering callback for event %d on core %d\n", type, q);

    // Return success
    return true;
}

/** @brief Execute callbacks for an event
 */
void fuse_exec_event(fuse_t *self, uint8_t q, fuse_event_t *evt)
{
    assert(self);
    assert(q < 2);
    assert(evt);

    // Get the event callbacks
    struct event_callbacks callbacks;
    switch (q)
    {
    case 0:
        if (self->core0 == NULL)
        {
            return;
        }
        callbacks = self->callbacks0[evt->type];
        break;
    case 1:
        if (self->core1 == NULL)
        {
            return;
        }
        callbacks = self->callbacks1[evt->type];
        break;
    default:
        return;
    }

    fuse_printf(self, "TODO: Executing callbacks for event %d on core %d\n", evt->type, q);
}

//////////////////////////////////////////////////////////////////////////////
// PRIVATE METHODS

/** @brief Append a quoted string representation of an event
 */
static inline size_t fuse_qstr_event_type(char *buf, size_t sz, size_t i, uint8_t type)
{
#ifdef DEBUG
    switch (type)
    {
    case FUSE_EVENT_NULL:
        return qstrtostr_internal(buf, sz, i, "NULL");
    case FUSE_EVENT_TIMER:
        return qstrtostr_internal(buf, sz, i, "TIMER");
    default:
        assert(false);
        return i;
    }
#else
    return uitostr_internal(buf, sz, i, type, 0);
#endif
}

/** @brief Append a quoted string representation of an event
 */
size_t fuse_qstr_event(fuse_t *self, char *buf, size_t sz, size_t i, fuse_value_t *v)
{
    assert(self);
    assert(buf == NULL || sz > 0);
    assert(v);
    assert(fuse_allocator_magic(self->allocator, v) == FUSE_MAGIC_EVENT);

    // Get the event properties
    struct event_context *evt = (struct event_context *)v;
    assert(evt);

    // Add prefix
    i = chtostr_internal(buf, sz, i, '{');

    // Add source
    i = qstrtostr_internal(buf, sz, i, "source");
    i = chtostr_internal(buf, sz, i, ':');
    i = vtostr_internal(self, buf, sz, i, evt->source, true);

    // Add type
    i = chtostr_internal(buf, sz, i, ',');
    i = qstrtostr_internal(buf, sz, i, "type");
    i = chtostr_internal(buf, sz, i, ':');
    i = fuse_qstr_event_type(buf, sz, i, evt->type);

    // Add user data
    if (evt->user_data)
    {
        i = chtostr_internal(buf, sz, i, ',');
        i = qstrtostr_internal(buf, sz, i, "user_data");
        i = chtostr_internal(buf, sz, i, ':');
        i = ptostr_internal(buf, sz, i, evt->user_data);
    }

    // Add suffix
    i = chtostr_internal(buf, sz, i, '}');

    // Return the index
    return i;
}
