#include <stdio.h>
#include <stdarg.h>

#include <fuse/fuse.h>
#include "fuse.h"
#include "printf.h"

/* @brief Append a character into the output buffer
 */
static inline size_t ctoa(char *out, size_t n, size_t i, char c)
{
    assert(out == NULL || i < n);
    if (out)
    {
        out[i] = c;
    }
    return i + 1;
}

/* @brief Append a value into the output buffer
 */
size_t outv(fuse_t *self, char *out, size_t n, size_t i, fuse_value_t *v)
{
    assert(self);
    assert(out == NULL || i < n);
    assert(v);

    int16_t magic = fuse_allocator_magic(self->allocator, v);
    assert(magic < FUSE_MAGIC_COUNT);
    assert(self->desc[magic].cstr);

    if (out == NULL)
    {
        return self->desc[magic].cstr(self, v, NULL, 0) + i;
    }
    else
    {
        return self->desc[magic].cstr(self, v, out + i, n - i) + i;
    }
}

/* @brief Append a json (quoted) string value into the output buffer
 */
size_t outq(fuse_t *self, char *out, size_t n, size_t i, fuse_value_t *v)
{
    assert(self);
    assert(out == NULL || i < n);
    assert(v);

    int16_t magic = fuse_allocator_magic(self->allocator, v);
    assert(magic < FUSE_MAGIC_COUNT);
    assert(self->desc[magic].qstr);

    if (out == NULL)
    {
        return self->desc[magic].qstr(self, v, NULL, 0) + i;
    }
    else
    {
        return self->desc[magic].qstr(self, v, out + i, n - i) + i;
    }
}

/* @brief Append a null-terminated string
 */
size_t stoa(char *out, size_t n, size_t i, const char *v, fuse_printf_flags_t flags)
{
    assert(out == NULL || i < n);

    if (v == NULL)
    {
        v = "<NULL>";
    }
    while (i < n && *v)
    {
        if (out)
        {
            out[i] = *v;
        }
        v++;
        i++;
    }
    return i;
}

/* @brief Append a null-terminated string in quoted format
 */
size_t qtoa(char *out, size_t n, size_t i, const char *v, fuse_printf_flags_t flags)
{
    assert(out == NULL || i < n);

    if (v == NULL)
    {
        return stoa(out, n, i, "null", flags);
    }

    // Output the prefix
    if (i = ctoa(out, n, i, '"'), i >= n)
        return i;

    // Output the string
    while (i < n && *v)
    {
        switch (*v)
        {
        case '"':
            i = ctoa(out, n, i, '\\');
            break;
        case '\n':
            i = ctoa(out, n, i, '\\');
            break;
        default:
            i = ctoa(out, n, i, *v++);
        }
    }

    // Output the suffix
    if (i < n)
    {
        i = ctoa(out, n, i, '"');
    }

    return i;
}

/* @brief Append a signed integer
 */
size_t itoa(char *out, size_t n, size_t i, int64_t v, int base, fuse_printf_flags_t flags)
{
    assert(out == NULL || i < n);

    // zero value
    if (v == 0)
    {
        return ctoa(out, n, i, '0');
    }

    // negative value
    if (v < 0)
    {
        if (out)
        {
            out[i] = '-';
        }
        v = -v;
        if (++i >= n)
        {
            return i;
        }
    }

    // output the number, but in reverse order
    size_t len = 0;
    while (i < n && v > 0)
    {
        if (out)
        {
            out[i] = (char)(v % base) + '0';
        }
        v /= base;
        i++;
        len++;
    }

    // reverse the string
    if (out)
    {
        for (size_t j = 0; j < len / 2; j++)
        {
            char c = out[i - j - 1];
            out[i - j - 1] = out[i - len + j];
            out[i - len + j] = c;
        }
    }

    // return the new index
    return i;
}

/* @brief Return digit for a given value
 */
static inline char udigit(uint64_t v, int base, bool upper)
{
    char c = (char)(v % base);
    return (c < 10) ? c + '0' : c - 10 + (upper ? 'A' : 'a');
}

/* @brief Append an unsigned integer
 */
size_t utoa(char *out, size_t n, size_t i, uint64_t v, int base, fuse_printf_flags_t flags)
{
    assert(out == NULL || i < n);

    // zero value
    if (v == 0)
    {
        return ctoa(out, n, i, '0');
    }

    size_t len = 0;
    while (i < n && v > 0)
    {
        if (out)
        {
            out[i] = udigit(v, base, flags & FUSE_PRINTF_FLAG_UPPER);
        }
        v /= base;
        i++;
        len++;
    }

    // reverse the string
    if (out)
    {
        for (size_t j = 0; j < len / 2; j++)
        {
            char c = out[i - j - 1];
            out[i - j - 1] = out[i - len + j];
            out[i - len + j] = c;
        }
    }

    // return the new index
    return i;
}

/* @brief Format a string into the output buffer or to stdout, replacing formatting directives
 */
int fuse_vsprintf(fuse_t *self, char *out, size_t n, const char *format, va_list va)
{
    assert(self);
    assert(format);
    assert(out == NULL || n > 0);

    // Reduce n so it is the maximum number of characters that can be written,
    // not including the null terminator
    n--;

    // iterate over the format string until it is exhausted or the buffer is full
    int i = 0;
    while (*format && i < n)
    {
        if (*format != '%')
        {
            i = ctoa(out, n, i, *format);
            format++;
            continue;
        }
        else
        {
            format++;
            if (*format == '\0')
            {
                i = ctoa(out, n, i, '%');
                continue;
            }
        }

        // evaluate flags
        fuse_printf_flags_t flags = 0;
        switch (*format)
        {
        case 'l':
            flags |= FUSE_PRINTF_FLAG_LONG;
            format++;
            if (*format == '\0')
            {
                i = stoa(out, n, i, "%l", 0);
                continue;
            }
            break;
        }

        // evaluate specifier
        switch (*format)
        {
        case 's':
            // cstring
            i = stoa(out, n, i, va_arg(va, const char *), flags);
            format++;
            break;
        case 'd':
            // signed integer
            if (flags & FUSE_PRINTF_FLAG_LONG)
            {
                i = itoa(out, n, i, va_arg(va, int64_t), 10, flags);
            }
            else
            {
                i = itoa(out, n, i, va_arg(va, int32_t), 10, flags);
            }
            format++;
            break;
        case 'u':
            // unsigned integer
            if (flags & FUSE_PRINTF_FLAG_LONG)
            {
                i = utoa(out, n, i, va_arg(va, uint64_t), 10, flags);
            }
            else
            {
                i = utoa(out, n, i, va_arg(va, uint32_t), 10, flags);
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
                i = utoa(out, n, i, va_arg(va, uint64_t), 16, flags);
            }
            else
            {
                i = utoa(out, n, i, va_arg(va, uint32_t), 16, flags);
            }
            format++;
            break;
        case 'v':
            // value
            i = outv(self, out, n, i, va_arg(va, fuse_value_t *));
            format++;
            break;
        case 'q':
            // quoted (in JSON format)
            i = outq(self, out, n, i, va_arg(va, fuse_value_t *));
            format++;
            break;
        default:
            i = ctoa(out, n, i, *format);
            format++;
            break;
        }
    }

    // termination
    ctoa(out, n + 1, i, '\0');

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

    // We have a static buffer for printf less than 81 characters
    static const int sz = 81;
    static char buffer[sz] = {0};

    // Try to print to the buffer first
    va_list va;
    va_start(va, format);
    const int n = fuse_vsprintf(self, buffer, sz, format, va);
    va_end(va);

    if (n < (sz - 1))
    {
        // Print to stdout
        puts(buffer);
        return n;
    }
    else
    {
        // TODO: Allocate a buffer and print to it
        assert("TODO");
        // print to the buffer
        // puts the buffer
        // Free the allocated buffer
    }

    return n;
}
