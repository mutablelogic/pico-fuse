/** @file printf.h
 *  @brief Print formatting functions
 */
#ifndef FUSE_PRINTF_H
#define FUSE_PRINTF_H

#include <stddef.h>

size_t fuse_sprintf(char *buffer, size_t size, const char *format, ...);

#endif /* FUSE_PRINTF_H */
