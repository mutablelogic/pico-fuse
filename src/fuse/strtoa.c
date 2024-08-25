#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

// Public headers
#include <fuse/fuse.h>

// Private headers
#include "printf.h"

/* @brief Append a character into the buffer
 */
inline size_t chtoa_internal(char *buf, size_t sz, size_t i, const char ch)
{
    assert(buf == NULL || sz > 0);
    if (buf && i < (sz - 1))
    {
        buf[i] = ch;
    }
    return i + 1;
}

/* @brief Append a null-terminated string into the buffer
 */
size_t cstrtoa_internal(char *buf, size_t sz, size_t i, const char *str)
{
    assert(buf == NULL || sz > 0);

    if (str == NULL)
    {
        str = FUSE_PRINTF_NULL;
    }
    while (*str)
    {
        if (buf && i < (sz - 1))
        {
            buf[i] = *str;
        }
        str++;
        i++;
    }

    // Return next index
    return i;
}

/* @brief Append a quoted-string into the buffer
 */
size_t strtoa_internal(char *buf, size_t sz, size_t i, const char *str)
{
    assert(buf == NULL || sz > 0);

    if (str == NULL)
    {
        return cstrtoa_internal(buf, sz, i, FUSE_PRINTF_NULL_JSON);
    }

    // Output the prefix
    i = chtoa_internal(buf, sz, i, '"');

    // Output characters
    while (*str)
    {
        switch (*str++)
        {
        case '"':
            i = cstrtoa_internal(buf, sz, i, "\\\"");
            break;
        case '\b':
            i = cstrtoa_internal(buf, sz, i, "\\b");
            break;
        case '\f':
            i = cstrtoa_internal(buf, sz, i, "\\f");
            break;
        case '\n':
            i = cstrtoa_internal(buf, sz, i, "\\n");
            break;
        case '\r':
            i = cstrtoa_internal(buf, sz, i, "\\r");
            break;
        case '\t':
            i = cstrtoa_internal(buf, sz, i, "\\t");
            break;
        default:
        {
            const char ch = *(str - 1);
            if (ch <= 0x1F || ch == 0x7F)
            {
                i = cstrtoa_internal(buf, sz, i, "\\u");
                i = utoa_internal(buf, sz, i, ch, FUSE_PRINTF_FLAG_HEX | FUSE_PRINTF_FLAG_UPPER | 4);
            }
            else
            {
                i = chtoa_internal(buf, sz, i, ch);
            }
        }
        }
    }

    // Output the suffix
    i = chtoa_internal(buf, sz, i, '"');

    // Return the new index
    return i;
}

/* @brief Return a quoted string value
 *
 */
size_t strtoa(char *buf, size_t sz, const char *v)
{
    assert(buf == NULL || sz > 0);
    size_t i = strtoa_internal(buf, sz, 0, v);

    // Terminate the string
    if (buf)
    {
        size_t k = MIN(i, sz - 1);
        buf[k] = '\0';
    }

    // Return the number of characters that would have been written
    return i;
}
