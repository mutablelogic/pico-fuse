#include <stdlib.h>
#include <fuse/fuse.h>

// Private includes
#include "pool.h"
#include "pool_std.h"

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

fuse_pool_t *fuse_pool_new(size_t size, fuse_flag_t flags)
{
    // Allocate a memory for the instance
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
    self->map = fuse_pool_std_map;

    // Return success
    return self;
}

void fuse_pool_destroy(fuse_pool_t *pool)
{
    assert(pool);

    // Free the elements in the memory pool, until the
    // next element is NULL, indicating there are no more
    // allocated memory blocks
    struct fuse_pool_header *header = pool->first;
    while (header != NULL)
    {
        pool->free(pool, header);
        header = header->next;
    }

    // Free the memory pool
    free(pool);
}

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

void *fuse_pool_alloc_ex(fuse_pool_t *pool, size_t size, const char *file, int line)
{
    assert(pool);

    // Allocate the memory block using the expected function
    struct fuse_pool_header *header = pool->alloc(pool, size);
    if (header == NULL)
    {
        return NULL;
    }

    // First memory block
    if (pool->first == NULL)
    {
        pool->first = header;
    }

    // Last memory block
    if (pool->last == NULL)
    {
        header->prev = NULL;
    }
    else
    {
        // Set as the last header in the linked list
        pool->last->next = header;
        header->prev = pool->last;
    }
    header->next = NULL;
    pool->last = header;

    // Set file and line
    header->file = file;
    header->line = line;

    // Adjust used memory
    pool->used += header->size;

    // Return pointer to allocation
    return header->ptr;
}

/*
 * Free a memory block in the memory pool
 *
 * @param pool The memory pool
 * @param ptr A pointer to the memory block
 */
void fuse_pool_free(fuse_pool_t *pool, void *ptr)
{
    assert(pool);
    assert(ptr);

    // Map the pointer to a memory header
    struct fuse_pool_header *header = pool->map(pool, ptr);
    assert(header);

    // First memory block
    if (header->prev == NULL)
    {
        assert(pool->first == header);
        pool->first = header->next;
    }
    else
    {
        header->prev->next = header->next;
    }

    // Last memory block
    if (pool->last == header)
    {
        pool->last = header->prev;
    }
    else
    {
        header->next->prev = header->prev;
    }

    // Adjust used memory
    pool->used -= header->size;

    // Free the memory block
    pool->free(pool, header);
}
