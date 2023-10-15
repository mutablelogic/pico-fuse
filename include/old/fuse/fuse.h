#ifndef FUSE_H
#define FUSE_H

/*
 * An event in fuse, which can be used to trigger an action
 */
typedef enum {
    // Initialize the application
    EV_INIT = 0x10000001
} fuse_event_t;

#include <fuse/debug.h>
#include <fuse/main.h>
#include <fuse/pool.h>

#endif
