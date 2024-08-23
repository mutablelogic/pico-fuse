/** @file fuse.h
 *  @brief Private function prototypes and structure definitions for fuse
 */
#ifndef FUSE_PRIVATE_FUSE_H
#define FUSE_PRIVATE_FUSE_H

#include <stdbool.h>
#include <fuse/alloc.h>
#include <fuse/value.h>

// Forward declaration
struct fuse_application;

///////////////////////////////////////////////////////////////////////////////
// TYPES

/* @brief Represents a value descriptor
 */
struct fuse_value_desc
{
    size_t size;
    const char *name;
    bool (*init)(struct fuse_application *self, fuse_value_t *value, const void *user_data);
    void (*destroy)(struct fuse_application *self, fuse_value_t *value);
    size_t (*cstr)(fuse_value_t *value,char *buffer, size_t size);
    size_t (*qstr)(fuse_value_t *value,char *buffer, size_t size);
};

/* @brief Represents an instance of a fuse application
 */
struct fuse_application
{
    fuse_allocator_t *allocator;                   ///< The allocator for the application
    struct fuse_value_desc desc[FUSE_MAGIC_COUNT]; ///< Value descriptors
    int exit_code;                                 ///< Exit code of the application
};

#endif
