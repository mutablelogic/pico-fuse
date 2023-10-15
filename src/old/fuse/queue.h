#ifndef FUSE_PRIVATE_QUEUE_H
#define FUSE_PRIVATE_QUEUE_H

#include <fuse/fuse.h>

/*
 * Represents an event node
 */
struct fuse_node_instance
{
    // The event to fire
    fuse_event_t event;

    // The extra parameters for the event
    void *data;

    // The next event in the linked list
    struct fuse_node_instance *next;
};

/*
 * Represents an event queue
 */
typedef struct
{
    struct fuse_node_instance *head;
    struct fuse_node_instance *tail;
} fuse_queue_t;

/*
 * Create a new fuse event queue
 */
fuse_queue_t *fuse_queue_new(fuse_pool_t *pool);

/*
 * Determine if the queue is empty
 *
 * @param queue              The queue
 * @return                   Returns true if the queue is empty
 */
bool fuse_queue_isempty(fuse_queue_t *queue);

/*
 * Free queue resources
 *
 * This function empties the queue
 *
 * @param queue              The queue
 */
void fuse_queue_zero(fuse_queue_t *queue);

/*
 * Return next event in the queue
 *
 * @param queue             The queue
 * @param data              A pointer to receive the data associated with the event
 * @return                  The event type, or zero if no event is available
 */
fuse_event_t fuse_queue_next(fuse_queue_t *queue, void **data);

/*
 * Add an event onto the queue
 *
 * @param queue             The queue
 * @param event             The event
 * @param data              The data associated with the event
 * @return                  Returns false on failure, which usually indicates an
 *                          out-of-memory condition
 */
bool fuse_queue_add(fuse_queue_t *queue, fuse_event_t event, void *data);

#endif
