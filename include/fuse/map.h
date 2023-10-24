/** @file map.h
 *  @brief Fuse map function prototypes
 *
 *  This file contains the function prototypes for creating and destroying
 *  maps. A map can be used to store key-value pairs, where both the key and
 *  the value are void pointers. In a map, all keys are unique.
 *
 *  The size argument provides the inital estimate on the number of key-value
 *  pairs that will be stored in the map. The map will automatically resize
 *  itself if the number of key-value pairs exceeds the initial estimate, but
 *  this will cause a performance penalty.
 */
#ifndef FUSE_MAP_H
#define FUSE_MAP_H
#include "fuse.h"

/** @brief Map defintion
 */
typedef struct fuse_map_instance fuse_map_t;

/** @brief Create a new empty map
 *  @param fuse The fuse application
 *  @param size The initial size of the map
 *  @returns A new empty map, or NULL if memory could not be allocated
 */
fuse_map_t *fuse_map_new_ex(fuse_t *fuse, size_t size, const char *file, int line);

#ifdef DEBUG
#define fuse_map_new(self, size) \
    (fuse_map_new_ex((self), (size), __FILE__, __LINE__))
#else
#define fuse_map_new(self, size) \
    (fuse_map_new_ex((self), (size), 0, 0))
#endif

/** @brief Deallocate a map
 *
 *  @param fuse The fuse application
 *  @param self The map to deallocate
 */
void fuse_map_destroy(fuse_t *fuse, fuse_map_t *self);

/** @brief Return statistics about the map
 * 
 *  Test for the number of elements in the map. If the size argument is not NULL,
 *  then the current maximum number of elements in the map is passed back. If this
 *  equals the count then the map is full.
 *
 *  @param self The map
 *  @param size A pointer to the current maximum number of elements in the map. If the size
 *              argument is NULL, the parameter is not passed back.
 *  @return The number of elements in the map
 */
size_t fuse_map_stats(fuse_map_t *self, size_t *size);

/** @brief Get a value from the map
 *
 *  @param self The map
 *  @param key The key, cannot be a NULL value.
 *  @return The value. Returns NULL if the value does not exist
 */
void *fuse_map_get(fuse_map_t *self, void *key);

/** @brief Set a key-value pair in the map
 *
 *  @param self The map
 *  @param key The key to insert. The key cannot be a NULL value.
 *  @param value The value to insert. If the value is NULL, then the key is removed from the map.
 *  @return True on success, or false if the map is full
 */
bool fuse_map_set(fuse_map_t *self, void *key, void *value);

#endif // FUSE_MAP_H
