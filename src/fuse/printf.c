
#include <fuse/fuse.h>
#include <stdarg.h>
#include "printf.h"

/* @brief Append a character into the output buffer
 */
size_t outch(char *out, size_t i, size_t n, char c)
{
    assert(out);
    assert(i < n);

    out[i] = c;
    return i + 1;
}

/* @brief Format a string into the output buffer, replacing %v and %V with the value and type of the next argument
 *        respectively
 */
int fuse_vsprintf(char *out, size_t n, const char *format, va_list va)
{
    int i = 0;
    while (*format && i < (n - 1))
    {
        if (*format != '%')
        {
            i = outch(out, i, n, *format);
            format++;
            continue;
        }
        else
        {
            format++;
            if (*format == '\0')
            {
                i = outch(out, i, n, '%');
                continue;
            }
        }

        // evaluate specifier
        switch (*format)
        {
        case 'v':
            // value cstr
            i = outch(out, i, n, '!');
            format++;
            break;
        case 'V':
            // value type
            i = outch(out, i, n, '!');
            format++;
            break;
        default:
            i = outch(out, i, n, *format);
            format++;
            break;
        }
    }

    // termination
    outch(out, i, n, '\0');

    // return number of characters written
    return i;
}

/* @brief Format a string into the output buffer, replacing %v and %V with the value and type of the next argument
 *        respectively
 */
size_t fuse_sprintf(char *buffer, size_t size, const char *format, ...)
{
    va_list va;
    va_start(va, format);
    const int ret = fuse_vsprintf(buffer, size, format, va);
    va_end(va);
    return ret;
}
