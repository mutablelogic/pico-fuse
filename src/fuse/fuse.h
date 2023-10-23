/** @file fuse.h
 *  @brief Private function prototypes and structure definitions for fuse
 */
#ifndef FUSE_PRIVATE_FUSE_H
#define FUSE_PRIVATE_FUSE_H

#include <fuse/fuse.h>

/* @brief Represents an instance of a fuse application
 */
struct fuse_instance
{
    fuse_pool_t *pool;                 ///< The memory pool for the application
    fuse_flag_t flags;                 ///< The flags which affect the behaviour of the application
    int exit_code;                     ///< Exit code of the application
};

#endif
