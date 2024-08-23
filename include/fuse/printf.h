/** @file printf.h
 *  @brief Print formatting functions
 */
#ifndef FUSE_PRINTF_H
#define FUSE_PRINTF_H

#include <stddef.h>

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
 * - %c:  Character
 * - %s:  NULL-terminated string
 * - %d:  Signed integer (int8_t, int16_t or int32_t)
 * - %u:  Unsigned integer (uint8_t, uint16_t or uint32_t)
 * - %ld: Signed long integer (int64_t)
 * - %lu: Unsigned long integer (uint64_t)
 * - %x:  Hexadecimal unsigned integer (lowercase)
 * - %X:  Hexadecimal unsigned integer (uppercase)
 * - %p:  Pointer value
 * - %v:  fuse_value_t*
 * - %q:  Quoted fuse_value_t* (in JSON format)
 * - %%:  Literal '%'
 */
size_t fuse_sprintf(fuse_t *self, char *buffer, size_t size, const char *format, ...);

#endif /* FUSE_PRINTF_H */
