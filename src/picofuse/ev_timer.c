#include <stdlib.h>

#include <picofuse/picofuse.h>
#include <pico/stdlib.h>
#include <pico/time.h>
#include <pico/util/pheap.h>

#include "ev.h"
#include "debug.h"

static alarm_pool_t *alarm_pool = NULL;

struct picofuse_timer_state_t
{
    picofuse_t *self;
    picofuse_timer_t *data;
};

///////////////////////////////////////////////////////////////////////////////
// Create and destroy the alarm pool

int picofuse_timer_init(picofuse_t *self)
{
    // Create a timer pool
    if (alarm_pool == NULL)
    {
        alarm_pool = alarm_pool_create_with_unused_hardware_alarm(PICO_PHEAP_MAX_ENTRIES);
    }

    // Return 0 on success
    return alarm_pool != NULL;
}

void picofuse_timer_deinit(picofuse_t *self)
{
    if (alarm_pool != NULL)
    {
        alarm_pool_destroy(alarm_pool);
        alarm_pool = NULL;
    }
}

///////////////////////////////////////////////////////////////////////////////
// Callbacks for timer fired

bool picofuse_handle_timer_repeating_fire(repeating_timer_t *timer)
{
    struct picofuse_timer_state_t *state = (struct picofuse_timer_state_t *)timer->user_data;

    // Call the registered event handler when periodic, and reschedule
    if (state->data->periodic)
    {
        if (picofuse_fire(state->self, EV_TIMER, state->data))
        {
            picofuse_debug("picofuse_handle_timer_repeating_fire: timer could not be fired with id=%d\n", state->data->id);
        }
        return true;
    }

    // Cancel timer
    picofuse_debug("picofuse_handle_timer_repeating_fire: periodic timer canceled with id=%d\n", state->data->id);
    free(timer);
    free(state);
    return false;
}

int64_t picofuse_handle_timer_alarm_fire(alarm_id_t id, void *user_data)
{
    struct picofuse_timer_state_t *state = (struct picofuse_timer_state_t *)user_data;

    // Call the registered event handler
    if (picofuse_fire(state->self, EV_TIMER, state->data))
    {
        picofuse_debug("picofuse_handle_timer_alarm_fire: timer could not be fired\n");
    }

    // Do not reschedule the alarm
    free(state);
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// Handle the EVENT_TIMER_INIT event

bool picofuse_handle_timer_init_alarm(picofuse_t *self, picofuse_timer_t *data)
{
    struct picofuse_timer_state_t *state = malloc(sizeof(struct picofuse_timer_state_t));
    if (state == NULL)
    {
        return false;
    }
    else
    {
        state->self = self;
        state->data = data;
    }

    if (!alarm_pool_add_alarm_in_ms(alarm_pool, data->delay_ms, picofuse_handle_timer_alarm_fire, state, false))
    {
        free(state);
        return false;
    }

    // Return success
    return true;
}

///////////////////////////////////////////////////////////////////////////////
// Handle the EVENT_TIMER_INIT event

bool picofuse_handle_timer_init_repeating(picofuse_t *self, picofuse_timer_t *data)
{
    struct picofuse_timer_state_t *state = malloc(sizeof(struct picofuse_timer_state_t));
    if (state == NULL)
    {
        return false;
    }
    else
    {
        state->self = self;
        state->data = data;
    }

    // Create a repeating timer
    repeating_timer_t *timer = malloc(sizeof(repeating_timer_t));
    if (timer == NULL)
    {
        free(state);
        return false;
    }
    if (!alarm_pool_add_repeating_timer_ms(alarm_pool, data->delay_ms, picofuse_handle_timer_repeating_fire, state, timer))
    {
        free(state);
        free(timer);
        return false;
    }

    // Return success
    return true;
}

///////////////////////////////////////////////////////////////////////////////
// Handle the EVENT_TIMER_INIT event

void picofuse_handle_timer_init(picofuse_t *self, picofuse_timer_t *data)
{
    // Call the registered event handler
    picofuse_callback(self, picofuse_event(self), (void *)(data));

    // If the delay_ms is empty, then we do nothing
    if (data->delay_ms == 0)
    {
        return;
    }

    // If periodic, the create a repeating timer else just an alarm
    bool success;
    if (data->periodic)
    {
        success = picofuse_handle_timer_init_repeating(self, data);
    }
    else
    {
        success = picofuse_handle_timer_init_alarm(self, data);
    }

    // Debug
    if (!success)
    {
        picofuse_debug("picofuse_handle_timer_init: failed to create timer with id=%d\n", data->id);
    }
    else
    {
        picofuse_debug("picofuse_handle_timer_init: created timer with id=%d delay_ms=%d periodic=%d\n", data->id, data->delay_ms, data->periodic);
    }
}

///////////////////////////////////////////////////////////////////////////////
// Handle the EVENT_TIMER event

void picofuse_handle_timer(picofuse_t *self, picofuse_timer_t *data)
{
    // Call the registered event handler
    picofuse_callback(self, picofuse_event(self), (void *)(data));
}

///////////////////////////////////////////////////////////////////////////////
// Schedule a timer with a callback

struct picofuse_schedule_state
{
    picofuse_t *self;
    picofuse_schedule_callback_t callback;
    alarm_id_t alarm;
    uint32_t delay_ns;
    void *data;
};

static int64_t picofuse_schedule_callback(alarm_id_t alarm, void *data)
{
    // If no callback, then don't do anything
    struct picofuse_schedule_state *state = data;
    if (state == NULL || state->callback == NULL)
    {
        return 0;
    }

    // Reschedule the alarm or cancel it
    if (!state->callback(state->self, state->data))
    {
        free(state);
        return 0;
    }
    else
    {
        return state->delay_ns;
    }
}

bool picofuse_schedule_ms(picofuse_t *self, uint32_t delay_ms, picofuse_schedule_callback_t callback, void *data)
{
    struct picofuse_schedule_state *state = malloc(sizeof(struct picofuse_schedule_state));
    if (state == NULL)
    {
        return false;
    }
    else
    {
        state->self = self;
        state->callback = callback;
        state->data = data;
        state->delay_ns = delay_ms * 1000;
    }

    alarm_id_t alarm = alarm_pool_add_alarm_in_ms(alarm_pool, delay_ms, picofuse_schedule_callback, state, true);
    if (alarm < 1)
    {
        free(state);
        return false;
    }
    else
    {
        state->alarm = alarm;
    }

    // Return success
    return true;
}
