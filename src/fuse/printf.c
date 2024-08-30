#include <stdio.h>
#include <stdarg.h>

// Public headers
#include <fuse/fuse.h>

// Private headers
#include "fuse.h"
#include "printf.h"

/* @brief Append a pointer value
 */
size_t ptoa(char *buf, size_t sz, size_t i, void *v)
{
    assert(buf == NULL || sz > 0);

    // Output the prefix
    i = cstrtoa_internal(buf, sz, i, "0x");

    // Output the value in hexadecimal, padded to the size of a pointer
    i = utoa_internal(buf, sz, i, (uintptr_t)v, FUSE_PRINTF_FLAG_HEX | (sizeof(void *) << 1));

    // return the new index
    return i;
}

/* @brief Format a string into the output buffer or to stdout, replacing formatting directives
 */
int fuse_vsprintf(fuse_t *self, char *buf, size_t sz, const char *format, va_list va)
{
    assert(self);
    assert(format);
    assert(buf == NULL || sz > 0);

    // iterate over the format string
    size_t i = 0;
    while (*format)
    {
        if (*format != '%')
        {
            i = chtoa_internal(buf, sz, i, *format);
            format++;
            continue;
        }
        else
        {
            format++;
            if (*format == '\0')
            {
                i = chtoa_internal(buf, sz, i, '%');
                continue;
            }
        }

        // evaluate flags
        fuse_printf_flags_t flags = 0;
        switch (*format)
        {
        case 'l':
            flags |= FUSE_PRINTF_FLAG_LONG;
            if (*++format == '\0')
            {
                i = cstrtoa_internal(buf, sz, i, "%l");
                continue;
            }
            break;
        }

        // evaluate specifier
        switch (*format)
        {
        case 's':
            // cstring
            i = cstrtoa_internal(buf, sz, i, va_arg(va, const char *));
            format++;
            break;
        case 'd':
            // signed integer
            if (flags & FUSE_PRINTF_FLAG_LONG)
            {
                i = itoa_internal(buf, sz, i, va_arg(va, int64_t), flags);
            }
            else
            {
                i = itoa_internal(buf, sz, i, va_arg(va, int32_t), flags);
            }
            format++;
            break;
        case 'u':
            // unsigned integer
            if (flags & FUSE_PRINTF_FLAG_LONG)
            {
                i = utoa_internal(buf, sz, i, va_arg(va, uint64_t), flags);
            }
            else
            {
                i = utoa_internal(buf, sz, i, va_arg(va, uint32_t), flags);
            }
            format++;
            break;
        case 'X':
            // upper case unsigned hexadecimal value
            flags |= FUSE_PRINTF_FLAG_UPPER;
        case 'x':
            // unsigned hexadecimal value
            if (flags & FUSE_PRINTF_FLAG_LONG)
            {
                i = utoa_internal(buf, sz, i, va_arg(va, uint64_t), flags | FUSE_PRINTF_FLAG_HEX);
            }
            else
            {
                i = utoa_internal(buf, sz, i, va_arg(va, uint32_t), flags | FUSE_PRINTF_FLAG_HEX);
            }
            format++;
            break;
        case 'b':
            // unsigned binary value
            if (flags & FUSE_PRINTF_FLAG_LONG)
            {
                i = utoa_internal(buf, sz, i, va_arg(va, uint64_t), flags | FUSE_PRINTF_FLAG_BIN);
            }
            else
            {
                i = utoa_internal(buf, sz, i, va_arg(va, uint32_t), flags | FUSE_PRINTF_FLAG_BIN);
            }
            format++;
            break;
        case 'v':
            // value
            i = vtoa_internal(self, buf, sz, i, va_arg(va, fuse_value_t *), false);
            format++;
            break;
        case 'q':
            // quoted value
            i = vtoa_internal(self, buf, sz, i, va_arg(va, fuse_value_t *), true);
            format++;
            break;
        case 'p':
            // pointer
            i = ptoa(buf, sz, i, va_arg(va, void *));
            format++;
            break;
        case 'f':
            // float or double
            i = ftoa_internal(buf, sz, i, va_arg(va, double), 0);
            format++;
            break;
        case '%':
            // quote a %
            i = chtoa_internal(buf, sz, i, '%');
            format++;
            break;
        default:
            assert(*format == 0);
        }
    }

    // Terminate the string
    if (buf)
    {
        size_t k = MIN(i, sz - 1);
        buf[k] = '\0';
    }

    // return number of characters written
    return i;
}

/* @brief Format a string into the output buffer, replacing formatting directives
 */
size_t fuse_sprintf(fuse_t *self, char *buffer, size_t size, const char *format, ...)
{
    assert(self);
    assert(format);

    va_list va;
    va_start(va, format);
    const int ret = fuse_vsprintf(self, buffer, size, format, va);
    va_end(va);
    return ret;
}

/* @brief Format a string to stdout, replacing formatting directives
 */
size_t fuse_printf(fuse_t *self, const char *format, ...)
{
    assert(self);
    assert(format);

    // We have a static buffer for printf
    static const int sz = FUSE_PRINTF_BUFFER_SIZE;
    static char buffer[FUSE_PRINTF_BUFFER_SIZE];

    // Try to print into the buffer first
    va_list va;
    va_start(va, format);
    const int n = fuse_vsprintf(self, buffer, sz, format, va);
    va_end(va);

    if (n < (sz - 1))
    {
        // Happy path - write the string to stdout
        puts(buffer);
    }
    else
    {
        // Unhappy path - allocate data, format and write the string to stdout
        char *tmp = fuse_alloc(self, FUSE_MAGIC_DATA, (void *)(uintptr_t)(n + 1));
        if (tmp == NULL)
        {
            return 0;
        }

        // Do it again...
        va_start(va, format);
        fuse_vsprintf(self, tmp, n + 1, format, va);
        va_end(va);

        // Write the string to stdout
        puts(tmp);

        // Free the memory
        fuse_free(self, tmp);
    }

    // return the number of characters written
    return n;
}
