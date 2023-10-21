#include <stdlib.h>
#include <fuse/fuse.h>

// Private includes
#include "fuse.h"
#include "pool.h"

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

fuse_t *fuse_new(fuse_flag_t flags)
{
    // Create a memory pool - use zero for the size to use the default size
    fuse_pool_t *pool = fuse_pool_new(0, flags);
    if (pool == NULL)
    {
        return NULL;
    }

    // Allocate application instance in the memory pool
    fuse_t *self = fuse_pool_alloc(pool, sizeof(fuse_t));
    if (self == NULL)
    {
        fuse_pool_destroy(pool);
        return NULL;
    }

    // Set the instance properties
    self->pool = pool;
    self->flags = flags;
    self->exit_code = 0;

    // Return success
    return self;
}

int fuse_destroy(fuse_t *fuse)
{
    assert(fuse);

    // Store the exit code
    int exit_code = fuse->exit_code;

    // Free the application
    fuse_pool_free(fuse->pool, fuse);

    // Free the pool
    fuse_pool_destroy(fuse->pool);

    // Return the exit code
    return exit_code;
}

inline bool fuse_is(fuse_t *fuse, fuse_flag_t flag)
{
    // Always return false if the fuse argument is NULL
    return fuse ? (fuse->flags & flag) == flag : false;
}

inline void *fuse_alloc_ex(fuse_t *self, size_t size, const char *file, int line)
{
    assert(self);
    void *ptr = fuse_pool_alloc_ex(self->pool, size, file, line);
    if (ptr == NULL && fuse_is(self, FUSE_FLAG_DEBUG))
    {
        fuse_debugf(self, "memory allocation error allocating %lu bytes (%s:%d)\n", size, file, line);
    }
    return ptr;
}

inline void fuse_free(fuse_t *self, void *ptr)
{
    assert(self);
    assert(ptr);
    fuse_pool_free(self->pool, ptr);
}
