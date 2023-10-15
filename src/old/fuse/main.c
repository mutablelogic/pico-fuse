#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <pico/stdlib.h>

#include <fuse/fuse.h>
#include "main.h"
#include "action.h"

#define FUSE_ACTION_HASHMAP_SIZE 64
#define FUSE_EVENT_HASHMAP_SIZE 64

///////////////////////////////////////////////////////////////////////////////
// PRIVATE METHODS

/*
 * Register an event and associate it with a driver
 */
static bool fuse_register_event(fuse_t *fuse, const char *name, fuse_driver_t *driver, const fuse_driver_event_t *event)
{
    assert(fuse);
    assert(fuse->events);
    assert(name);
    assert(strlen(name));
    assert(event->event);
    assert(event->name);
    assert(strlen(event->name));

    // Check if the event is already registered
    fuse_driver_event_t *existing = hashmap_get_voidp(fuse->events, (void *)event->event);
    if (existing != NULL)
    {
        fuse_debugf(fuse, "fuse_register_event: %s: event %d (%s) already registered as %s\n", name, event->event, event->name, existing->name);
        return false;
    }

    // Allocate a new event
    fuse_driver_event_t *new = fuse_malloc(fuse, sizeof(fuse_driver_event_t));
    if (new == NULL)
    {
        fuse_debugf(fuse, "fuse_register_event: %s: failed to allocate memory for %s\n", name, event->name);
        return false;
    }

    // Set parameters
    new->event = event->event;
    new->name = event->name;
    new->finalizer = event->finalizer;
    new->driver = driver;

    // Add the event to the hashmap
    if (!hashmap_put_voidp(fuse->events, (void *)event->event, new))
    {
        fuse_debugf(fuse, "fuse_register_event: %s: failed to add %s to hashmap\n", name, event->name);
        return false;
    }

    // Registered an event
    fuse_debugf(fuse, "fuse_register_event: %s: registered event %s with finalizer %08X\n", name, new->name, new->finalizer);

    // Return success
    return true;
}

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

/*
 * Create a new fuse application
 */
fuse_t *fuse_init(size_t size, fuse_flag_t flags)
{
    assert(size > 0);

    // Initialize stdio
    stdio_init_all();
    sleep_ms(1000);

    // Create a memory pool
    fuse_pool_t *pool = fuse_pool_new(size);
    if (pool == NULL)
    {
        return NULL;
    }

    // Allocate instance
    fuse_t *self = (fuse_t *)fuse_pool_alloc(pool, sizeof(fuse_t));
    if (self == NULL)
    {
        fuse_pool_delete(pool);
        return NULL;
    }

    // Set the instance properties
    self->pool = pool;
    self->flags = flags;
    self->status = 0;
    self->queue = fuse_queue_new(pool);
    if (self->queue == NULL)
    {
        fuse_pool_delete(pool);
        return NULL;
    }

    // Create actions hashmap
    self->actions = hashmap_new(self, FUSE_ACTION_HASHMAP_SIZE);
    if (self->actions == NULL)
    {
        fuse_queue_zero(self->queue);
        fuse_pool_delete(pool);
        return NULL;
    }

    // Create events hashmap
    self->events = hashmap_new(self, FUSE_EVENT_HASHMAP_SIZE);
    if (self->events == NULL)
    {
        hashmap_delete(self->actions);
        fuse_queue_zero(self->queue);
        fuse_pool_delete(pool);
        return NULL;
    }

    // Register the EV_INIT event
    const fuse_driver_event_t event = {EV_INIT, "EV_INIT"};
    if (!fuse_register_event(self, "fuse", NULL, &event))
    {
        fuse_debugf(self, "fuse_init: failed to register EV_INIT event\n");
        hashmap_delete(self->events);
        hashmap_delete(self->actions);
        fuse_queue_zero(self->queue);
        fuse_pool_delete(pool);
        return NULL;
    }

    // Fire an EV_INIT event to get things started
    if (!fuse_queue_add(self->queue, EV_INIT, NULL))
    {
        fuse_debugf(self, "fuse_init: failed to add EV_INIT event to queue");
        hashmap_delete(self->events);
        hashmap_delete(self->actions);
        fuse_queue_zero(self->queue);
        fuse_pool_delete(pool);
        return NULL;
    }

    // Return success
    return self;
}

/*
 * Free resources from a fuse application, and stop the application
 */
int fuse_exit(fuse_t *fuse)
{
    assert(fuse);
    assert(fuse->pool);

    // Set the return code
    int status = fuse->status;

    // Clear resources
    fuse_pool_delete(fuse->pool);

    // Return the return code from the application
    return status;
}

/*
 * Check for existence of fuse flags
 */
inline bool fuse_is(fuse_t *fuse, fuse_flag_t flag)
{
    assert(fuse);
    return (fuse->flags & flag) == flag;
}

/*
 * fuse_malloc allocates a block of memory from the pool
 */
inline void *fuse_malloc(fuse_t *fuse, size_t size)
{
    assert(fuse);
    assert(fuse->pool);
    void *data = fuse_pool_alloc(fuse->pool, size);
    if (data == NULL)
    {
        fuse_debugf(fuse, "fuse_malloc: failed to allocate %d bytes\n", size);
    }
    return data;
}

/*
 * Register a driver with the fuse application
 */
fuse_driver_t *fuse_register_driver(fuse_t *fuse, const fuse_driver_params_t *params, void *user_data)
{
    assert(fuse);
    assert(params->init);
    assert(params->name);
    assert(strlen(params->name));

    // Call the driver's init function
    fuse_driver_t *instance = params->init(fuse, user_data);
    if (instance == NULL)
    {
        return NULL;
    }

    // Register event handlers
    const fuse_driver_event_t *event = params->events;
    while (event && event->event && event->name)
    {
        if (!fuse_register_event(fuse, params->name, instance, event))
        {
            fuse_debugf(fuse, "fuse_register_driver: %s: failed to register event %d (%s)\n", params->name, event->event, event->name);
            return NULL;
        }
        event++;
    }

    return instance;
}

/*
 * Fire an event
 */
inline bool fuse_event_fire(fuse_t *fuse, fuse_event_t event, void *data)
{
    assert(fuse);
    assert(fuse->queue);
    assert(event);

    return fuse_queue_add(fuse->queue, event, data);
}

/*
 * Fire an event with a boolean argument
 *
 * @param fuse             The fuse application
 * @param driver           The driver associated with the event
 * @param event            The event that has been fired
 * @param data             The boolean data associated with the event
 */
inline bool fuse_event_fire_bool(fuse_t *fuse, fuse_event_t event, bool data)
{
    assert(fuse);
    assert(fuse->queue);
    assert(event);

    return fuse_queue_add(fuse->queue, event, (void *)data);
}

/*
 * Return the name of an event
 */
const char *fuse_event_name(fuse_t *fuse, fuse_event_t event)
{
    assert(fuse);
    assert(fuse->events);

    // Retrieve the existing event
    fuse_driver_event_t *existing = hashmap_get_voidp(fuse->events, (void *)event);
    if (existing == NULL)
    {
        return NULL;
    }

    // Return the event name
    return existing->name;
}

/*
 * Poll WiFi driver - returns false if the queue is empty and nothing more to do
 */
bool fuse_poll(fuse_t *fuse, int delay_ms)
{
    assert(fuse);
    assert(fuse->queue);

#if PICO_CYW43_ARCH_POLL
    cyw43_arch_poll();
#endif
    if (fuse_queue_isempty(fuse->queue))
    {
#if PICO_CYW43_ARCH_POLL
        cyw43_arch_wait_for_work_until(make_timeout_time_ms(delay_ms));
#else
        sleep_ms(delay_ms);
#endif
        return false;
    }
    return true;
}

/*
 * Process events until the application is stopped
 */
void fuse_main(fuse_t *fuse)
{
    assert(fuse);
    assert(fuse->queue);

    // Debug
    fuse_debugf(fuse, "fuse_main: Starting run loop\n");

    // Continue until the application is stopped
    int i = 0;
    void *data;
    while (fuse->status == 0)
    {
        // Poll for events, and pause for 10ms
        // If nothing to do, then try again
        if (!fuse_poll(fuse, 10))
        {
            continue;
        }

        // Get next event from the queue
        fuse_event_t event = fuse_queue_next(fuse->queue, &data);
        if (event == 0)
        {
            continue;
        }

        fuse_debugf(fuse, "%04d Processing event: %s\n", i++, fuse_event_name(fuse, event));
        fuse_call_action(fuse, event, data);
    }
}
