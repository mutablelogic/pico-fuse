
#include <stdio.h>
#include <stdlib.h>
#include <pico/stdlib.h>
#include <pico/binary_info.h>
#include <hardware/adc.h>

#include "runloop.h"

#define HAS_FLAG(f) ((runloop->flags & f) == f)

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
};

///////////////////////////////////////////////////////////////////////////////
// GLOBALS

runloop_init_t runloop_init_data;
runloop_adc_t runloop_adc_data[5];

///////////////////////////////////////////////////////////////////////////////
// Initialize the runloop structure

runloop_t *runloop_init(runloop_flags_t flags)
{
    runloop_t *r = malloc(sizeof(runloop_t));
    if (r == NULL)
    {
        return NULL;
    }
    r->head = NULL;
    r->tail = NULL;
    r->state = ZERO;
    r->flags = flags;

    // Add an INIT event onto the queue to get things started
    if (runloop_push(r, EVENT_INIT, &runloop_init_data))
    {
        free(r);
        return NULL;
    }

    // Return the runloop
    return r;
}

///////////////////////////////////////////////////////////////////////////////
// Free runloop structure

void runloop_free(runloop_t *runloop)
{
    // TODO: Deallocate all the events
    free(runloop);
}

///////////////////////////////////////////////////////////////////////////////
// Determine if list is empty

bool runloop_is_empty(runloop_t *runloop)
{
    return runloop->head == NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Push an event onto the runloop, returns -1 on error or 0 on success

int runloop_push(runloop_t *runloop, runloop_event_t type, void *data)
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

void runloop_event(runloop_t *runloop, runloop_state_t state,
                   runloop_event_t event, runloop_callback_t *callback)
{
    printf("TODO: Register event handler for state=%d event=%d\n", state, event);
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
        runloop_push(runloop, EVENT_ADC_INIT, &runloop_adc_data[0]);
    }
    if (HAS_FLAG(ADC_1))
    {
        runloop_adc_data[1].channel = 1;
        runloop_adc_data[1].gpio = 27;
        runloop_push(runloop, EVENT_ADC_INIT, &runloop_adc_data[1]);
    }
    if (HAS_FLAG(ADC_2))
    {
        runloop_adc_data[2].channel = 2;
        runloop_adc_data[2].gpio = 28;
        runloop_push(runloop, EVENT_ADC_INIT, &runloop_adc_data[2]);
    }
    if (HAS_FLAG(ADC_3))
    {
        runloop_adc_data[3].channel = 3;
        runloop_adc_data[3].gpio = 29;
        runloop_push(runloop, EVENT_ADC_INIT, &runloop_adc_data[3]);
    }
    if (HAS_FLAG(ADC_4))
    {
        runloop_adc_data[4].channel = 4;
        runloop_adc_data[4].gpio = 0;
        runloop_push(runloop, EVENT_ADC_INIT, &runloop_adc_data[4]);
    }

    // TODO: Call the registered event handler
    // set the appName if not NULL
    if (data->appName != NULL)
    {
        bi_decl(bi_program_description(data->appName));
    }
}

///////////////////////////////////////////////////////////////////////////////
// Handle the EVENT_ADC_INIT event

void runloop_handle_adc_init(runloop_t *runloop, runloop_adc_t *data)
{
    printf("EVENT_ADC_INIT channel=%d gpio=%d\n", data->channel, data->gpio);

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

    // Dispatch to any registered event handlers

    // Set pin name
    if (data->gpio != 0 && data->pinName != NULL)
    {
        bi_decl(bi_1pin_with_name(data->gpio, data->pinName));
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
        default:
            printf("Other event=%d data=%p\n", type, data);
        }
        sleep_ms(1000);
    }
}
