#ifndef FUSE_PRIVATE_ACTION_H
#define FUSE_PRIVATE_ACTION_H

#include <fuse/fuse.h>

/*
 * Call actions for an event
 * 
 * @param fuse              The fuse instance
 * @param event             The event to handle
 * @param data              The event data
 * @return                  True if the event was handled, or false if no actions were found
 */
bool fuse_call_action(fuse_t *fuse, fuse_event_t event, void *data);

#endif
