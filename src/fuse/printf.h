/** @file printf.h
 *  @brief Private function prototypes for printing formatted values
 */
#ifndef FUSE_PRIVATE_PRINTF_H
#define FUSE_PRIVATE_PRINTF_H

/* @brief Flags for printf functions
 */
#define FUSE_PRINTF_NULL "(null)"         ///< How a NULL value is printed
#define FUSE_PRINTF_NULL_JSON "null"      ///< How a NULL value is printed for JSON
#define FUSE_PRINTF_TRUE "true"           ///< How a true value is printed
#define FUSE_PRINTF_FALSE "false"         ///< How a false value is printed
#define FUSE_PRINTF_NAN "NaN"             ///< How a NaN float value is printed
#define FUSE_PRINTF_MINUS_INF "-Infinity" ///< How -ve infinity float value is printed
#define FUSE_PRINTF_PLUS_INF "Infinity"   ///< How +ve infinity float value is printed
#define FUSE_PRINTF_BUFFER_SIZE 80        ///< Default buffer size for printf functions

/* @brief Return the minimum of two values
 */
#define MIN(a, b) ((a) < (b) ? (a) : (b))

/* @brief Flags to modify the output format
 *
 * The first 16 bits are reserved for the number of digits to print
 * for hexadecimal values. The remaining bits are used for
 * flags that modify the output format.
 */
typedef enum
{
    FUSE_PRINTF_FLAG_LONG = 0x0100,    ///< Long integer
    FUSE_PRINTF_FLAG_UPPER = 0x0200,   ///< Uppercase hex digits
    FUSE_PRINTF_FLAG_ZEROPAD = 0x0400, ///< Pad with zeros
    FUSE_PRINTF_FLAG_BIN = 0x0800,     ///< Binary output
    FUSE_PRINTF_FLAG_HEX = 0x1000,     ///< Hexadecimal output
} fuse_printf_flags_t;

/* @brief Append a character to a string
 *
 * @param buf    Output buffer
 * @param sz     Size of the buffer, including the null terminator
 * @param i      Current index in the buffer
 * @param ch     Character to append
 * @returns      The new index in the buffer
 */
size_t chtoa_internal(char *buf, size_t sz, size_t i, const char ch);

/* @brief Append a null-terminated string to a string
 *
 * @param buf    Output buffer
 * @param sz     Size of the buffer, including the null terminator
 * @param i      Current index in the buffer
 * @param str    Null-terminated string to append
 * @returns      The new index in the buffer
 */
size_t cstrtoa_internal(char *buf, size_t sz, size_t i, const char *str);

/* @brief Append a null-terminated string to a string in quoted format
 *
 * @param buf    Output buffer
 * @param sz     Size of the buffer, including the null terminator
 * @param i      Current index in the buffer
 * @param str    Null-terminated string to append
 * @returns      The new index in the buffer
 */
size_t qstrtoa_internal(char *buf, size_t sz, size_t i, const char *str);

/* @brief Append an unsigned integer value to a string
 *
 * @param buf    Output buffer
 * @param sz     Size of the buffer, including the null terminator
 * @param i      Current index in the buffer
 * @param v      Value to append
 * @param flags  Flags to modify the output format
 * @returns      The new index in the output buffer
 */
size_t itoa_internal(char *buf, size_t sz, size_t i, int64_t v, fuse_printf_flags_t flags);

/* @brief Append a signed integer value to a string
 *
 * @param buf    Output buffer
 * @param sz     Size of the buffer, including the null terminator
 * @param i      Current index in the buffer
 * @param v      Value to append
 * @param flags  Flags to modify the output format
 * @returns      The new index in the output buffer
 */
size_t utoa_internal(char *buf, size_t sz, size_t i, uint64_t v, fuse_printf_flags_t flags);

/* @brief Append a float value to a string
 *
 * @param buf    Output buffer
 * @param sz     Size of the buffer, including the null terminator
 * @param i      Current index in the buffer
 * @param v      Value to append
 * @param flags  Flags to modify the output format
 * @returns      The new index in the output buffer
 */
size_t ftoa_internal(char *buf, size_t sz, size_t i, double v, fuse_printf_flags_t flags);

/* @brief Append a null-terminated string
 *
 * @param buf    Output buffer
 * @param sz     Size of the buffer, including the null terminator
 * @param i      Current index in the buffer
 * @param v      Value to append
 * @returns      The new index in the output buffer
 */
size_t cstrtoa_internal(char *buf, size_t sz, size_t i, const char *str);

/* @brief Append a value
 *
 * @param self   Fuse application
 * @param buf    Output buffer
 * @param sz     Size of the buffer, including the null terminator
 * @param i      Current index in the buffer
 * @param v      Value to append
 * @param quoted If true, the value is quoted for JSON output
 * @returns      The new index in the output buffer
 */
size_t vtoa_internal(fuse_t *self, char *buf, size_t sz, size_t i, fuse_value_t *v, bool quoted);

/* @brief Append a value to a buffer as a base64 string
 *
 * @param buf    Output buffer
 * @param sz     Size of the buffer, including the null terminator
 * @param i      Current index in the buffer
 * @param v      Value to append
 * @param len    The number of bytes
 * @returns      The new index in the output buffer
 */
size_t b64toa_internal(char *buf, size_t sz, size_t i, void *v, size_t len);

/* @brief Append a pointer value
 *
 * @param buf    Output buffer
 * @param sz     Size of the buffer, including the null terminator
 * @param i      Current index in the buffer
 * @param ptr    Value to append
 * @returns      The new index in the output buffer
 */
size_t ptoa_internal(char *buf, size_t sz, size_t i, void *ptr);

#endif
