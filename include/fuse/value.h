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
#define fuse_value_new_null(self) \
    (fuse_value_new_ex((self), FUSE_MAGIC_NULL, 0, __FILE__, __LINE__))
#define fuse_value_new_list(self) \
    (fuse_value_new_ex((self), FUSE_MAGIC_LIST, 0, __FILE__, __LINE__))
#define fuse_value_new_map(self, cap) \
    (fuse_value_new_ex((self), FUSE_MAGIC_MAP, (cap), __FILE__, __LINE__))
#define fuse_value_new_u8(self) \
    (fuse_value_new_ex((self), FUSE_MAGIC_U8, 0, __FILE__, __LINE__))
#define fuse_value_new_u16(self) \
    (fuse_value_new_ex((self), FUSE_MAGIC_U16, 0, __FILE__, __LINE__))
#define fuse_value_new_u32(self) \
    (fuse_value_new_ex((self), FUSE_MAGIC_U32, 0, __FILE__, __LINE__))
#define fuse_value_new_u64(self) \
    (fuse_value_new_ex((self), FUSE_MAGIC_U64, 0, __FILE__, __LINE__))
#define fuse_value_new_s8(self) \
    (fuse_value_new_ex((self), FUSE_MAGIC_S8, 0, __FILE__, __LINE__))
#define fuse_value_new_s16(self) \
    (fuse_value_new_ex((self), FUSE_MAGIC_S16, 0, __FILE__, __LINE__))
#define fuse_value_new_s32(self) \
    (fuse_value_new_ex((self), FUSE_MAGIC_S32, 0, __FILE__, __LINE__))
#define fuse_value_new_s64(self) \
    (fuse_value_new_ex((self), FUSE_MAGIC_S64, 0, __FILE__, __LINE__))
#define fuse_value_new_f32(self) \
    (fuse_value_new_ex((self), FUSE_MAGIC_F32, 0, __FILE__, __LINE__))
#define fuse_value_new_f64(self) \
    (fuse_value_new_ex((self), FUSE_MAGIC_F64, 0, __FILE__, __LINE__))
#define fuse_value_new_bool(self) \
    (fuse_value_new_ex((self), FUSE_MAGIC_BOOL, 0, __FILE__, __LINE__))
#else
#define fuse_value_new(self, magic, size) \
    (fuse_value_new_ex((self), (magic), (size), 0, 0))
#define fuse_value_new_null(self) \
    (fuse_value_new_ex((self), FUSE_MAGIC_NULL, 0, 0, 0))
#define fuse_value_new_list(self) \
    (fuse_value_new_ex((self), FUSE_MAGIC_LIST, 0, 0, 0))
#define fuse_value_new_map(self, cap) \
    (fuse_value_new_ex((self), FUSE_MAGIC_MAP, (cap), 0, 0))
#define fuse_value_new_u8(self) \
    (fuse_value_new_ex((self), FUSE_MAGIC_U8, 0, 0, 0))
#define fuse_value_new_u16(self) \
    (fuse_value_new_ex((self), FUSE_MAGIC_U16, 0, 0, 0))
#define fuse_value_new_u32(self) \
    (fuse_value_new_ex((self), FUSE_MAGIC_U32, 0, 0, 0))
#define fuse_value_new_u64(self) \
    (fuse_value_new_ex((self), FUSE_MAGIC_U64, 0, 0, 0))
#define fuse_value_new_s8(self) \
    (fuse_value_new_ex((self), FUSE_MAGIC_S8, 0, 0, 0))
#define fuse_value_new_s16(self) \
    (fuse_value_new_ex((self), FUSE_MAGIC_S16, 0, 0, 0))
#define fuse_value_new_s32(self) \
    (fuse_value_new_ex((self), FUSE_MAGIC_S32, 0, 0, 0))
#define fuse_value_new_s64(self) \
    (fuse_value_new_ex((self), FUSE_MAGIC_S64, 0, 0, 0))
#define fuse_value_new_f32(self) \
    (fuse_value_new_ex((self), FUSE_MAGIC_F32, 0, 0, 0))
#define fuse_value_new_f64(self) \
    (fuse_value_new_ex((self), FUSE_MAGIC_F64, 0, 0, 0))
#define fuse_value_new_bool(self) \
    (fuse_value_new_ex((self), FUSE_MAGIC_BOOL, 0, 0, 0))
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

/** @brief Return the count of elements in a list or map value
 *
 *  The function will return the number of elements in a list or map value. If the value is not a
 *  list or map, the function will panic.
 * 
 *  @param self The fuse instance
 *  @param value The value
 *  @returns The number of elements in a list or map value
 */
uint32_t fuse_value_count(fuse_t *self, fuse_value_t *value);

/** @brief Append an element to the end of a list
 *
 *  The function will append a element to the end of a list. If adding the element would
 *  create a circular reference, the function will panic. If the value is not a list, the
 *  function will panic.
 * 
 *  @param self The fuse instance
 *  @param value The value
 *  @param element The element to append
 */
void fuse_value_append(fuse_t *self, fuse_value_t *value, fuse_value_t *element);

/** @brief Prepend an element to the beginning of a list 
 *
 *  The function will prepend a element to the beginning of a list. If adding the element would
 *  create a circular reference, the function will panic. If the value is not a list, the
 *  function will panic.
 * 
 *  @param self The fuse instance
 *  @param value The value
 *  @param element The element to prepend
 */
void fuse_value_prepend(fuse_t *self, fuse_value_t *value, fuse_value_t *element);

#endif /* FUSE_VALUE_H */
