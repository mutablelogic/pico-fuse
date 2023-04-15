#ifndef FUSE_H
#define FUSE_H

#include <fuse/debug.h>
#include <fuse/main.h>
#include <fuse/pool.h>

/*
 * An event in fuse, which can be used to trigger an action
 */
typedef enum {
    // Initialize the application
    EV_INIT = 1
} fuse_event_t;

#endif
