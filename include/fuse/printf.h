/** @file printf.h
 *  @brief Print formatting functions
 */
#ifndef FUSE_PRINTF_H
#define FUSE_PRINTF_H

#include <stddef.h>
#include <stdbool.h>

/* @brief Return a quoted string
 *
 * Returns a quoted string value, suitable for JSON output, enclosed in double quotes.
 *
 * @param buf    The output buffer, or NULL to calculate the length only
 * @param sz     Size of the output buffer in bytes, including the null terminator
 * @param v      The value terminated by a zero byte, or NULL
 * @returns      The number of bytes that the value would require
 */
size_t qstrtostr(char *buf, size_t sz, const char *v);

/* @brief Return an double float value as a string
 *
 * @param buf    The output buffer, or NULL to calculate the length only
 * @param sz     Size of the output buffer in bytes, including the null terminator
 * @param v      The value
 * @returns      The number of bytes that the value would require
 */
size_t ftostr(char *buf, size_t sz, double v);

/* @brief Convert a value to a string
 *
 * Returns a value as a string, optionally quoted for JSON output
 *
 * @param buf    The output buffer, or NULL to calculate the length only
 * @param sz     Size of the output buffer in bytes, including the null terminator
 * @param v      The value
 * @param quoted If true, the value is quoted for JSON output
 * @returns      The number of bytes that the value would require
 */
size_t vtostr(fuse_t *self, char *buf, size_t sz, fuse_value_t *v, bool quoted);

/* @brief Return a signed integer value as a string
 *
 * @param buf    The output buffer, or NULL to calculate the length only
 * @param sz     Size of the output buffer in bytes, including the null terminator
 * @param v      The value
 * @returns      The number of bytes that the value would require
 */
size_t itostr(char *buf, size_t sz, int64_t v);

/* @brief Return an unsigned integer value as a string
 *
 * @param buf    The output buffer, or NULL to calculate the length only
 * @param sz     Size of the output buffer in bytes, including the null terminator
 * @param v      The value
 * @returns      The number of bytes that the value would require
 */
size_t utostr(char *buf, size_t sz, uint64_t v);

/* @brief Return an unsigned integer value as a string, formatted as binary
 *
 * @param buf    The output buffer, or NULL to calculate the length only
 * @param sz     Size of the output buffer in bytes, including the null terminator
 * @param v      The value
 * @param bits   The number of bits to format the value as, or zero for the minimum
 * @returns      The number of bytes that the value would require
 */
size_t utostr_binary(char *buf, size_t sz, uint64_t v, uint8_t bits);

/* @brief Return an unsigned integer value as a string, formatted as hexidecimal
 *
 * @param buf    The output buffer, or NULL to calculate the length only
 * @param sz     Size of the output buffer in bytes, including the null terminator
 * @param v      The value
 * @param bits   The number of bits to format the value as, or zero for the minimum.
 *               The number of bits must be a multiple of 4
 * @param upper  If true, use uppercase hex digits
 * @returns      The number of bytes that the value would require
 */
size_t utostr_hex(char *buf, size_t sz, uint64_t v, uint8_t bits, bool upper);

/* @brief Convert a value to a  base64 string
 * 
 * @param buf    The output buffer, or NULL to calculate the length only
 * @param sz     Size of the output buffer in bytes, including the null terminator
 * @param v      The value
 * @param len    The length of the value in bytes
 * @returns      The number of bytes that the value would require
 */
size_t b64tostr(char *buf, size_t sz, void *v, size_t len);

/* @brief Format a string into the output buffer, replacing formatting directives
 *
 * @param self       The fuse application
 * @param buffer     The buffer to write the formatted string to. If the parameter is
 *                   set to NULL, only the length of what would have been written
 *                   is returned
 * @param size       The size of the buffer, including the null terminator. If the
 *                   buffer is set to NULL, this parameter is ignored
 * @param format     The format string. See below for the list of supported directives
 * @param ...        The arguments to format
 * @returns          The number of characters written, not including the null terminator
 *
 * The following format directives are supported:
 * - %s:  NULL-terminated string
 * - %d:  Signed integer (int8_t, int16_t or int32_t)
 * - %u:  Unsigned integer (uint8_t, uint16_t or uint32_t)
 * - %ld: Signed long integer (int64_t)
 * - %lu: Unsigned long integer (uint64_t)
 * - %x:  Hexadecimal unsigned integer (lowercase)
 * - %X:  Hexadecimal unsigned integer (uppercase)
 * - %p:  Pointer value
 * - %f:  Floating point value (float or double)
 * - %v:  fuse_value_t*
 * - %q:  Quoted fuse_value_t* (in JSON format)
 * - %%:  Literal '%'
 */
size_t fuse_sprintf(fuse_t *self, char *buffer, size_t size, const char *format, ...);

/* @brief Print a formatted string to stdout, replacing formatting directives
 *
 * @param self       The fuse application
 * @param format     The format string. See below for the list of supported directives
 * @param ...        The arguments to format
 * @returns          The number of characters written, not including the null terminator
 *
 * The following format directives are supported:
 * - %s:  NULL-terminated string
 * - %d:  Signed integer (int8_t, int16_t or int32_t)
 * - %u:  Unsigned integer (uint8_t, uint16_t or uint32_t)
 * - %ld: Signed long integer (int64_t)
 * - %lu: Unsigned long integer (uint64_t)
 * - %x:  Hexadecimal unsigned integer (lowercase)
 * - %X:  Hexadecimal unsigned integer (uppercase)
 * - %p:  Pointer value
 * - %f:  Floating point value (float or double)
 * - %v:  fuse_value_t*
 * - %q:  Quoted fuse_value_t* (in JSON format)
 * - %%:  Literal '%'
 */
size_t fuse_printf(fuse_t *self, const char *format, ...);

/** @brief Prints formatted debugging messages in debug mode
 *
 * @param self       The fuse application
 * @param format     The format string. See below for the list of supported directives
 * @param ...        The arguments to format
 * @returns          The number of characters written, not including the null terminator
 */
size_t fuse_debugf(fuse_t *self, const char *format, ...);

#endif /* FUSE_PRINTF_H */
