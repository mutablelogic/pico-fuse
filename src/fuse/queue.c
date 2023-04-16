#include <stdlib.h>
#include <stdio.h>

#include <fuse/fuse.h>
#include "queue.h"

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

/*
 * Create a new fuse event queue
 */
fuse_queue_t *fuse_queue_new(fuse_pool_t *pool)
{
    assert(pool);

    // Allocate queue instance
    fuse_queue_t *queue = (fuse_queue_t *)fuse_pool_alloc(pool, sizeof(fuse_queue_t));
    if (queue == NULL)
    {
        return NULL;
    }

    // Set the instance properties
    queue->head = NULL;
    queue->tail = NULL;

    // Return success
    return queue;
}

/*
 * Return true if the queue is empty
 */
bool fuse_queue_isempty(fuse_queue_t *queue)
{
    return queue->head == NULL;
}

/*
 * Empty the queue
 */
void fuse_queue_zero(fuse_queue_t *queue)
{
    assert(queue);

    void *data;
    while (!fuse_queue_isempty(queue))
    {
        fuse_queue_next(queue, &data);
    }
}

/*
 * Return next event in the queue
 */
fuse_event_t fuse_queue_next(fuse_queue_t *queue, void **data)
{
    assert(queue);
    assert(data);

    if (fuse_queue_isempty(queue))
    {
        return 0;
    }

    assert(queue->head);
    struct fuse_node_instance *node = queue->head;
    queue->head = queue->head->next;
    if (queue->head == NULL)
    {
        queue->tail = NULL;
    }
    *data = node->data;
    fuse_event_t event = node->event;
    free(node);
    return event;
}

/*
 * Add an event onto the queue
 */
bool fuse_queue_add(fuse_queue_t *queue, fuse_event_t event, void *data)
{
    assert(queue);
    assert(event);

    struct fuse_node_instance *node = malloc(sizeof(struct fuse_node_instance));
    if (node == NULL)
    {
        printf("fuse_queue_add: malloc failed\n");
        return false;
    }
    node->event = event;
    node->data = data;
    node->next = NULL;
    if (fuse_queue_isempty(queue))
    {
        queue->head = node;
        queue->tail = node;
    }
    else
    {
        queue->tail->next = node;
        queue->tail = node;
    }

    // Return success
    return true;
}
