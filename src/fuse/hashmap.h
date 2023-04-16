
#ifndef HASHMAP_H
#define HASHMAP_H

#include <fuse/fuse.h>

/*
 * Represents a hashmap, which is a hash table that maps keys to values.
 */
typedef struct hashmap_instance_t hashmap_t;

/*
 * Create a new hashmap with a maximum size
 *
 * @param fuse              The fuse instance
 * @param size              The maximum size of the hashmap
 * @return                  A pointer to the hashmap
 */
hashmap_t *hashmap_new(fuse_t *fuse, size_t size);

/*
 * Free hashmap resources
 *
 * @param hashmap           The hashmap instance
 */
void hashmap_delete(hashmap_t *hashmap);

/*
 * Get a value from the hashmap based on a void* key
 *
 * @param hashmap           The hashmap instance
 * @param key               The key to search for
 * @return                  The value associated with the key, or NULL if not found
 */
void *hashmap_get_voidp(hashmap_t *hashmap, void *key);

/*
 * Put a value from the hashmap based on a key
 *
 * @param hashmap           The hashmap instance
 * @param key               The key used to store the entry
 * @param value             The value to store. If the key already exists, 
 *                          the existing value will be overwritten
 * @return                  Returns true if the value was stored. Usually a
 *                          false return indicates that the hashmap is full
 */
bool hashmap_put_voidp(hashmap_t *hashmap, void *key, void *value);

#endif
