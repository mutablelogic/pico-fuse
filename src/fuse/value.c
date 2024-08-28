
// Public headers
#include <fuse/fuse.h>

// Private headers
#include "alloc.h"
#include "fuse.h"

///////////////////////////////////////////////////////////////////////////////
// PRIVATE METHODS

void fuse_drain_callback(struct fuse_allocator_header *hdr, void *user)
{

    // Get retain count
    if (hdr->ref == 0)
    {
        // Log the drain
        fuse_debugf("DRAIN: %p magic %04X (%d bytes)", hdr->ptr, hdr->magic, hdr->size);
#ifdef DEBUG
        if (hdr->file != NULL)
        {
            fuse_debugf(" [allocated at %s:%d]", hdr->file, hdr->line);
        }
#endif
        fuse_debugf("\n");

        // Increment the count
        (*((size_t *)user))++;
    }
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

    // TODO

    return value;
}

/** @brief Release a value and destroy it if the reference count reaches 0
 */
void fuse_release(fuse_t *self, fuse_value_t *value)
{
    assert(self);
    assert(value);

    // TODO
}

/** @brief Drain the memory allocation pool
 */
size_t fuse_drain(fuse_t *self, size_t cap)
{
    assert(self);

    // Walk through any remaining memory blocks
#ifdef DEBUG
    void *ctx = NULL;
    size_t count = 0;
    while ((ctx = fuse_allocator_walk(self->allocator, ctx, fuse_drain_callback, &count)) != NULL)
    {
        // Do nothing
    }
#endif
    return count;
}
