
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
    size_t c = self->desc[magic].cstr(v, out + i, n - i);
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
    size_t c = self->desc[magic].qstr(v, out + i, n - i);
    return i + c;
}

/* @brief Format a string into the output buffer, replacing %v and %V with the value and type of the next argument
 *        respectively
 */
int fuse_vsprintf(fuse_t *self, char *out, size_t n, const char *format, va_list va)
{
    assert(self);
    assert(out);
    assert(format);

    int i = 0;
    while (*format && i < (n - 1))
    {
        fuse_debugf("outv %c\n", *format);
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

        // evaluate specifier
        switch (*format)
        {
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
    outch(out, n, i, '\0');

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
