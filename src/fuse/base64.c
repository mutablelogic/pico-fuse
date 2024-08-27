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
    static const char b64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    const char *in = v;
    size_t j = 0;
    while (j < len)
    {
        // Put 3 bytes into a 32-bit integer
        uint32_t bits = in[j];
        bits = (bits << 8) | ((j < len) ? in[j + 1] : 0);
        bits = (bits << 8) | ((j < len) ? in[j + 2] : 0);

        // Output the binary - 4 characters
        i = chtoa_internal(buf, sz, i, b64_table[(bits >> 18) & 0x3f]);
        i = chtoa_internal(buf, sz, i, j > (len - 1) ? '=' : b64_table[(bits >> 12) & 0x3f]);
        i = chtoa_internal(buf, sz, i, j > (len - 2) ? '=' : b64_table[(bits >> 6) & 0x3f]);
        i = chtoa_internal(buf, sz, i, j > (len - 3) ? '=' : b64_table[(bits >> 0) & 0x3F]);

        // Increment j by 3 bytes
        j += 3;
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

    // Decode
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
