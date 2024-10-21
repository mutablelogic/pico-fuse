#include <fuse/fuse.h>
#include "alloc.h"
#include "fuse.h"
#include "data.h"
#include "printf.h"

///////////////////////////////////////////////////////////////////////////////
// DECLARATIONS

static bool fuse_init_data(fuse_t *self, fuse_value_t *value, const void *user_data);
static size_t fuse_str_data(fuse_t *self, char *buf, size_t sz, size_t i, fuse_value_t *v, bool json);

///////////////////////////////////////////////////////////////////////////////
// LIFECYCLE

/** @brief Register type for a data values
 */
void fuse_register_value_data(fuse_t *self)
{
    assert(self);

    // Register type
    fuse_value_desc_t fuse_data_type = {
        .size = 0,
        .name = "DATA",
        .init = fuse_init_data,
        .str = fuse_str_data,
    };
    fuse_register_value_type(self, FUSE_MAGIC_DATA, fuse_data_type);
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE METHODS

/* @brief Initialise by copying memory
 */
static bool fuse_init_data(fuse_t *self, fuse_value_t *value, const void *user_data)
{
    assert(self);
    assert(value);

    uint16_t magic = fuse_allocator_magic(self->allocator, value);
    assert(magic < FUSE_MAGIC_COUNT);
    size_t size = self->desc[magic].size;

    // Copy or zero the memory
    if (user_data != NULL)
    {
        memcpy(value, user_data, size);
    }
    else
    {
        memset(value, 0, size);
    }

    // Return success
    return true;
}

/* @brief Output a data block as hex or base64
 *
 * Outputs as hexidecimal if json is false, otherwise as base64
 */
static size_t fuse_str_data(fuse_t *self, char *buf, size_t sz, size_t i, fuse_value_t *v, bool json)
{
    assert(self);
    assert(v);
    assert(fuse_allocator_magic(self->allocator, v) == FUSE_MAGIC_DATA);

    if (json)
    {
        return b64tostr_internal(buf, sz, i, v, fuse_allocator_size(self->allocator, v));
    }

    // If data size is zero, then return empty string
    size_t datasz = fuse_allocator_size(self->allocator, v);
    if (datasz == 0)
    {
        return i;
    }

    // Write hex data
    size_t j = 0;
    while (j < datasz)
    {
        i = utostr_internal(buf, sz, i, ((uint8_t *)v)[j++], FUSE_PRINTF_FLAG_HEX | FUSE_PRINTF_FLAG_UPPER | 2);
    }

    // Return the new index
    return i;
}
