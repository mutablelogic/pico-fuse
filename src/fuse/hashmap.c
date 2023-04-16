#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <fuse/fuse.h>
#include "hashmap.h"

///////////////////////////////////////////////////////////////////////////////
// STRUCTURES

struct hashmap_node_t
{
    void* voidp;
    void *value;
};

struct hashmap_instance_t
{
    size_t size;
    struct hashmap_node_t *nodes;
};

///////////////////////////////////////////////////////////////////////////////
// Create a new hashmap

/*
 * Create a new hashmap with a maximum size
 */
hashmap_t *hashmap_new(fuse_t *fuse, size_t size)
{
    assert(fuse);
    assert(size > 0);

    // Create the hashmap
    hashmap_t *hashmap = fuse_malloc(fuse, sizeof(hashmap_t));
    if (hashmap == NULL)
    {
        return NULL;
    }

    // Set the hashmap properties
    hashmap->size = size;
    hashmap->nodes = fuse_malloc(fuse, sizeof(struct hashmap_node_t) * size);
    if (hashmap->nodes == NULL)
    {
        return NULL;
    }

    // Zero the nodes
    memset(hashmap->nodes, 0, sizeof(struct hashmap_node_t) * size);

    // Return success
    return hashmap;
}

/*
 * Free hashmap resources
 */
void hashmap_delete(hashmap_t *hashmap)
{
    assert(hashmap);
    // Do nothing
}

/*
 * Return a hash for a void* key
 */
inline uint32_t hashmap_key_voidp(void *key)
{
    if (key == 0)
    {
        return 0;
    }
    uint32_t x = (uint32_t)key;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    assert(x);
    return x;
}

/*
 * Return a hash for (uint32_t, uint32_t) key)
 */
inline int32_t hashmap_key_int32int32(uint32_t x, uint32_t y)
{
    return (((x + y) * (x + y + 1)) / 2 + y);
}

/*
 * Get a value from the hashmap based on a void* key
 */
void *hashmap_get_voidp(hashmap_t *hashmap, void *key)
{
    assert(hashmap);
    assert(key);

    uint32_t hash = hashmap_key_voidp(key);
    uint32_t index = hash % hashmap->size;
    while (hashmap->nodes[index].voidp != 0)
    {
        if (hashmap->nodes[index].voidp == key)
        {
            return hashmap->nodes[index].value;
        }
        index = (index + 1) % hashmap->size;
    }

    // Key not found
    return NULL;
}

/*
 * Put a value from the hashmap based on a key
 */
bool hashmap_put_voidp(hashmap_t *hashmap, void *key, void *value)
{
    assert(hashmap);
    assert(key);

    // Find a spot for the key
    uint32_t hash = hashmap_key_voidp(key);
    uint32_t first = hash % hashmap->size;
    uint32_t index = first;
    while (hashmap->nodes[index].voidp != 0)
    {
        if (hashmap->nodes[index].voidp == key)
        {
            hashmap->nodes[index].value = value;
            return true;
        }
        index = (index + 1) % hashmap->size;

        // Check for a full table
        if(index == first) {
            return false;
        }
    }

    // Insert a new node
    hashmap->nodes[index].voidp = key;
    hashmap->nodes[index].value = value;

    // Return success
    return true;
}
