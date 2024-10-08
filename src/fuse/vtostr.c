#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <fuse/fuse.h>
#include "alloc.h"
#include "fuse.h"
#include "printf.h"

/* @brief Append a value to a string
 */
size_t vtostr_internal(fuse_t *self, char *buf, size_t sz, size_t i, fuse_value_t *v, bool quoted)
{
    assert(self);
    assert(buf == NULL || sz > 0);
    assert(v);

    int16_t magic = fuse_allocator_magic(self->allocator, v);
    assert(magic < FUSE_MAGIC_COUNT);

    assert(self->desc[magic].str);
    return self->desc[magic].str(self, buf, sz, i, v, quoted);
}

/* @brief Convert a value to a string
 */
size_t vtostr(fuse_t *self, char *buf, size_t sz, fuse_value_t *v, bool quoted)
{
    assert(self);
    assert(v);
    assert(buf == NULL || sz > 0);

    size_t i = vtostr_internal(self, buf, sz, 0, v, quoted);

    // Terminate the string
    if (buf)
    {
        size_t k = MIN_(i, sz - 1);
        buf[k] = '\0';
    }

    // Return the number of characters that would have been written
    return i;
}
