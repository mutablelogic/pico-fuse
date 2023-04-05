#include <stdlib.h>
#include <pico/stdlib.h>

#include "picofuse.h"

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
    //hashmap_t *hashmap;
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
