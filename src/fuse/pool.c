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
    // Allocate a single block of memory which includes both the
    // memory pool and the required memory
    fuse_pool_t *self = (fuse_pool_t *)malloc(sizeof(struct fuse_pool_instance));
    if (self == NULL)
    {
        return NULL;
    }

    // Set the instance properties
    self->size = size;
    self->flags = flags;
    self->used = 0;
    self->first = NULL;
    self->last = NULL;

    // Set the alloc and free functions
    self->alloc = fuse_pool_std_alloc;
    self->free = fuse_pool_std_free;

    // Return success
    return self;
}

/*
 * Deallocate a fuse memory pool
 */
void fuse_pool_destroy(fuse_pool_t *pool)
{
    assert(pool);

    // Free the elements in the memory pool
    struct fuse_pool_header *header = pool->first;
    while (header != NULL)
    {
        pool->free(pool, header);
        header = header->next;
    }

    // Free the memory pool
    free(pool);
}

/*
 * Return statistics about the memory pool
 * 
 * @param pool The memory pool
 * @param size A pointer to the total size of the memory pool, to be filled in or NULL. Sets to
 *            zero if the memory pool is unbounded
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

    // Allocate the memory block
    struct fuse_pool_header *header = pool->alloc(pool, size);
    if (header == NULL) {
        return NULL;
    }

    // TODO: Link memory block into the linked list of memory blocks

    // Return pointer to allocation
    return header->ptr;
}

/*
 * Free a memory block in the memory pool
 * 
 * @param pool The memory pool
 * @param ptr A pointer to the memory block
 */
void fuse_pool_free(fuse_pool_t *pool, void *ptr) {
    assert(pool);

    // If the pointer is NULL, then return
    if(ptr == NULL) {
        return;
    }

    // Obtain the header for the memory block

    // TODO
}
