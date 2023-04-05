
#include <stdio.h>
#include <stdlib.h>
#include <pico/stdlib.h>
#include <pico/binary_info.h>
#include <hardware/adc.h>
#include <hardware/gpio.h>

#include "runloop.h"
#include "hashmap.h"

#define HAS_FLAG(f) ((runloop->flags & f) == f)
#define HASHMAP_SIZE 100

///////////////////////////////////////////////////////////////////////////////
// STRUCTURES

struct runloop_node_t
{
    runloop_event_t type;
    void *data;
    struct runloop_node_t *next;
};

struct runloop_instance_t
{
    runloop_state_t state;
    runloop_flags_t flags;
    struct runloop_node_t *head;
    struct runloop_node_t *tail;
    hashmap_t *hashmap;
};

///////////////////////////////////////////////////////////////////////////////
// GLOBALS

runloop_t *runloop;
runloop_init_t runloop_init_data;
runloop_adc_t runloop_adc_data[5];

///////////////////////////////////////////////////////////////////////////////
// Initialize the runloop structure

runloop_t *runloop_init(runloop_flags_t flags)
{
    if (runloop != NULL)
    {
        return runloop;
    }

    runloop = malloc(sizeof(runloop_t));
    if (runloop == NULL)
    {
        return NULL;
    }

    runloop->head = NULL;
    runloop->tail = NULL;
    runloop->state = ZERO;
    runloop->flags = flags;
    runloop->hashmap = hashmap_init(HASHMAP_SIZE);
    if (runloop->hashmap == NULL)
    {
        free(runloop);
        runloop = NULL;
        return NULL;
    }

    // Add an INIT event onto the queue to get things started
    if (runloop_fire(runloop, EVENT_INIT, &runloop_init_data))
    {
        free(runloop);
        runloop = NULL;
        return NULL;
    }

    // Return the runloop
    return runloop;
}

///////////////////////////////////////////////////////////////////////////////
// Determine if list is empty

bool runloop_is_empty(runloop_t *runloop)
{
    return runloop->head == NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Fire an event on the runloop, returns -1 on error or 0 on success

int runloop_fire(runloop_t *runloop, runloop_event_t type, void *data)
{
    struct runloop_node_t *node = malloc(sizeof(struct runloop_node_t));
    if (node == NULL)
    {
        return -1;
    }
    node->type = type;
    node->data = data;
    node->next = NULL;
    if (runloop_is_empty(runloop))
    {
        runloop->head = node;
        runloop->tail = node;
    }
    else
    {
        runloop->tail->next = node;
        runloop->tail = node;
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// Pop an event from the runloop, returns EVENT_NONE

runloop_event_t runloop_pop(runloop_t *runloop, void **data)
{
    if (runloop_is_empty(runloop))
    {
        return EVENT_NONE;
    }
    struct runloop_node_t *node = runloop->head;
    runloop->head = runloop->head->next;
    if (runloop->head == NULL)
    {
        runloop->tail = NULL;
    }
    *data = node->data;
    runloop_event_t type = node->type;
    free(node);
    return type;
}

///////////////////////////////////////////////////////////////////////////////
// Register an event handler for a given (state, event) pair

int runloop_event(runloop_t *runloop, runloop_state_t state,
                  runloop_event_t event, runloop_callback_t *callback)
{
    return hashmap_put(runloop->hashmap, state, event, (void *)(callback));
}

///////////////////////////////////////////////////////////////////////////////
// Call a handler and update state as needed

void runloop_callback(runloop_t *runloop, runloop_event_t event, void *data)
{
    runloop_callback_t *callback = hashmap_get(runloop->hashmap, runloop->state, event);
    if (callback == NULL)
    {
        callback = hashmap_get(runloop->hashmap, ANY, event);
    }
    if (callback != NULL)
    {
        runloop_state_t state = callback(runloop, runloop->state, event, data);
        if (state != ANY)
        {
            runloop->state = state;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Handle the EVENT_INIT event

void runloop_handle_init(runloop_t *runloop, runloop_init_t *data)
{
    stdio_init_all();
    sleep_ms(1000);

    // Debug
    printf("EVENT_INIT\n");

    // ADC initialization
    if (runloop->flags & (ADC_0 | ADC_1 | ADC_2 | ADC_3 | ADC_4))
    {
        adc_init();
    }
    if (HAS_FLAG(ADC_0))
    {
        runloop_adc_data[0].channel = 0;
        runloop_adc_data[0].gpio = 26;
        runloop_fire(runloop, EVENT_ADC_INIT, &runloop_adc_data[0]);
    }
    if (HAS_FLAG(ADC_1))
    {
        runloop_adc_data[1].channel = 1;
        runloop_adc_data[1].gpio = 27;
        runloop_fire(runloop, EVENT_ADC_INIT, &runloop_adc_data[1]);
    }
    if (HAS_FLAG(ADC_2))
    {
        runloop_adc_data[2].channel = 2;
        runloop_adc_data[2].gpio = 28;
        runloop_fire(runloop, EVENT_ADC_INIT, &runloop_adc_data[2]);
    }
    if (HAS_FLAG(ADC_3))
    {
        runloop_adc_data[3].channel = 3;
        runloop_adc_data[3].gpio = 29;
        runloop_fire(runloop, EVENT_ADC_INIT, &runloop_adc_data[3]);
    }
    if (HAS_FLAG(ADC_4))
    {
        runloop_adc_data[4].channel = 4;
        runloop_adc_data[4].gpio = 0;
        runloop_fire(runloop, EVENT_ADC_INIT, &runloop_adc_data[4]);
    }

    // Call the registered event handler
    runloop_callback(runloop, EVENT_INIT, (void *)(data));

    // set the appName if not NULL
    if (data->appName != NULL)
    {
        printf("TODO: Set appname=%s\n", data->appName);
    }
}

///////////////////////////////////////////////////////////////////////////////
// Handle the EVENT_ADC_INIT event

void runloop_handle_adc_init(runloop_t *runloop, runloop_adc_t *data)
{
    // Set GPIO pin for ADC
    if (data->gpio != 0)
    {
        adc_gpio_init(data->gpio);
        adc_select_input(data->channel);
    }

    // Enable temperature sensor
    if (data->channel == 4)
    {
        adc_set_temp_sensor_enabled(true);
    }

    // Call the registered event handler
    runloop_callback(runloop, EVENT_ADC_INIT, (void *)(data));

    // Set pin name
    if (data->gpio != 0 && data->pinName != NULL)
    {
        printf("TODO: Set gpio=%d adc=%d name=%s\n", data->gpio, data->channel, data->pinName);
    }
}

///////////////////////////////////////////////////////////////////////////////
// Handle the EVENT_GPIO_INIT event

void runloop_handle_gpio_callback(uint gpio, uint32_t events)
{
    // printf("GPIO IRQ: gpio=%d events=%d\n",gpio, events);
    if (runloop != NULL)
    {
        // Trigger event
        runloop_fire(runloop, EVENT_GPIO, NULL);
    }
}

void runloop_handle_gpio_init(runloop_t *runloop, runloop_gpio_t *data)
{
    // Set GPIO pin direction, and pulls
    if (data->gpio != 0)
    {
        // gpio_init(data->gpio);
        if (data->direction == GPIO_IN)
        {
            gpio_set_input_enabled(data->gpio, true);
            gpio_set_pulls(data->gpio, data->pullup, data->pulldown);
        }
        else
        {
            gpio_set_input_enabled(data->gpio, false);
        }

        // Set IRQ callback
        uint32_t events;
        if (data->irqrise)
        {
            events |= GPIO_IRQ_EDGE_RISE;
        }
        if (data->irqfall)
        {
            events |= GPIO_IRQ_EDGE_FALL;
        }
        if (events > 0)
        {
            gpio_set_irq_enabled_with_callback(data->gpio, events, true, runloop_handle_gpio_callback);
        }
    }

    // Call the registered event handler
    runloop_callback(runloop, EVENT_GPIO_INIT, (void *)(data));

    // Set pin name
    if (data->gpio != 0 && data->pinName != NULL)
    {
        printf("TODO: Set gpio=%d direction=%d name=%s\n", data->gpio, data->direction, data->pinName);
    }
}

///////////////////////////////////////////////////////////////////////////////
// EVENT_LED

void runloop_handle_led(runloop_t *runloop, bool value)
{
    printf("TODO: Set led=%d\n", value);
}

///////////////////////////////////////////////////////////////////////////////
// EVENT_TIMER_INIT and EVENT_TIMER

bool runloop_handle_timer_callback(repeating_timer_t *timer)
{
    // Call the registered event handler
    runloop_callback(runloop, EVENT_TIMER, timer->user_data);

    // TODO: free the timer if we want to cancel the timer

    // Should not cancel the timer here
    return true;
}

void runloop_handle_timer_init(runloop_t *runloop, runloop_timer_t *data)
{
    if (data->delay_ms != 0)
    {
        // Create a structure to hold the timer data
        repeating_timer_t *timer = malloc(sizeof(repeating_timer_t));
        if (timer == NULL)
        {
            printf("ERROR: Failed to add timer\n");
            return;
        }

        // Set timer
        if (!add_repeating_timer_ms(data->delay_ms, runloop_handle_timer_callback, (void *)(data), timer))
        {
            printf("ERROR: Failed to add timer\n");
            return;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Run

void runloop_main(runloop_t *runloop)
{
    while (true)
    {
        if (runloop_is_empty(runloop))
        {
            sleep_ms(10);
            continue;
        }

        // Pop the next event off the queue
        void *data;
        runloop_event_t type = runloop_pop(runloop, &data);

        // Process the event
        switch (type)
        {
        case EVENT_NONE:
            break;
        case EVENT_INIT:
            runloop_handle_init(runloop, (runloop_init_t *)data);
            break;
        case EVENT_ADC_INIT:
            runloop_handle_adc_init(runloop, (runloop_adc_t *)data);
            break;
        case EVENT_GPIO_INIT:
            runloop_handle_gpio_init(runloop, (runloop_gpio_t *)data);
            break;
        case EVENT_GPIO:
            runloop_callback(runloop, EVENT_GPIO, data);
            break;
        case EVENT_LED:
            runloop_handle_led(runloop, (bool)data);
            break;
        case EVENT_TIMER_INIT:
            runloop_handle_timer_init(runloop, (runloop_timer_t *)data);
            break;
        default:
            printf("Unhandled event=%d data=%p\n", type, data);
        }
    }
}
