/** @file event.h
 *  @brief Fuse events
 *
 *  This file enumerates events
 */
#ifndef FUSE_EVENT_H
#define FUSE_EVENT_H

// Define the event types
#define FUSE_EVENT_NULL 0x00  ///< NULL value
#define FUSE_EVENT_TIMER 0x01 ///< Timer event

// Maximum number of magic numbers
#define FUSE_EVENT_COUNT 0x02 ///< Maximum number of events

#endif /* FUSE_EVENT_H */
