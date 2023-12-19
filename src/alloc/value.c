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

fuse_value_t *fuse_value_new_null_ex(fuse_t *self, const char *file, int line)
{
    assert(self);
    return fuse_alloc_ex(self, 0, FUSE_MAGIC_NULL, file, line);
}

fuse_value_t *fuse_value_new_block_ex(fuse_t *self, size_t size, const char *file, int line)
{
    assert(self);
    return fuse_alloc_ex(self, size, (uint16_t)(FUSE_MAGIC_BLOCK), file, line);
}

const char *fuse_value_cstr(fuse_t *self, fuse_value_t *value, char *buffer, size_t size)
{
    assert(self);
    assert(value);

    switch(fuse_allocator_magic(self->allocator, value)) {
        case (uint16_t)(FUSE_MAGIC_NULL):
            return "NULL";  
        default:
        return NULL;
    }
}
