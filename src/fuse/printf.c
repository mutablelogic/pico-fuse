#include <stdio.h>
#include <stdarg.h>
#include <fuse/fuse.h>

#if defined(TARGET_PICO)
#include <pico/stdio.h>
#endif

#include "fuse.h"
#include "printf.h"

/* @brief Append a pointer value
 */
size_t ptostr_internal(char *buf, size_t sz, size_t i, void *v)
{
    assert(buf == NULL || sz > 0);

    // Output the prefix
    i = cstrtostr_internal(buf, sz, i, "0x");

    // Output the value in hexadecimal, padded to the size of a pointer
    i = utostr_internal(buf, sz, i, (uintptr_t)v, FUSE_PRINTF_FLAG_HEX | (sizeof(void *) << 1));

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
            i = chtostr_internal(buf, sz, i, *format);
            format++;
            continue;
        }
        else
        {
            format++;
            if (*format == '\0')
            {
                i = chtostr_internal(buf, sz, i, '%');
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
                i = cstrtostr_internal(buf, sz, i, "%l");
                continue;
            }
            break;
        }

        // evaluate specifier
        switch (*format)
        {
        case 's':
            // cstring
            i = cstrtostr_internal(buf, sz, i, va_arg(va, const char *));
            format++;
            break;
        case 'd':
            // signed integer
            if (flags & FUSE_PRINTF_FLAG_LONG)
            {
                i = itostr_internal(buf, sz, i, va_arg(va, int64_t), flags);
            }
            else
            {
                i = itostr_internal(buf, sz, i, va_arg(va, int32_t), flags);
            }
            format++;
            break;
        case 'u':
            // unsigned integer
            if (flags & FUSE_PRINTF_FLAG_LONG)
            {
                i = utostr_internal(buf, sz, i, va_arg(va, uint64_t), flags);
            }
            else
            {
                i = utostr_internal(buf, sz, i, va_arg(va, uint32_t), flags);
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
                i = utostr_internal(buf, sz, i, va_arg(va, uint64_t), flags | FUSE_PRINTF_FLAG_HEX);
            }
            else
            {
                i = utostr_internal(buf, sz, i, va_arg(va, uint32_t), flags | FUSE_PRINTF_FLAG_HEX);
            }
            format++;
            break;
        case 'b':
            // unsigned binary value
            if (flags & FUSE_PRINTF_FLAG_LONG)
            {
                i = utostr_internal(buf, sz, i, va_arg(va, uint64_t), flags | FUSE_PRINTF_FLAG_BIN);
            }
            else
            {
                i = utostr_internal(buf, sz, i, va_arg(va, uint32_t), flags | FUSE_PRINTF_FLAG_BIN);
            }
            format++;
            break;
        case 'v':
            // value
            i = vtostr_internal(self, buf, sz, i, va_arg(va, fuse_value_t *), false);
            format++;
            break;
        case 'q':
            // quoted value
            i = vtostr_internal(self, buf, sz, i, va_arg(va, fuse_value_t *), true);
            format++;
            break;
        case 'p':
            // pointer
            i = ptostr_internal(buf, sz, i, va_arg(va, void *));
            format++;
            break;
        case 'f':
            // float or double
            i = ftostr_internal(buf, sz, i, va_arg(va, double), 0);
            format++;
            break;
        case '%':
            // quote a %
            i = chtostr_internal(buf, sz, i, '%');
            format++;
            break;
        default:
            assert(*format == 0);
        }
    }

    // Terminate the string
    if (buf)
    {
        size_t k = MIN_(i, sz - 1);
        buf[k] = '\0';
    }

    // return number of characters written
    return i;
}

/* @brief Format a string to stdout, replacing formatting directives
 */
size_t fuse_vprintf(fuse_t *self, const char *format, va_list va)
{
    assert(self);
    assert(format);

    // We have a static buffer for printf
    static const int sz = FUSE_PRINTF_BUFFER_SIZE;
    static char buffer[FUSE_PRINTF_BUFFER_SIZE];
    va_list va2;

    // Make a copy
    va_copy(va2, va);

    // Try to print into the buffer first
    const int n = fuse_vsprintf(self, buffer, sz, format, va);
    if (n < (sz - 1))
    {
        // Happy path - write the string to stdout
#if defined(TARGET_PICO)
        stdio_put_string(buffer, n, false, true);
        stdio_flush();
#else
        // TODO: Don't print the terminating \n
        puts(buffer);
#endif
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
        fuse_vsprintf(self, tmp, n + 1, format, va2);

        // Write the string to stdout
#if defined(TARGET_PICO)
        stdio_put_string(tmp, n, false, true);
        stdio_flush();
#else
        // TODO: Don't print the terminating \n
        puts(tmp);
#endif

        // Free the memory
        fuse_free(self, tmp);
    }

    // return the number of characters written
    return n;
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
    va_list va;
    va_start(va, format);
    const size_t n = fuse_vprintf(self, format, va);
    va_end(va);
    return n;
}

/** @brief Prints formatted debugging messages in debug mode
 */
size_t fuse_debugf(fuse_t *self, const char *format, ...)
{
#ifndef DEBUG
    return 0;
#else
    va_list va;
    va_start(va, format);
    const size_t n = fuse_vprintf(self, format, va);
    va_end(va);
    return n;
#endif
}
