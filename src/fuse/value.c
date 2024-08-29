
// Public headers
#include <fuse/fuse.h>

// Private headers
#include "alloc.h"
#include "fuse.h"

///////////////////////////////////////////////////////////////////////////////
// PRIVATE METHODS

void fuse_drain_callback(struct fuse_allocator_header *hdr, void *user)
{
    *(bool *)(user) = (hdr->ref == 0) ? true : false;
}

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

/** @brief Create a new autoreleased value
 */
fuse_value_t *fuse_new_value_ex(fuse_t *self, const uint16_t magic, const void *user_data, const char *file, const int line)
{
    assert(self);
    assert(magic < FUSE_MAGIC_COUNT);

    // Allocate memory for the value - retain count is zero
    return fuse_alloc_ex(self, magic, user_data, file, line);
}

/** @brief Retain the value and return it.
 */
fuse_value_t *fuse_retain(fuse_t *self, fuse_value_t *value)
{
    assert(self);
    assert(value);
    assert(fuse_allocator_magic(self->allocator, value) < FUSE_MAGIC_COUNT);

    // Retain value
    fuse_allocator_retain(self->allocator, value);

    // Return value
    return value;
}

/** @brief Release a value and destroy it if the reference count reaches 0
 */
void fuse_release(fuse_t *self, fuse_value_t *value)
{
    assert(self);
    assert(value);
    assert(fuse_allocator_magic(self->allocator, value) < FUSE_MAGIC_COUNT);

    // Release value
    if (fuse_allocator_release(self->allocator, value))
    {
        // Free the value
        fuse_free(self, value);
    }
}

/** @brief Drain the memory allocation pool
 */
size_t fuse_drain(fuse_t *self, size_t cap)
{
    assert(self);

    // Walk through any remaining memory blocks
    size_t count = 0;
    void *ctx = NULL;
    bool drain = false;
    fuse_printf(self, "DRAIN fuse=%p\n", self);
    while ((ctx = fuse_allocator_walk(self->allocator, ctx, fuse_drain_callback, &drain)) != NULL)
    {
        // Drain if the flag is set
        if (drain)
        {
            fuse_printf(self, "  DRAIN: %u %p\n", count + 1, ctx);
            // fuse_free(self, ctx);
            count++;
        }

        // Break if we have reached the cap
        if (count >= cap)
        {
            break;
        }
    }
    return count;
}
