
// Includes
#include <fuse/fuse.h>
#include "alloc.h"

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

inline void fuse_allocator_destroy(struct fuse_allocator *self)
{
    assert(self);
    self->destroy(self);
}

inline void *fuse_allocator_malloc(struct fuse_allocator *self, size_t size, uint16_t magic, const char *file, int line)
{
    assert(self);
    return self->malloc(self, size, magic, file, line);
}

inline void fuse_allocator_free(struct fuse_allocator *self, void *ptr)
{
    assert(self);
    assert(ptr);
    self->free(self, ptr);
}

inline uint16_t fuse_allocator_magic(struct fuse_allocator *self, void *ptr)
{
    assert(self);
    assert(ptr);
    return self->magic(self, ptr);
}

inline size_t fuse_allocator_size(struct fuse_allocator *self, void *ptr)
{
    assert(self);
    assert(ptr);
    return self->size(self, ptr);
}

inline void fuse_allocator_retain(struct fuse_allocator *self, void *ptr)
{
    assert(self);
    assert(ptr);
    self->retain(self, ptr);
}

inline bool fuse_allocator_release(struct fuse_allocator *self, void *ptr)
{
    assert(self);
    assert(ptr);
    return self->release(self, ptr);
}

void *fuse_allocator_walk(struct fuse_allocator *self, void *ctx, fuse_allocator_walk_callback_t callback, void *user)
{
    assert(self);
    assert(callback);

    fuse_debugf("Walking memory pool ctx=%p\n", ctx);

    // Obtain the next unused block
    if (ctx == NULL)
    {
        ctx = self->head;
        fuse_debugf("Walking returning first ctx=%p\n", ctx);
        return ctx;
    }

    // Call the callback on this header
    struct fuse_allocator_header *block = ctx;
    fuse_debugf("Walking callback ctx=%p\n", ctx);
    callback(block, user);

    // Return the next block
    fuse_debugf("Walking next ctx=%p\n", block->next);
    return block->next;
}
