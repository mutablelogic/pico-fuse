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
