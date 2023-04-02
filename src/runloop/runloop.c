
#include <stdio.h>
#include <pico/stdlib.h>
#include "runloop.h"

typedef struct
{
    rl_event_type type;
    void *data;
    struct runloop_node *next;
} runloop_node_t;

struct runloop_instance_t
{
    runloop_node_t *head;
    runloop_node_t *tail;
};

// Initialize the runloop structure
runloop_t *runloop_init()
{
    runloop_t *runloop = malloc(sizeof(runloop_t));
    if (runloop == NULL)
    {
        return NULL;
    }
    runloop->head = NULL;
    runloop->tail = NULL;
    return runloop;
}

// Free runloop structure
void runloop_free(runloop_t *runloop)
{
    // TODO: Deallocate all the events
    free(runloop);
}

// Determine if list is empty
bool runloop_is_empty(runloop_t *runloop)
{
    return runloop->head == NULL;
}

// Push an event onto the runloop, returns -1 on error or 0 on success
int runloop_push(runloop_t *runloop, rl_event_type type, void *data)
{
    runloop_node_t *node = malloc(sizeof(runloop_node_t));
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
    // Return success
    return 0;
}

// Pop an event from the runloop, returns NULL on error or the data otherwise
void *runloop_pop(runloop_t *runloop, rl_event_type *type)
{
    if (runloop_is_empty(runloop))
    {
        *type = RL_EVENT_NONE;
        return NULL;
    }
    void *data = runloop->head->data;
    *type = runloop->head->type;
    runloop_node_t *node = runloop->head;
    runloop->head = runloop->head->next;
    if (runloop->head == NULL)
    {
        runloop->tail = NULL;
    }
    free(node);
    return data;
}

// Run the loop forever
void runloop_run(runloop_t *runloop)
{
    while (true)
    {
        tight_loop_contents();
    }
}
