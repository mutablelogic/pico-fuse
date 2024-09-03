#include <stdlib.h>
#include <fuse/fuse.h>

// Private includes
#include "pool.h"
#include "pool_std.h"

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

struct fuse_pool_header *fuse_pool_std_alloc(fuse_pool_t *self, size_t size)
{
    assert(self);

    // Calculate additional size needed, and check size
    size_t alloc_size = sizeof(struct fuse_pool_header) + size;
    if(self->size > 0 && self->used + alloc_size > self->size) {
        fuse_debugf(NULL, "DEBUG: in fuse_pool_std_alloc failed during size test self->size=%lu self->used=%lu alloc_size=%lu\n",self->size,self->used,alloc_size);
        return NULL;
    }

    // Allocate the memory block
    struct fuse_pool_header *header = (struct fuse_pool_header *)malloc(alloc_size);
    if (header == NULL)
    {
        fuse_debugf(NULL, "DEBUG: in fuse_pool_std_alloc failed during malloc\n");
        return NULL;
    }

    // Set the header properties
    header->ptr = (void *)header + sizeof(struct fuse_pool_header);
    header->size = alloc_size;
    header->used = true;
    header->prev = NULL;
    header->next = NULL;

    // Return success
    return header;
}

void fuse_pool_std_free(fuse_pool_t *self, struct fuse_pool_header *ptr)
{
    assert(self);
    assert(ptr);

    // Simple free
    free(ptr);
}

struct fuse_pool_header *fuse_pool_std_map(fuse_pool_t *self, void *ptr)
{
    assert(self);
    assert(ptr);

    return (struct fuse_pool_header *)ptr - 1;
}
