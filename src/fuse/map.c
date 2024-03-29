#include <string.h>
#include <fuse/fuse.h>

// Private includes
#include "map.h"

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

fuse_map_t *fuse_map_new_ex(fuse_t *fuse, size_t size, size_t (*hashfunc)(void *), const char *file, int line)
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
    instance->count = 0;
    instance->size = size;
    instance->nodes = (struct fuse_map_node *)(instance + 1);
    instance->hashfunc = hashfunc;

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

inline size_t fuse_map_stats(fuse_map_t *self, size_t *size)
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
size_t fuse_map_hashfunc(void *key)
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

    // Obtain the hash value for the key, and the index into the map
    size_t hash = self->hashfunc ? self->hashfunc(key) : fuse_map_hashfunc(key);
    size_t first = hash % self->size;
    size_t index = first;

    // Walk through the nodes in the map, starting at the index
    while (self->nodes[index].key != 0)
    {
        if (self->nodes[index].key == key && self->nodes[index].value != 0)
        {
            return self->nodes[index].value;
        }
        index = (index + 1) % self->size;
        if (index == first)
        {
            // Key not found
            return NULL;
        }
    }

    // Key not found
    return NULL;
}

/** @brief Set a value in the map
 *
 *  @param self The map
 *  @param key The key, cannot be a NULL value.
 *  @param value The value. If the value is NULL, then the key is deleted from the map.
 *  @return Returns false if the map is full.
 */
bool fuse_map_set(fuse_map_t *self, void *key, void *value)
{
    assert(self);
    assert(key);

    // Obtain the hash value for the key, and the index into the map
    size_t hash = self->hashfunc ? self->hashfunc(key) : fuse_map_hashfunc(key);
    size_t first = hash % self->size;
    size_t index = first;

    // Walk through the nodes in the map, starting at the index
    while (self->nodes[index].key != 0)
    {
        // If the key already exists or the value has been deleted, then set the value
        // and return success. If the value is NULL then effectively the key is deleted,
        // or else it is set.
        if (self->nodes[index].key == key)
        {
            if (self->nodes[index].value != value)
            {
                self->nodes[index].value = value;
                self->count += value ? 1 : -1;
            }
            return true;
        }

        // Move to the next node, wrapping around if necessary
        // If the index is the same as the first index, then the table is full
        // so return false, if the value is not null.
        index = (index + 1) % self->size;
        if (index == first)
        {
            return value ? false : true;
        }
    }

    // If the value is not null, then add the key to the map
    self->nodes[index].key = key;
    if (self->nodes[index].value != value)
    {
        self->nodes[index].value = value;
        self->count += value ? 1 : -1;
    }

    // Key not found
    return true;
}
