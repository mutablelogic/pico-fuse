
// Includes
#include <fuse/fuse.h>
#include "alloc.h"

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

inline void fuse_allocator_destroy(fuse_allocator_t *self)
{
    assert(self);
    self->destroy(self);
}

inline void *fuse_allocator_malloc(fuse_allocator_t *self, size_t size, uint16_t magic, const char *file, int line)
{
    assert(self);
    return self->malloc(self, size, magic, file, line);
}

inline void fuse_allocator_free(fuse_allocator_t *self, void *ptr)
{
    assert(self);
    assert(ptr);
    self->free(self, ptr);
}

inline uint16_t fuse_allocator_magic(fuse_allocator_t *self, void *ptr)
{
    assert(self);
    assert(ptr);
    return self->magic(self, ptr);
}

inline size_t fuse_allocator_size(fuse_allocator_t *self, void *ptr)
{
    assert(self);
    assert(ptr);
    return self->size(self, ptr);
}

inline void fuse_allocator_retain(fuse_allocator_t *self, void *ptr)
{
    assert(self);
    assert(ptr);
    self->retain(self, ptr);
}

inline bool fuse_allocator_release(fuse_allocator_t *self, void *ptr)
{
    assert(self);
    assert(ptr);
    return self->release(self, ptr);
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
        callback(block, user);

        // Move to the next block
        block = block->next;
    }

    // Return the context
    return block;
}
