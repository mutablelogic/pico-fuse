#ifndef FUSE_PRIVATE_POOL_H
#define FUSE_PRIVATE_POOL_H

#include <fuse/fuse.h>

/*
 * @brief Represents an instance of a fuse memory pool
 */
struct fuse_pool_instance
{
    fuse_flag_t flags; ///< The flags which affect the behaviour of the memory pool
    size_t size;       ///< The size of the memory pool, in bytes
    size_t used;       ///< The amount of memory used in the memory pool, in bytes
    void* next;        ///< A pointer to the first memory block header, or NULL if no memory has been allocated
};

/*
 * @brief Represents a memory block header
 */
struct fuse_pool_header
{
    void* ptr;   ///< A pointer to the memory block
    size_t size; ///< The size of the memory block, in bytes
    bool used;   ///< Whether the memory block is used, or free
    void* next;  ///< A pointer to the next memory block header, or NULL if this is the last memory block header
};

#endif
