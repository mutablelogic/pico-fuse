
// Public includes
#include <fuse/fuse.h>

// Private includes
#include "fuse.h"
#include "alloc.h"
#include "autorelease.h"

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

/* @brief Create a new autorelease pool
 */
struct fuse_autorelease *fuse_autorelease_new(fuse_t *self, size_t cap)
{
    assert(self);
    assert(cap > 0);

    // Allocate an autorelease pool
    struct fuse_autorelease *pool = fuse_allocator_malloc(self->allocator, sizeof(struct fuse_autorelease), (uint16_t)(FUSE_MAGIC_POOL), NULL, 0);
    if (pool == NULL)
    {
        return NULL;
    }

    // Allocate the values in the pool
    pool->values = fuse_allocator_malloc(self->allocator, sizeof(fuse_value_t *) * cap, (uint16_t)(FUSE_MAGIC_DATA), NULL, 0);
    if (pool->values == NULL)
    {
        fuse_allocator_free(self->allocator, pool);
        return NULL;
    }

    // Initialize the autorelease pool
    pool->cap = cap;
    memset(pool->values, 0, sizeof(fuse_value_t *) * cap);
    pool->next = NULL;

    // Return success
    return pool;
}

/* @brief Drain the autorelease pool, and release the pool
 */
void fuse_autorelease_destroy(fuse_t *self, struct fuse_autorelease *pool)
{
    assert(self);
    assert(pool);
    assert(pool->values);

    // Release values
    for (size_t i = 0; i < pool->cap; i++)
    {
        if (pool->values[i] != NULL)
        {
            fuse_free(self, pool->values[i]);
        }
    }

    // Free the pool
    fuse_allocator_free(self->allocator, pool->values);
    fuse_allocator_free(self->allocator, pool);
}
