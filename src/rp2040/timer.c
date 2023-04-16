#include <stdlib.h>
#include <pico/stdlib.h>
#include <pico/time.h>
#include <pico/util/pheap.h>
#include <fuse/fuse.h>
#include <rp2040/timer.h>

///////////////////////////////////////////////////////////////////////////////

typedef struct
{
    alarm_pool_t *alarm_pool;
} timer_t;

struct fuse_timer_instance
{
    fuse_t *fuse;
    fuse_timer_t *timer;
};

///////////////////////////////////////////////////////////////////////////////
// PRIVATE METHODS

bool rp2040_timer_repeating_fired(repeating_timer_t *timer)
{
    struct fuse_timer_instance *state = (struct fuse_timer_instance *)timer->user_data;
    assert(state);
    assert(state->fuse);
    assert(state->timer);

    // Call the registered event handler when periodic, and reschedule
    if (state->timer->repeat)
    {
        if (!fuse_event_fire(state->fuse, EV_TIMER, state->timer))
        {
            fuse_debugf(state->fuse, "rp2040_timer_repeating_fired: timer could not be fired with id=%d\n", state->timer->id);
        }
        return true;
    }

    // Cancel timer
    fuse_debugf(state->fuse, "rp2040_timer_repeating_fired: periodic timer canceled with id=%d\n", state->timer->id);
    free(timer);
    free(state);
    return false;
}

int64_t rp2040_timer_alarm_fired(alarm_id_t id, void *user_data)
{
    assert(user_data);
    struct fuse_timer_instance *state = (struct fuse_timer_instance *)user_data;

    // Call the registered event handler
    if (!fuse_event_fire(state->fuse, EV_TIMER, state->timer))
    {
        fuse_debugf(state->fuse, "rp2040_timer_alarm_fired: timer could not be fired with id=%d\n", state->timer->id);
    }

    // Do not reschedule the alarm
    free(state);
    return 0;
}

/*
 * Create a new timer
 */
static void rp2040_timer_init(fuse_t *fuse, fuse_driver_t *driver, fuse_event_t event, void *data)
{
    assert(fuse);
    assert(driver);
    assert(data);
    assert(event == EV_TIMER_INIT);

    // Create a state object
    fuse_timer_t *timer = (fuse_timer_t *)data;
    struct fuse_timer_instance *state = malloc(sizeof(struct fuse_timer_instance));
    if (state == NULL)
    {
        fuse_debugf(fuse, "rp2040_timer_init: failed to allocate memory for timer\n");
        return;
    }
    else
    {
        state->fuse = fuse;
        state->timer = timer;
    }

    timer_t *self = (timer_t *)driver;
    if (timer->repeat)
    {
        // Create a repeating timer
        repeating_timer_t *repeating_timer = malloc(sizeof(repeating_timer_t));
        if (repeating_timer == NULL)
        {
            fuse_debugf(fuse, "rp2040_timer_init: failed to allocate memory for repeating timer\n");
            free(state);
            return;
        }
        if (!alarm_pool_add_repeating_timer_ms(self->alarm_pool, timer->delay_ms, rp2040_timer_repeating_fired, state, repeating_timer))
        {
            fuse_debugf(fuse, "rp2040_timer_init: alarm_pool_add_repeating_timer_ms was unsusccessful\n");
            free(repeating_timer);
            free(state);
            return;
        }
    }
    else
    {
        if (!alarm_pool_add_alarm_in_ms(self->alarm_pool, timer->delay_ms, rp2040_timer_alarm_fired, state, false))
        {
            fuse_debugf(fuse, "rp2040_timer_init: alarm_pool_add_alarm_in_ms was unsusccessful\n");
            free(state);
            return;
        }
    }
}

/*
 * Register timer driver
 */
static void *rp2040_timer_register(fuse_t *fuse, void *data)
{
    timer_t *self = fuse_malloc(fuse, sizeof(timer_t));
    if (self == NULL)
    {
        fuse_debugf(fuse, "rp2040_timer_register: failed to allocate memory for driver\n");
        return NULL;
    }

    self->alarm_pool = alarm_pool_create_with_unused_hardware_alarm(PICO_PHEAP_MAX_ENTRIES);
    if (self->alarm_pool == NULL)
    {
        fuse_debugf(fuse, "rp2040_timer_register: failed to create alarm pool\n");
        return NULL;
    }

    // Register actions
    if (!fuse_register_action(fuse, EV_TIMER_INIT, self, rp2040_timer_init))
    {
        return NULL;
    }

    // Return success
    return self;
}

/*
 * Deregister timer - TODO
 */
static void rp2040_timer_deregister(fuse_t *fuse, fuse_driver_t *driver)
{
    assert(fuse);
    assert(driver);

    timer_t *self = (timer_t *)driver;
    alarm_pool_destroy(self->alarm_pool);
}

///////////////////////////////////////////////////////////////////////////////

fuse_driver_params_t rp2040_timer = {
    .name = "rp2040_timer",
    .init = rp2040_timer_register,
    .events = {
        {.event = EV_TIMER_INIT, .name = "EV_TIMER_INIT"},
        {.event = EV_TIMER, .name = "EV_TIMER"},
        {.event = 0, .name = NULL, .finalizer = NULL}}};
