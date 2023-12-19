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

#ifdef DEBUG
#define fuse_value_new(self, magic, size) \
    (fuse_value_new_ex((self), (magic), (size), __FILE__, __LINE__))
#else
#define fuse_value_new(self, magic, size) \
    (fuse_value_new_ex((self), (magic), (size), 0, 0))
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
fuse_value_t *fuse_value_new_ex(fuse_t *self, uint16_t magic, size_t size, const char *file, int line);

/** @brief Retain the value and return it.
 *
 * This method increments the reference count of the value, so that when released, the
 * value is not destroyed, but the reference is decremented.
 *
 * @param self The fuse instance
 * @param ptr The value to retain
 * @return The retained value
 */
fuse_value_t *fuse_value_retain(fuse_t *self, fuse_value_t *value);

/** @brief Release a value and destroy it if the reference count reaches 0
 *
 * This method decrements the reference count of the value. When it reaches zero, it is
 * freed.
 *
 * @param self The fuse instance
 * @param ptr The value to release
 * @return The released value. Returns NULL if the memory is freed
 */
fuse_value_t *fuse_value_release(fuse_t *self, fuse_value_t *value);

/** @brief Convert a value to a zero-terminated string
 *
 *  @param self The fuse instance
 *  @param value The value
 *  @param buffer The buffer to write the string to
 *  @param size The size of the buffer
 *  @returns A zero-terminated string representation of the magic number, or NULL if the value could
 *          not be converted
 */
const char *fuse_value_cstr(fuse_t *self, fuse_value_t *value, char *buffer, size_t size);

#endif /* FUSE_VALUE_H */
