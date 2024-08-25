#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

// Public headers
#include <fuse/fuse.h>

// Private headers
#include "printf.h"

/* @brief Append a value to a buffer as a base64 string
 */
size_t b64toa_internal(char *buf, size_t sz, size_t i, void *v, size_t len)
{
    assert(buf == NULL || sz > 0);
    assert(v);

    size_t j = 0;
    while(j < len) {
        // TODO
    }

    // Return the new index
    return i;
}

/* @brief Convert a value to a  base64 string
 */
size_t b64toa(char *buf, size_t sz, void *v, size_t len)
{
    assert(buf == NULL || sz > 0);
    assert(v);

    size_t i = b64toa_internal(buf, sz, 0, v, len);

    // Terminate the string
    if (buf)
    {
        size_t k = MIN(i, sz - 1);
        buf[k] = '\0';
    }

    // Return the number of characters that would have been written
    return i;
}
