/** @file pool_std.h
 *  @brief Private function prototypes and structure definitions for standard memory allocation functions
 *
 * The standard memory allocation functions are implemented directly by malloc and free.
 */
#ifndef FUSE_PRIVATE_POOL_STD_H
#define FUSE_PRIVATE_POOL_STD_H

#include <fuse/fuse.h>
#include "pool.h"

/** @brief Allocator function which is implemented directly by malloc
 *
 *  @param self The memory pool
 *  @param size The size of the memory block to allocate
 *  @return A pointer to the allocated memory block, or NULL if no memory could be allocated
 */
struct fuse_pool_header *fuse_pool_std_alloc(fuse_pool_t *self, size_t size);

/** @brief Deallocator function which is implemented directly by free
 *
 *  @param self The memory pool
 *  @param header A pointer to the memory block header to deallocate
 */
void fuse_pool_std_free(fuse_pool_t *self, struct fuse_pool_header *header);

/** @brief Mapping function from a void pointer to a memory pool header
 *
 *  @param self The memory pool
 *  @param ptr A pointer to the memory block to map
 *  @return The memory block header which is mapped from the pointer
 */
struct fuse_pool_header *fuse_pool_std_map(fuse_pool_t *self, void *ptr);

#endif
