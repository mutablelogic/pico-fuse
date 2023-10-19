#ifndef FUSE_PRIVATE_POOL_H
#define FUSE_PRIVATE_POOL_H

#include <fuse/fuse.h>

/*
 * @brief Represents a memory block header
 */
struct fuse_pool_header
{
    void *ptr;                     ///< A pointer to the memory block
    size_t size;                   ///< The size of the memory block, in bytes
    bool used;                     ///< Whether the memory block is used
    struct fuse_pool_header *prev; ///< The previous memory block header, or NULL if this is the first memory block header
    struct fuse_pool_header *next; ///< The next memory block header, or NULL if this is the last memory block header
};

/*
 * @brief Represents an instance of a fuse memory pool
 */
struct fuse_pool_instance
{
    fuse_flag_t flags;              ///< The flags which affect the behaviour of the memory pool
    size_t size;                    ///< The size of the memory pool, in bytes. If zero, then memory pool is unbounded
    size_t used;                    ///< The amount of memory used in the memory pool, in bytes
    struct fuse_pool_header *first; ///< A pointer to the first memory block header, or NULL
    struct fuse_pool_header *last;  ///< A pointer to the last memory block header, or NULL

    // Allocate and free implementations
    struct fuse_pool_header *(*alloc)(fuse_pool_t *self, size_t size); ///< The memory allocator function
    void (*free)(fuse_pool_t *self, struct fuse_pool_header *header);  ///< The memory deallocator function
};

/*
 * Allocator function which is implemented directly by malloc
 *
 * @param self The memory pool
 * @param size The size of the memory block to allocate
 * @return A pointer to the allocated memory block, or NULL if no memory could be allocated
 */
struct fuse_pool_header *fuse_pool_std_alloc(fuse_pool_t *self, size_t size);

/*
 * Deallocator function which is implemented directly by free
 *
 * @param self The memory pool
 * @param ptr A pointer to the memory block to deallocate
 */
void fuse_pool_std_free(fuse_pool_t *self, struct fuse_pool_header *ptr);

#endif
