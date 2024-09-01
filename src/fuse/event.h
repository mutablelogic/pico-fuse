/** @file event.h
 *  @brief Private function prototypes and structure definitions for events.
 */
#ifndef FUSE_PRIVATE_EVENT_H
#define FUSE_PRIVATE_EVENT_H

#include <fuse/fuse.h>

struct event_context
{
    uint8_t type;
    fuse_value_t *source;
    void *user_data;
};

/** @brief Register value primitive for a mutex
 */
void fuse_register_value_event(fuse_t *self);

#endif
