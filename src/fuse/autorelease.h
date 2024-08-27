/** @file autorelease.h
 *  @brief Private function prototypes and structure definitions for autorelease pools
 */
#ifndef FUSE_PRIVATE_AUTORELEASE_H
#define FUSE_PRIVATE_AUTORELEASE_H

// Public includes
#include <fuse/fuse.h>

///////////////////////////////////////////////////////////////////////////////
// TYPES

/* @brief Represents a pool of autorelease values
 */
struct fuse_autorelease
{
    size_t cap;                    ///< The capacity of the pool
    fuse_value_t **values;         ///< The values in the pool
    struct fuse_autorelease *next; ///< The next autorelease pool
};

/* @brief Create a new autorelease pool
 *
 * @param  self The fuse application
 * @param  cap The initial capacity of the pool, must be greater than zero
 * @return The new autorelease pool, or NULL if an error occurred
 */
struct fuse_autorelease *fuse_autorelease_new(fuse_t *self, size_t cap);

/* @brief Release all values in the pool and the pool itself
 *
 * @param  self The fuse application
 * @param  pool The autorelease pool
 */
void fuse_autorelease_destroy(fuse_t *self, struct fuse_autorelease *pool);

/* @brief Drain the autorelease pool 
 * 
 * This will release all values in the pool which are not "owned" 
 *
 * @param  self The fuse application
 * @param  pool The autorelease pool
 * @return The number of values released
 */
size_t fuse_autorelease_drain(fuse_t *self, struct fuse_autorelease *pool);

/* @brief Add a value into the autorelease pool
 *
 * @param  self The fuse application
 * @param  pool The autorelease pool
 * @param  value The value to add
 * @return The value, or NULL if an error occurred
 */
fuse_value_t *fuse_autorelease_add(fuse_t *self, struct fuse_autorelease *pool, fuse_value_t *value);

#endif
