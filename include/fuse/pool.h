
#ifndef FUSE_POOL_H
#define FUSE_POOL_H

/*
 * Represents a memory pool
*/
typedef struct fuse_pool_instance fuse_pool_t;

/*
 * fuse_pool_new creates a new fixed-size memory pool
 *
 * @param size             The size of the memory pool
 * @return                 A pointer to the memory pool
 */
fuse_pool_t *fuse_pool_new(size_t size);

/*
 * fuse_pool_delete frees resources from a memory pool
 *
 * @param pool             The memory pool
 */
void fuse_pool_delete(fuse_pool_t *pool);

/*
 * fuse_pool_alloc allocates a block of memory from the pool
 *
 * @param pool             The memory pool
 * @param size             The size of the memory block
 * @return                 A pointer to the memory block, or
 *                         NULL if the pool is full
 */
void *fuse_pool_alloc(fuse_pool_t *pool, size_t size);

#endif
