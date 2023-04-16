#ifndef FUSE_PRIVATE_MAIN_H
#define FUSE_PRIVATE_MAIN_H

#include <fuse/fuse.h>
#include "queue.h"
#include "hashmap.h"

/*
 * Represents an instance of a fuse application
 */
struct fuse_instance
{
    // The memory pool
    fuse_pool_t *pool;

    // The flags which affect the behaviour of the application
    fuse_flag_t flags;

    // The return code from the application. Set as non-zero to
    // quit the application.
    int status;

    // Actions to fire when an event is fired
    hashmap_t* actions;

    // Registered events
    hashmap_t* events;

    // The event queue
    fuse_queue_t *queue;
};

#endif
