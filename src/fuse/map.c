#include <string.h>
#include <fuse/fuse.h>

// Private includes
#include "map.h"

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

fuse_map_t *fuse_map_new_ex(fuse_t *fuse, size_t size, const char *file, int line)
{
    assert(fuse);
    assert(size > 0);

    // Create the map and the nodes in a single block of memory
    size_t alloc_size = sizeof(struct fuse_map_instance) + sizeof(struct fuse_map_node) * size;
    struct fuse_map_instance *instance = fuse_alloc_ex(fuse, alloc_size, file, line);
    if (instance == NULL)
    {
        return NULL;
    }

    // Set the instance properties
    memset(instance, 0, alloc_size);
    instance->size = size;
    instance->nodes = (struct fuse_map_node *)(instance + 1);

    // Return success
    return instance;
}

void fuse_map_destroy(fuse_t *fuse, fuse_map_t *self)
{
    assert(fuse);
    assert(self);

    // Free the instance
    fuse_free(fuse, self);
}

size_t fuse_map_stats(fuse_map_t *self, size_t *size)
{
    assert(self);
    if (size)
    {
        *size = self->size;
    }
    return self->count;
}

/** @brief This is the hash function for void* keys
 */
inline size_t fuse_map_hashfunc(void *key)
{
    if (key == 0)
    {
        return 0;
    }
    size_t x = (size_t)key;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    assert(x);
    return x;
}

/** @brief Set a key-value pair in the map
 *
 *  @param self The map
 *  @param key The key to insert. The key cannot be a NULL value.
 *  @param value The value to insert. If the value is NULL, then the key is removed from the map.
 *  @return True on success, or false if the map is full
 */
bool fuse_map_set(fuse_map_t *self, void *key, void *value)
{
    assert(self);
    assert(key);

    // TODO
}

/** @brief Get a value from the map
 *
 *  @param self The map
 *  @param key The key, cannot be a NULL value.
 *  @return The value. Returns NULL if the value does not exist
 */
void *fuse_map_get(fuse_map_t *self, void *key)
{
    assert(self);
    assert(key);

    // TODO
}
