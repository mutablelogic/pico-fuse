/** @file value.h
 *  @brief Private function prototypes and structure definitions for values
 */
#ifndef FUSE_PRIVATE_VALUE_H
#define FUSE_PRIVATE_VALUE_H

#include <fuse/fuse.h>

/** @brief Represents a value
 *
 *  This data structure represents an arbitary value, including NULL values
 */
struct fuse_value_instance
{
    void *ptr;                        ///< A pointer to the value storage
    struct fuse_value_instance *prev; ///< The previous value in the list (when the value is part of a list)
    struct fuse_value_instance *next; ///< The next value in the list  (when the value is part of a list)
};

#endif
