#include <stdio.h>
#include <stdlib.h>
#include <pico/stdlib.h>
#include <picofuse/picofuse.h>
#include "ev.h"
#include "hashmap.h"

///////////////////////////////////////////////////////////////////////////////
// Structures

struct picofuse_node_t
{
    picofuse_event_t type;
    void *data;
    struct picofuse_node_t *next;
};

struct picofuse_instance_t
{
    picofuse_state_t state;
    picofuse_flags_t flags;
    struct picofuse_node_t *head;
    struct picofuse_node_t *tail;
    hashmap_t *hashmap;
};

///////////////////////////////////////////////////////////////////////////////
// Global variables

picofuse_t *self = NULL;
picofuse_init_t picofuse_init_data;

///////////////////////////////////////////////////////////////////////////////
// Initialize the runloop structure

picofuse_t *picofuse_init(picofuse_flags_t flags)
{
    if (self != NULL)
    {
        return self;
    }

    self = malloc(sizeof(picofuse_t));
    if (self == NULL)
    {
        return NULL;
    }

    self->head = NULL;
    self->tail = NULL;
    self->state = ZERO;
    self->flags = flags;

    // Add an EV_INIT event onto the queue to get things started
    if (picofuse_fire(self, EV_INIT, &picofuse_init_data))
    {
        free(self);
        self = NULL;
        return NULL;
    }

    // Return the picofuse object
    return self;
}

///////////////////////////////////////////////////////////////////////////////
// Determine if list is empty

bool picofuse_is_empty(picofuse_t *runloop)
{
    return runloop->head == NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Get next event, or returns EVENT_NONE

picofuse_event_t picofuse_next(picofuse_t *self, void **data)
{
    if (picofuse_is_empty(self))
    {
        return EV_NONE;
    }
    struct picofuse_node_t *node = self->head;
    self->head = self->head->next;
    if (self->head == NULL)
    {
        self->tail = NULL;
    }
    *data = node->data;
    picofuse_event_t type = node->type;
    free(node);
    return type;
}

///////////////////////////////////////////////////////////////////////////////
// Fire an event immediately, returns -1 on error or 0 on success

int picofuse_fire(picofuse_t *self, picofuse_event_t type, void *data)
{
    struct picofuse_node_t *node = malloc(sizeof(struct picofuse_node_t));
    if (node == NULL)
    {
        return -1;
    }
    node->type = type;
    node->data = data;
    node->next = NULL;
    if (picofuse_is_empty(self))
    {
        self->head = node;
        self->tail = node;
    }
    else
    {
        self->tail->next = node;
        self->tail = node;
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// Call a handler and update state as needed

void picofuse_callback(picofuse_t *self, picofuse_event_t event, void *data)
{
    picofuse_callback_t *callback = hashmap_get(self->hashmap, self->state, event);
    if (callback == NULL)
    {
        callback = hashmap_get(self->hashmap, ANY, event);
    }
    if (callback != NULL)
    {
        picofuse_state_t state = callback(self, self->state, event, data);
        if (state != ANY)
        {
            self->state = state;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Register an event handler for a given (state, event) pair. Returns -1 on
// error or 0 on success.

int picofuse_register(picofuse_t *self, picofuse_state_t state,
                      picofuse_event_t event, picofuse_callback_t *callback)
{
    return hashmap_put(self->hashmap, state, event, (void *)(callback));
}

///////////////////////////////////////////////////////////////////////////////
// Free resources

void picofuse_free(picofuse_t *self)
{
    while (!picofuse_is_empty(self))
    {
        void *data;
        picofuse_next(self, &data);
    }
    hashmap_free(self->hashmap);
    free(self);
}

///////////////////////////////////////////////////////////////////////////////
// Main

int picofuse_main(picofuse_t *self)
{
    int errorCode = 0;
    while (!errorCode)
    {
        if (picofuse_is_empty(self))
        {
            sleep_ms(10);
            continue;
        }

        // Get next event from the queue
        void *data;
        picofuse_event_t type = picofuse_next(self, &data);

        // Process the event
        switch (type)
        {
        case EV_NONE:
            break;
        case EV_INIT:
            picofuse_handle_init(self, (picofuse_init_t *)data);
            break;
        case EV_QUIT:
            picofuse_handle_quit(self, (picofuse_init_t *)data);
            break;
        default:
            printf("Unhandled event=%d data=%p\n", type, data);
        }
    }

    return errorCode;
}
