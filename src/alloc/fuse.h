/** @file fuse.h
 *  @brief Private function prototypes and structure definitions for fuse
 */
#ifndef FUSE_PRIVATE_FUSE_H
#define FUSE_PRIVATE_FUSE_H

#include <alloc/alloc.h>

/* @brief Represents an instance of a fuse application
 */
struct fuse_application
{
    fuse_allocator_t *allocator; ///< The allocator for the application
    int exit_code;               ///< Exit code of the application
};

#endif
