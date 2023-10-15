#include <stdlib.h>
#include <fuse/fuse.h>

// Private includes
#include "pool.h"

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

/*
 * Create a new memory pool
 */
fuse_pool_t *fuse_pool_new(size_t size, fuse_flag_t flags)
{
    assert(size > 0);

    // Allocate a single block of memory which includes both the
    // memory pool and the required memory
    fuse_pool_t *self = (fuse_pool_t *)malloc(sizeof(struct fuse_pool_instance) + size);
    if (self == NULL)
    {
        return NULL;
    }

    // Set the instance properties
    self->size = size + sizeof(struct fuse_pool_instance);
    self->used = sizeof(struct fuse_pool_instance);
    self->flags = flags;
    self->next = NULL;

    // Return success
    return self;
}

/*
 * Deallocate a fuse memory pool
 */
void fuse_pool_destroy(fuse_pool_t *pool)
{
    assert(pool);

    // Free the memory pool
    free(pool);
}

/*
 * Return statistics about the memory pool
 * 
 * @param pool The memory pool
 * @param size A pointer to the total size of the memory pool, to be filled in or NULL
 * @param used A pointer to the amount of memory used in the memory pool, to be filled in or NULL
 */
void fuse_pool_stats(fuse_pool_t *pool, size_t *size, size_t *used)
{
    assert(pool);

    if (size != NULL)
    {
        *size = pool->size;
    }

    if (used != NULL)
    {
        *used = pool->used;
    }
}


/*
 * Allocate a memory block in the memory pool
 * 
 * @param pool The memory pool
 * @param size The size of the memory block to allocate
 * @return A pointer to the allocated memory block, or NULL if no memory could be allocated
 */
void* fuse_pool_alloc(fuse_pool_t *pool, size_t size) {
    assert(pool);

    void* next = pool->next;
    if (next == NULL) {
        // Allocate the first block
        struct fuse_pool_header* header = pool + sizeof(struct fuse_pool_instance);
        header->ptr = header + sizeof(struct fuse_pool_header);
        header->size = size;
        header->used = true;
        header->next = NULL;

        // Increment the pool state
        pool->next = header;
        pool->used += size + sizeof(struct fuse_pool_header);

        // Return pointer to the allocated memory
        return header->ptr;
    }

    // Return error
    return NULL;
}

/*
 * Free a memory block in the memory pool
 * 
 * @param pool The memory pool
 * @param ptr A pointer to the memory block
 */
void fuse_pool_free(void *ptr) {
    assert(ptr);

    // TODO
}
