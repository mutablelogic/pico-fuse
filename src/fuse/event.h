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

/** @brief Append a quoted string representation of an event
 */
size_t fuse_qstr_event(fuse_t *self, char *buf, size_t sz, size_t i, fuse_value_t *v);

#endif
