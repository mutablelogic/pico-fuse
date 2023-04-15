#include <stddef.h>
#include <assert.h>
#include <stdlib.h>

#include <fuse/fuse.h>

///////////////////////////////////////////////////////////////////////////////

// Alignment of pool memory on uint32_t boundary
#define FUSE_POOL_ALIGN 4

/*
 * Represents a memory pool of objects
 */
struct fuse_pool_instance
{
    void *mem;   // Memory pool
    size_t size; // Size of the memory pool
    size_t used; // Used bytes in the memory pool
};

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

/*
 * fuse_pool_new creates a new fixed-size memory pool
 *
 * @param size             The size of the memory pool
 * @return                 A pointer to the memory pool
 */
fuse_pool_t *fuse_pool_new(size_t size)
{
    assert(size > 0);

    // Allocate room for the pool structure
    fuse_pool_t *pool = (fuse_pool_t *)malloc(sizeof(fuse_pool_t));
    if (pool == NULL)
    {
        return NULL;
    }

    // Allocate the memory pool
    pool->mem = malloc(size);
    if (pool->mem == NULL)
    {
        free(pool);
        return NULL;
    }

    // Set the pool properties
    pool->size = size;
    pool->used = 0;

    // Return success
    return pool;
}

/*
 * fuse_pool_delete frees resources from a memory pool
 *
 * @param pool             The memory pool
 */
void fuse_pool_delete(fuse_pool_t *pool)
{
    assert(pool);
    free(pool->mem);
    free(pool);
}

/*
 * fuse_pool_alloc allocates a block of memory from the pool
 *
 * @param pool             The memory pool
 * @param size             The size of the memory block
 * @return                 A pointer to the memory block, or
 *                         NULL if the pool is full
 */
void *fuse_pool_alloc(fuse_pool_t *pool, size_t size)
{
    assert(pool);

    // Check size - if zero return NULL
    if (size == 0)
    {
        return NULL;
    }

    // Align size on boundary
    size = (size + FUSE_POOL_ALIGN - 1) & ~(FUSE_POOL_ALIGN - 1);
    if (pool->used + size > pool->size)
    {
        return NULL;
    }
    void *ptr = pool->mem + pool->used;
    pool->used += size;

    // Return success
    return ptr;
}
