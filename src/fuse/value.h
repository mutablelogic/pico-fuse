/** @file value.h
 *  @brief Private function prototypes and structure definitions for values
 */
#ifndef FUSE_PRIVATE_VALUE_H
#define FUSE_PRIVATE_VALUE_H

#include <fuse/fuse.h>
#include <stdint.h>

/* @brief Represents an instance of a fuse value
 */
struct fuse_value
{
    union
    {
        // Scalar numbers
        uint8_t u8;
        uint16_t u16;
        uint32_t u32;
        uint64_t u64;
        int8_t s8;
        int16_t s16;
        int32_t s32;
        int64_t s64;
        float f32;
        double f64;

        // List and map element count
        uint32_t count;
    } data;

    union
    {
        struct fuse_value *head;
        struct fuse_value *prev;
    } l;

    union
    {
        struct fuse_value *tail;
        struct fuse_value *next;
    } r;
};

/* @brief Creates a new NULL value
 *
 * @param self The fuse instance
 * @param file The file name where the function was called
 * @param line The line number where the function was called
 * @return The new value or NULL if it can't be created
 */
fuse_value_t *fuse_value_new_null_ex(fuse_t *self, const char *file, int line);

/* @brief Creates a new memory block
 *
 * @param self The fuse instance
 * @param size The size of the memory block
 * @param file The file name where the function was called
 * @param line The line number where the function was called
 * @return The new value or NULL if it can't be created
 */
fuse_value_t *fuse_value_new_block_ex(fuse_t *self, size_t size, const char *file, int line);

/* @brief Creates a new list value
 *
 * @param self The fuse instance
 * @param file The file name where the function was called
 * @param line The line number where the function was called
 * @return A new empty list or NULL if it can't be created
 */
fuse_value_t *fuse_value_new_list_ex(fuse_t *self, const char *file, int line);

/* @brief Creates a new scalar value
 *
 * @param self The fuse instance
 * @param magic The magic number of the value
 * @param file The file name where the function was called
 * @param line The line number where the function was called
 * @return A new empty scalar value or NULL if it can't be created
 */
fuse_value_t *fuse_value_new_scalar_ex(fuse_t *self, uint16_t magic, const char *file, int line);

#endif
