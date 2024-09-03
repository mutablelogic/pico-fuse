/** @file event.h
 *  @brief Private function prototypes and structure definitions for events.
 */
#ifndef FUSE_PRIVATE_EVENT_H
#define FUSE_PRIVATE_EVENT_H

#include <fuse/fuse.h>
#include <stdint.h>

/** @brief Event data
 */
struct event_context
{
    uint8_t type;
    fuse_value_t *source;
    void *user_data;
};

/** @brief Array of event callbacks
 */
struct event_callbacks
{
    fuse_callback_t callback[FUSE_EVENT_CALLBACK_COUNT];
};

/** @brief Register value type for events
 */
void fuse_register_value_event(fuse_t *self);

#endif
