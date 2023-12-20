/** @file fuse.h
 *  @brief Private function prototypes and structure definitions for fuse
 */
#ifndef FUSE_PRIVATE_FUSE_H
#define FUSE_PRIVATE_FUSE_H

#include <stdbool.h>
#include <fuse/alloc.h>

/* @brief Represents an instance of a fuse application
 */
struct fuse_application
{
    fuse_allocator_t *allocator; ///< The allocator for the application
    int exit_code;               ///< Exit code of the application
};

// Destroy callback
void fuse_destroy_callback(void *ptr, size_t size, uint16_t magic, const char *file, int line, void *user);

#endif
