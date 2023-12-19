#include <stdint.h>
#include <alloc/fuse.h>
#include "value.h"
#include "fuse.h"

fuse_value_t *fuse_value_new_ex(fuse_t *self, uint16_t magic, size_t size, const char *file, int line)
{
    assert(self);
    switch (magic)
    {
    case (uint16_t)(FUSE_MAGIC_NULL):
        return fuse_value_new_null_ex(self, file, line);
    case (uint16_t)(FUSE_MAGIC_BLOCK):
        return fuse_value_new_block_ex(self, size, file, line);
    default:
        return NULL;
    }
}

inline fuse_value_t *fuse_value_new_null_ex(fuse_t *self, const char *file, int line)
{
    assert(self);
    return fuse_alloc_ex(self, 0, FUSE_MAGIC_NULL, file, line);
}

fuse_value_t *fuse_value_new_block_ex(fuse_t *self, size_t size, const char *file, int line)
{
    assert(self);
    return fuse_alloc_ex(self, size, (uint16_t)(FUSE_MAGIC_BLOCK), file, line);
}

fuse_value_t *fuse_value_new_list_ex(fuse_t *self, const char *file, int line) {
    assert(self);
}


const char *fuse_value_cstr(fuse_t *self, fuse_value_t *value, char *buffer, size_t size)
{
    assert(self);
    assert(value);

    switch(fuse_allocator_magic(self->allocator, value)) {
        case (uint16_t)(FUSE_MAGIC_NULL):
            return "null"; 
        default:
        return NULL;
    }
}

inline fuse_value_t *fuse_value_retain(fuse_t *self, fuse_value_t *value) {
    assert(self);
    assert(value);
    fuse_allocator_retain(self->allocator, value);
    return value;
}

inline fuse_value_t *fuse_value_release(fuse_t *self, fuse_value_t *value) {
    assert(self);
    assert(value);
    return fuse_allocator_release(self->allocator, value) ? NULL : value;
}

