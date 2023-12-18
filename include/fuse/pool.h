/** @file pool.h
 *  @brief Fuse memory pool management function prototypes
 *
 *  This file contains the function prototypes for creating and destroying
 *  memory pools, which are blocks of memory used for allocation of memory
 *  resources.
 */
#ifndef FUSE_POOL_H
#define FUSE_POOL_H

/** @brief Memory pool defintion
 */
typedef struct fuse_pool_instance fuse_pool_t;

/** @brief Create a new memory pool
 *
 *  @param size The size of the memory pool, in bytes
 *  @param flags The flags which affect the behaviour of the memory pool. Currently
 *               unused, but may be used in the future
 *  @returns A new memory pool, or NULL if memory could not be allocated
 */
fuse_pool_t *fuse_pool_new(size_t size, fuse_flag_t flags);

/** @brief Deallocate a fuse memory pool
 *
 *  @param fuse The fuse memory pool to deallocate
 */
void fuse_pool_destroy(fuse_pool_t *pool);

/** @brief Return statistics about the memory pool
 *
 *  @param pool The memory pool
 *  @param size A pointer to the total size of the memory pool, to be filled in or NULL
 *  @param used A pointer to the amount of memory used in the memory pool, to be filled in or NULL
 */
void fuse_pool_stats(fuse_pool_t *pool, size_t *size, size_t *used);

/** @brief Allocate a memory block in the memory pool
 *
 *  @param pool The memory pool
 *  @param size The size of the memory block to allocate
 *  @param file The file where the allocation was made
 *  @param line The line where the allocation was made
 *  @return A pointer to the allocated memory block, or NULL if no memory could be allocated
 */
void *fuse_pool_alloc_ex(fuse_pool_t *pool, size_t size, const char *file, int line);

#ifdef DEBUG
#define fuse_pool_alloc(pool, size) \
    (fuse_pool_alloc_ex((pool), (size), __FILE__, __LINE__))
#else
#define fuse_pool_alloc(pool, size) \
    (fuse_pool_alloc_ex((pool), (size), 0, 0))
#endif

/** @brief Free a memory block in the memory pool
 *
 *  @param pool The memory pool
 *  @param ptr A pointer to the memory block
 */
void fuse_pool_free(fuse_pool_t *pool, void *ptr);

#endif
