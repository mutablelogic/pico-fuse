/** @file printf.h
 *  @brief Private function prototypes for printing formatted values
 */
#ifndef FUSE_PRIVATE_PRINTF_H
#define FUSE_PRIVATE_PRINTF_H

/* @brief Flags to modify the output format
 *
 * The first 16 bits are reserved for the number of digits to print
 * for hexadecimal values. The remaining bits are used for
 * flags that modify the output format.
 */
typedef enum
{
    FUSE_PRINTF_FLAG_LONG = 0x0100,  ///< Long integer
    FUSE_PRINTF_FLAG_UPPER = 0x0200, ///< Uppercase hex digits
} fuse_printf_flags_t;

/* @brief Append a signed integer value to a string
 *
 * @param out    The output buffer
 * @param n      Size of the output buffer, not including the null terminator
 * @param i      The current index in the output buffer
 * @param v      The value to append
 * @param base   Numeric base to use for the conversion
 * @param flags  Flags to modify the output format (currently ignored)
 * @returns      The new index in the output buffer
 */
size_t itoa(char *out, size_t n, size_t i, int64_t v, int base, fuse_printf_flags_t flags);

/* @brief Append an unsigned integer value to a string
 *
 * @param out    Output buffer
 * @param n      Size of the output buffer, not including the null terminator
 * @param i      Current index in the output buffer
 * @param v      Value to append
 * @param base   Numeric base to use for the conversion
 * @param flags  Flags to modify the output format
 * @returns      The new index in the output buffer
 */
size_t utoa(char *out, size_t n, size_t i, uint64_t v, int base, fuse_printf_flags_t flags);

/* @brief Append a null-terminated string
 *
 * @param out    The output buffer
 * @param n      Size of the output buffer, not including the null terminator
 * @param i      The current index in the output buffer
 * @param v      The value to append
 * @param flags  Flags to modify the output format (currently ignored)
 * @returns      The new index in the output buffer
 */
size_t stoa(char *out, size_t n, size_t i, const char *v, fuse_printf_flags_t flags);

/* @brief Append a null-terminated string in quoted format
 *
 * @param out    The output buffer
 * @param n      Size of the output buffer, not including the null terminator
 * @param i      The current index in the output buffer
 * @param v      The value to append
 * @param flags  Flags to modify the output format (currently ignored)
 * @returns      The new index in the output buffer
 */
size_t qtoa(char *out, size_t n, size_t i, const char *v, fuse_printf_flags_t flags);

#endif
