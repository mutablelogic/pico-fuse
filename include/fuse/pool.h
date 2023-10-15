#ifndef FUSE_POOL_H
#define FUSE_POOL_H

/*
 * Represents a memory pool
 */
typedef struct fuse_pool_instance fuse_pool_t;

/*
 * Create a new memory pool
 *
 * @param size The size of the memory pool, in bytes
 * @param flags The flags which affect the behaviour of the memory pool. Currently
 *            unused, but may be used in the future
 * @returns A new memory pool, or NULL if memory could not be allocated
 */
fuse_pool_t *fuse_pool_new(size_t size, fuse_flag_t flags);

/*
 * Deallocate a fuse memory pool
 *
 * @param fuse The fuse memory pool to deallocate
 *
 */
void fuse_pool_destroy(fuse_pool_t *pool);

/*
 * Return statistics about the memory pool
 * 
 * @param pool The memory pool
 * @param size A pointer to the total size of the memory pool, to be filled in or NULL
 * @param used A pointer to the amount of memory used in the memory pool, to be filled in or NULL
 */
void fuse_pool_stats(fuse_pool_t *pool, size_t *size, size_t *used);

/*
 * Allocate a memory block in the memory pool
 * 
 * @param pool The memory pool
 * @param size The size of the memory block to allocate
 * @return A pointer to the allocated memory block, or NULL if no memory could be allocated
 */
void* fuse_pool_alloc(fuse_pool_t *pool, size_t size);

/*
 * Free a memory block in the memory pool
 * 
 * @param pool The memory pool
 * @param ptr A pointer to the memory block
 */
void fuse_pool_free(void *ptr);

#endif
