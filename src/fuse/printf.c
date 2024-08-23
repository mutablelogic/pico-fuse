
#include <fuse/fuse.h>
#include <stdarg.h>

#include "fuse.h"
#include "printf.h"

/* @brief Append a character into the output buffer
 */
size_t outch(char *out, size_t n, size_t i, char c)
{
    assert(out);
    assert(i < n);

    out[i] = c;
    return i + 1;
}

/* @brief Append a cstring value into the output buffer
 */
size_t outv(fuse_t *self, char *out, size_t n, size_t i, fuse_value_t *v)
{
    assert(self);
    assert(out);
    assert(i < n);
    assert(v);

    int16_t magic = fuse_allocator_magic(self->allocator, v);
    assert(magic < FUSE_MAGIC_COUNT);
    assert(self->desc[magic].cstr);
    size_t c = self->desc[magic].cstr(self, v, out + i, n - i);
    return i + c;
}

/* @brief Append a json string value into the output buffer
 */
size_t outq(fuse_t *self, char *out, size_t n, size_t i, fuse_value_t *v)
{
    assert(self);
    assert(out);
    assert(i < n);
    assert(v);

    int16_t magic = fuse_allocator_magic(self->allocator, v);
    assert(magic < FUSE_MAGIC_COUNT);
    assert(self->desc[magic].qstr);
    size_t c = self->desc[magic].qstr(self, v, out + i, n - i);
    return i + c;
}

/* @brief Append a null-terminated string value into the output buffer
 */
size_t outs(char *out, size_t n, size_t i, const char *v)
{
    assert(out);
    assert(i < n);

    if (v == NULL)
    {
        v = "<NULL>";
    }
    while (i < n && *v)
    {
        out[i++] = *v++;
    }
    return i;
}

/* @brief Append a signed integer
 */
size_t itoa(char *out, size_t n, size_t i, int64_t v, int base, fuse_printf_flags_t flags)
{
    assert(out);
    assert(i < n);

    // zero value
    if (v == 0)
    {
        return outch(out, n, i, '0');
    }

    // negative value
    if (v < 0)
    {
        out[i++] = '-';
        v = -v;
        if (i >= n)
        {
            return i;
        }
    }

    size_t len = 0;
    while (i < n && v > 0)
    {
        out[i++] = (char)(v % base) + '0';
        v /= base;
        len++;
    }

    // reverse the string
    for (size_t j = 0; j < len / 2; j++)
    {
        char c = out[i - j - 1];
        out[i - j - 1] = out[i - len + j];
        out[i - len + j] = c;
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
    assert(out);
    assert(i < n);

    // zero value
    if (v == 0)
    {
        return outch(out, n, i, '0');
    }

    size_t len = 0;
    while (i < n && v > 0)
    {
        out[i++] = udigit(v, base, flags & FUSE_PRINTF_FLAG_UPPER);
        v /= base;
        len++;
    }

    // reverse the string
    for (size_t j = 0; j < len / 2; j++)
    {
        char c = out[i - j - 1];
        out[i - j - 1] = out[i - len + j];
        out[i - len + j] = c;
    }

    // return the new index
    return i;
}

/* @brief Format a string into the output buffer, replacing %v and %V with the value and type of the next argument
 *        respectively
 */
int fuse_vsprintf(fuse_t *self, char *out, size_t n, const char *format, va_list va)
{
    assert(self);
    assert(out);
    assert(format);

    // where the buffer is zero-sized, return zero
    if (n == 0)
    {
        return 0;
    }

    // Reduce n so it is the maximum number of characters that can be written,
    // not including the null terminator
    n--;

    // iterate over the format string until it is exhausted or the buffer is full
    int i = 0;
    while (*format && i < n)
    {
        if (*format != '%')
        {
            i = outch(out, n, i, *format);
            format++;
            continue;
        }
        else
        {
            format++;
            if (*format == '\0')
            {
                i = outch(out, n, i, '%');
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
                i = outs(out, n, i, "%l");
                continue;
            }
            break;
        }

        // evaluate specifier
        switch (*format)
        {
        case 's':
            // cstring
            i = outs(out, n, i, va_arg(va, const char *));
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
            i = outch(out, n, i, *format);
            format++;
            break;
        }
    }

    // termination
    outch(out, n + 1, i, '\0');

    // return number of characters written
    return i;
}

/* @brief Format a string into the output buffer, replacing %v and %V with the value and type of the next argument
 *        respectively
 */
size_t fuse_sprintf(fuse_t *self, char *buffer, size_t size, const char *format, ...)
{
    va_list va;
    va_start(va, format);
    const int ret = fuse_vsprintf(self, buffer, size, format, va);
    va_end(va);
    return ret;
}
