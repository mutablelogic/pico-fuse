/** @file fuse.h
 *  @brief Private function prototypes and structure definitions for fuse
 */
#ifndef FUSE_PRIVATE_FUSE_H
#define FUSE_PRIVATE_FUSE_H

#include <stdbool.h>
#include <fuse/fuse.h>
#include "alloc.h"
#include "event.h"
#include "list.h"

///////////////////////////////////////////////////////////////////////////////
// TYPES

/* @brief Represents an instance of a fuse application
 */
struct fuse_application
{
    struct fuse_allocator *allocator;              ///< The allocator for the application
    fuse_value_desc_t desc[FUSE_MAGIC_COUNT]; ///< Value descriptors
    int exit_code;                                 ///< Exit code of the application
    struct fuse_list* core0; ///< Core 0 event queue
    struct event_callbacks callbacks0[FUSE_EVENT_COUNT]; ///< Core 0 callbacks
    struct fuse_list* core1; ///< Core 1 event queue
    struct event_callbacks callbacks1[FUSE_EVENT_COUNT]; ///< Core 1 callbacks
    bool drain;                         ///< Drain the memory pool
};

#endif
