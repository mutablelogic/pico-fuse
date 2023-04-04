#include <stdlib.h>
#include "hashmap.h"

///////////////////////////////////////////////////////////////////////////////
// STRUCTURES

struct hashmap_node_t
{
    int key;
    void *value;
};

struct hashmap_instance_t
{
    int size;
    struct hashmap_node_t **nodes;
};

///////////////////////////////////////////////////////////////////////////////
// Create a new hashmap

hashmap_t *hashmap_init(int size)
{
    hashmap_t *hashmap = malloc(sizeof(hashmap_t));
    if (hashmap == NULL)
    {
        return NULL;
    }
    hashmap->size = size;
    hashmap->nodes = malloc(sizeof(struct hashmap_node_t *) * size);
    if (hashmap->nodes == NULL)
    {
        free(hashmap);
        return NULL;
    }
    for (int i = 0; i < size; i++)
    {
        hashmap->nodes[i] = NULL;
    }
    return hashmap;
}

///////////////////////////////////////////////////////////////////////////////
// Free hashmap

void hashmap_free(hashmap_t *hashmap)
{
    for (int i = 0; i < hashmap->size; i++)
    {
        if (hashmap->nodes[i] != NULL)
        {
            free(hashmap->nodes[i]);
        }
    }
    free(hashmap->nodes);
    free(hashmap);
}

///////////////////////////////////////////////////////////////////////////////
// Return a key from the state and event

inline int hashmap_key(hashmap_t *hashmap,int x, int y)
{
    return ((x + y) * (x + y + 1)) / 2 + y;
}

///////////////////////////////////////////////////////////////////////////////
// Return a value from the hashmap or NULL if not found

void *hashmap_get(hashmap_t *hashmap, int state, int event)
{
    int key = hashmap_key(hashmap, state, event);
    int index = key % hashmap->size;
    while (hashmap->nodes[index] != NULL) {
        if (hashmap->nodes[index]->key == key) {
            return hashmap->nodes[index]->value;
        }
        index = (index + 1) % hashmap->size;
    }

    // Key not found
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Put a value into the hashmap, returns 0 on success or -1 on error

int hashmap_put(hashmap_t *hashmap, int state, int event, void *value)
{
    if(value == NULL) {
        return -1;
    }

    // Find a spot for the key
    int key = hashmap_key(hashmap, state, event);
    int index = key % hashmap->size;
    while (hashmap->nodes[index] != NULL) {
        if (hashmap->nodes[index]->key == key) {
            hashmap->nodes[index]->value = value;
            return 0;
        }
        index = (index + 1) % hashmap->size;
    }

    // Insert a new node
    hashmap->nodes[index] = malloc(sizeof(struct hashmap_node_t));
    if (hashmap->nodes[index] == NULL)
    {
        return -1;
    }
    hashmap->nodes[index]->key = key;
    hashmap->nodes[index]->value = value;

    // Return success
    return 0;
}
