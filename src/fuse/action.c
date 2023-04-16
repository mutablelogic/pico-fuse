#include <stddef.h>
#include <stdlib.h>

#include <pico/stdlib.h>
#include <fuse/fuse.h>
#include "main.h"

///////////////////////////////////////////////////////////////////////////////

struct fuse_action_node
{
    // The driver associated with this action
    fuse_driver_t *driver;

    // The action callback
    fuse_action_t action;

    // The next node to call
    struct fuse_action_node *next;
};

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

/*
 * Register an action for an event firing
 */
bool fuse_register_action(fuse_t *fuse, fuse_event_t event, fuse_driver_t *driver, fuse_action_t action)
{
    assert(fuse);
    assert(fuse->events);
    assert(fuse->actions);
    assert(action);

    // Create a new node
    struct fuse_action_node *new = fuse_malloc(fuse, sizeof(struct fuse_action_node));
    if (new == NULL)
    {
        fuse_debugf(fuse, "fuse_register_action: failed to allocate memory for action\n");
        return false;
    }

    // Set the action properties
    new->driver = driver;
    new->action = action;
    new->next = NULL;

    // Retrieve the existing action for this event
    struct fuse_action_node *node = hashmap_get_voidp(fuse->actions, (void *)event);
    if (node == NULL)
    {
        // Add the action to the hashmap
        if (!hashmap_put_voidp(fuse->actions, (void *)event, new))
        {
            fuse_debugf(fuse, "fuse_register_action: failed to add action to hashmap\n");
            return false;
        }
    }
    else
    {
        // Add the action to the end of the chain
        while(node->next != NULL)
        {
            node = node->next;
        }
        node->next = new;
    }

    // Return success
    return true;
}

/*
 * Call actions for an event
 */
bool fuse_call_action(fuse_t *fuse, fuse_event_t event, void *data)
{
    assert(fuse);
    assert(fuse->actions);
    assert(event);

    // Retrieve the existing action for this event
    struct fuse_action_node *node = hashmap_get_voidp(fuse->actions, (void *)event);
    if (node == NULL)
    {
        return false;
    }

    // Call actions in a chain
    while (node != NULL)
    {
        assert(node->action);
        node->action(fuse, node->driver, event, data);

        // Get next action to call
        node = node->next;
    }

    // Call the finalizer
    fuse_driver_event_t *event_driver = hashmap_get_voidp(fuse->events, (void *)event);
    if (event_driver != NULL && event_driver->finalizer != NULL)
    {
        event_driver->finalizer(fuse, event_driver->driver, event, data);
    }

    // Return success
    return true;
}
