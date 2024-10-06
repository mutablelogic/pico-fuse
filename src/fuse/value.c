#include <fuse/fuse.h>
#include "alloc.h"
#include "fuse.h"

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS


/** @brief Determine if a value type is registered
 *
 * @param self The fuse instance
 * @param magic The value type
 * @return true if the value type is registered
 */
inline bool fuse_is_registered_value(fuse_t *self, uint16_t magic)
{
    assert(self);
    assert(magic < FUSE_MAGIC_COUNT);

    // We can only register a value descriptor once
    return (self->desc[magic].name == 0) ? false : true;
}

/** @brief Register value type
 *
 * @param self The fuse instance
 * @param desc The value descriptor
 */
void fuse_register_value_type(fuse_t *self, uint16_t magic, fuse_value_desc_t desc)
{
    assert(self);
    assert(magic < FUSE_MAGIC_COUNT);
    assert(desc.name);

    // We can only register a value descriptor once
    assert(self->desc[magic].name == 0);

    // Register the value descriptor
    self->desc[magic] = desc;
}

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
fuse_value_t *fuse_retain(fuse_t *self, void *value)
{
    assert(self);
    assert(value == NULL || fuse_allocator_magic(self->allocator, (fuse_value_t *)value) < FUSE_MAGIC_COUNT);

    // Retain value
    if (value != NULL)
    {
        fuse_allocator_retain(self->allocator, (fuse_value_t *)value);
    }

    // Return value
    return (fuse_value_t *)value;
}

/** @brief Release a value and destroy it if the reference count reaches 0
 */
void fuse_release(fuse_t *self, void *value)
{
    assert(self);
    assert(value == NULL || fuse_allocator_magic(self->allocator, (fuse_value_t *)value) < FUSE_MAGIC_COUNT);

    // Decrement the reference count
    if (value != NULL)
    {
        if (fuse_allocator_release(self->allocator, (fuse_value_t *)value))
        {
            // Indicate we should drain the memory pool
            self->drain = true;
        }
    }
}


/** @brief Check type of a value
 *
 * This method returns the value type, or 0 if the value is not
 * a valid value or a NULL value.
 *
 * @param self The fuse instance
 * @param value The value
 * @return The magic number of the value
 */
uint16_t fuse_value_type(fuse_t *self, void *value) {
    assert(self);

    // If value is NULL, then return NULL
    if (value == NULL) {
        return FUSE_MAGIC_NULL;
    }

    // TODO: We should check that this is a valid pointer value in the allocator pool
    // or else we could get a segfault - so return NULL

    // Get the magic number
    return fuse_allocator_magic(self->allocator, (fuse_value_t *)value);
}
