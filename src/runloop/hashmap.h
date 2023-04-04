
#ifndef HASHMAP_H
#define HASHMAP_H

// Opaque hasmap structure
typedef struct hashmap_instance_t hashmap_t;

// Initialize the hashmap structure
extern hashmap_t *hashmap_init(int size);

// Free runloop structure
extern void hashmap_free(hashmap_t *hashmap);

// Return a value from the hashmap or NULL if not found
extern void *hashmap_get(hashmap_t *hashmap, int state, int event);

// Put a value into the hashmap, returns 0 on success
extern int hashmap_put(hashmap_t *hashmap, int state, int event, void *data);

#endif
