/** @file value.h
 *  @brief Fuse values
 *
 *  This file contains methods for creating and releasing values
 */
#ifndef FUSE_VALUE_H
#define FUSE_VALUE_H

#include "fuse.h"
#include <stdint.h>

/** @brief The representation of a fuse value
 */
typedef struct fuse_value fuse_value_t;

/** @brief The representation of a fuse value descriptor
 */
typedef struct fuse_value_desc fuse_value_desc_t;

/** @brief Register value primitives
 *
 * @param self The fuse instance
 * @param desc The value descriptor
 */
void fuse_register_value_desc(const fuse_t *self, const fuse_value_desc_t *desc);

#ifdef DEBUG
#define fuse_value_new(self, magic, user_data) \
    (fuse_value_new_ex((self), (magic), (user_data), __FILE__, __LINE__))
#else
#define fuse_value_new(self, magic, user_data) \
    (fuse_value_new_ex((self), (magic), (user_data), 0, 0))
#endif

/** @brief Create a new zero-based value
 *
 * @param self The fuse instance
 * @param magic The magic number of the value
 * @param size The size of the value, which is ignored if the magic number is 0
 * @param file The file name of the caller
 * @param line The line number of the caller
 * @return The new value or NULL if the value could not be created
 */
fuse_value_t *fuse_value_new_ex(const fuse_t *self, const uint16_t magic, const void *user_data, const char *file, const int line);

/** @brief Retain the value and return it.
 *
 * This method increments the reference count of the value, to take ownership of the value.
 *
 * @param self The fuse instance
 * @param value The value to retain
 * @return The retained value, or NULL if the value could not be retained
 */
fuse_value_t *fuse_value_retain(fuse_t *self, fuse_value_t *value);

/** @brief Release a value and destroy it if the reference count reaches 0
 *
 * This method decrements the reference count of the value. When it reaches zero, it is
 * freed.
 *
 * @param self The fuse instance
 * @param ptr The value to release
 */
void fuse_value_release(fuse_t *self, fuse_value_t *value);

#endif /* FUSE_VALUE_H */
