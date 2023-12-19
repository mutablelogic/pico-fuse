
// Includes
#include <alloc/fuse.h>
#include "alloc.h"

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

void fuse_allocator_destroy(fuse_allocator_t *self)
{
    assert(self);
    self->destroy(self);
}

void *fuse_allocator_malloc(fuse_allocator_t *self, size_t size, uint16_t magic, const char *file, int line)
{
    assert(self);
    return self->malloc(self, size, magic, file, line);
}

void fuse_allocator_free(fuse_allocator_t *self, void *ptr)
{
    assert(self);
    self->free(self, ptr);
}

void *fuse_allocator_walk(fuse_allocator_t *self, void *ctx, fuse_allocator_walk_callback_t callback, void *user)
{
    assert(self);
    assert(callback);

    // Obtain the next unused block
    struct fuse_allocator_header *block = ctx ? ctx : self->head;
    while (block != NULL)
    {
        // Call the callback
        if (block->used)
        {
            callback(block->ptr, block->size,block->magic, block->file, block->line, user);
        }

        // Move to the next block
        block = block->next;
    }

    // Return the context
    return block;
}
