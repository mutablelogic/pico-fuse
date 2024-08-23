/** @file printf.h
 *  @brief Print formatting functions
 */
#ifndef FUSE_PRINTF_H
#define FUSE_PRINTF_H

#include <stddef.h>


/* @brief Format a string with arguments
 *
 * @param self The fuse application
 * @param buffer The buffer to write the formatted string to
 * @param size The size of the buffer, including the null terminator
 * @param format The format string
 * @param ... The arguments to format
 * @returns The number of characters written
 */
size_t fuse_sprintf(fuse_t *self, char *buffer, size_t size, const char *format, ...);

#endif /* FUSE_PRINTF_H */
