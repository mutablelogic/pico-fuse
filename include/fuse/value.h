/** @file value.h
 *  @brief Fuse values
 *
 *  This file contains methods for creating and releasing values
 */
#ifndef FUSE_VALUE_H
#define FUSE_VALUE_H

#include "fuse.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/** @brief The representation of a fuse value
 */
typedef struct fuse_value fuse_value_t;

/** @brief The representation of a fuse value descriptor
 */
typedef struct {
    size_t size;
    const char *name;
    bool (*init)(struct fuse_application *self, fuse_value_t *value, const void *user_data);
    void (*destroy)(struct fuse_application *self, fuse_value_t *value);
    size_t (*cstr)(struct fuse_application *self, char *buf, size_t sz, size_t i, fuse_value_t *v);
    size_t (*qstr)(struct fuse_application *self, char *buf, size_t sz, size_t i, fuse_value_t *v);
} fuse_value_desc_t;

/** @brief Register value type
 *
 * @param self The fuse instance
 * @param magic The magic number of the value type
 * @param type The value type descriptor
 */
void fuse_register_value_type(fuse_t *self, uint16_t magic, fuse_value_desc_t type);

#ifdef DEBUG
#define fuse_new_null(self) \
    (fuse_new_value_ex((self), (FUSE_MAGIC_NULL), (0), __FILE__, __LINE__))
#define fuse_new_data(self, sz) \
    (fuse_new_value_ex((self), (FUSE_MAGIC_DATA), (void *)(sz), __FILE__, __LINE__))
#define fuse_new_u8(self, u8) \
    (fuse_new_value_ex((self), (FUSE_MAGIC_U8), (void *)(uintptr_t)(u8), __FILE__, __LINE__))
#else
#define fuse_new_null(self) \
    (fuse_new_value_ex((self), (FUSE_MAGIC_NULL), (0), 0, 0))
#define fuse_new_data(self, sz) \
    (fuse_new_value_ex((self), (FUSE_MAGIC_DATA), (void *)(sz), 0, 0))
#define fuse_new_u8(self, u8) \
    (fuse_new_value_ex((self), (FUSE_MAGIC_U8), (void *)(uintptr_t)(u8), 0, 0))
#endif

/** @brief Create a new autoreleased value
 *
 *  This method creates a new value. The value is initialized with user_data, the behavior of which is
 *  determined by the magic number. The value is set to be automatically released, so to take ownership
 *  of the value, you must retain it with fuse_value_retain.
 *
 * @param self The fuse instance
 * @param magic The magic number of the value
 * @param size The size of the value, which is ignored if the magic number is 0
 * @param file The file name of the caller
 * @param line The line number of the caller
 * @return The new value or NULL if the value could not be created
 */
fuse_value_t *fuse_new_value_ex(fuse_t *self, const uint16_t magic, const void *user_data, const char *file, const int line);

/** @brief Retain the value and return it.
 *
 * This method increments the reference count of the value, to take ownership of the value.
 *
 * @param self The fuse instance
 * @param value The value to retain, which must be a fuse_value_t*
 * @return The retained value, or NULL if the value could not be retained
 */
fuse_value_t *fuse_retain(fuse_t *self, void *value);

/** @brief Release a value and destroy it if the reference count reaches 0
 *
 * This method decrements the reference count of the value. When it reaches zero, it is
 * freed.
 *
 * @param self The fuse instance
 * @param ptr The value to release, which must be a fuse_value_t*
 */
void fuse_release(fuse_t *self, void *value);

#endif /* FUSE_VALUE_H */
