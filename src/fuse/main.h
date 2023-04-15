#ifndef FUSE_PRIVATE_MAIN_H
#define FUSE_PRIVATE_MAIN_H

#include <fuse/pool.h>
#include <fuse/main.h>

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
};

/*
 * Represents an event node
 */
struct fuse_node_instance
{
    void *data;
    struct fuse_node_instance *next;
};

#endif
