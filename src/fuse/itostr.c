#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <fuse/fuse.h>
#include "printf.h"

/* @brief Return the numeric base for the flags
 */
static inline int itostr_base(fuse_printf_flags_t flags)
{
    if (flags & FUSE_PRINTF_FLAG_BIN)
    {
        return 2;
    }
    if (flags & FUSE_PRINTF_FLAG_HEX)
    {
        return 16;
    }
    return 10;
}

/* @brief Return the padding length of a signed integer value
 */
static inline int itostr_pad(fuse_printf_flags_t flags)
{
    return flags & 0xFF;
}

/* @brief Return digit for a given value
 */
static inline char itostr_udigit(uint64_t v, int base, bool upper)
{
    char c = (char)(v % base);
    return (c < 10) ? c + '0' : c - 10 + (upper ? 'A' : 'a');
}

/* @brief Reverse a string
 */
void itostr_reverse(char *buf, size_t sz, size_t i, size_t j)
{
    assert(buf);
    assert(sz > 0);
    assert(i <= j);

    size_t min = MIN_(i, sz - 1);
    size_t max = MIN_(j, sz - 1);
    size_t len = max - min;
    for (size_t k = 0; k < (len >> 1); k++)
    {
        char c = buf[min + k];
        buf[min + k] = buf[max - k - 1];
        buf[max - k - 1] = c;
    }
}

/* @brief Append an integer to a string buffer
 */
size_t itostr_internal(char *buf, size_t sz, size_t i, int64_t v, fuse_printf_flags_t flags)
{
    assert(buf == NULL || sz > 0);

    int base = itostr_base(flags);
    int len = itostr_pad(flags);
    bool neg = false;

    // Only support base 10 for signed integers
    assert(base == 10);

    // zero value shortcut
    if (v == 0 && len == 0)
    {
        return chtostr_internal(buf, sz, i, '0');
    }

    // negative value
    if (v < 0)
    {
        neg = true;
        v = -v;
    }

    // output the number, but in reverse order
    size_t j = i;
    while (v > 0)
    {
        j = chtostr_internal(buf, sz, j, itostr_udigit(v, base, false));
        v /= base;
    }

    // append the negative sign
    if (neg)
    {
        j = chtostr_internal(buf, sz, j, '-');
    }

    // pad with spaces if needed
    while ((j - i) < len)
    {
        j = chtostr_internal(buf, sz, j, (flags & FUSE_PRINTF_FLAG_ZEROPAD) ? '0' : ' ');
    }

    // reverse the string
    if (buf && sz > 2)
    {
        itostr_reverse(buf, sz, i, j);
    }

    // return the new index
    return j;
}

/* @brief Append an unsigned integer to a string buffer
 */
size_t utostr_internal(char *buf, size_t sz, size_t i, uint64_t v, fuse_printf_flags_t flags)
{
    assert(buf == NULL || sz > 0);

    int base = itostr_base(flags);
    int pad = itostr_pad(flags);

    // Support bases 2, 10 and 16 for unsigned integers
    assert(base == 2 || base == 10 || base == 16);

    // Zero-value
    size_t j = i;
    if (v == 0)
    {
        j = chtostr_internal(buf, sz, j, '0');
    }

    // output the number, but in reverse order
    while (v > 0)
    {
        j = chtostr_internal(buf, sz, j, itostr_udigit(v, base, flags & FUSE_PRINTF_FLAG_UPPER));
        v /= base;

        // break if we have a fixed width and we have reached it
        if (pad && j - i == pad)
        {
            break;
        }
    }

    // pad with zeros if needed
    while ((j - i) < pad)
    {
        j = chtostr_internal(buf, sz, j, '0');
    }

    // reverse the string
    if (buf && sz > 2)
    {
        itostr_reverse(buf, sz, i, j);
    }

    // return the new index
    return j;
}

/* @brief Return a signed integer value as a string, formatted as decimal
 */
size_t itostr(char *buf, size_t sz, int64_t v)
{
    assert(buf == NULL || sz > 0);
    size_t i = itostr_internal(buf, sz, 0, v, 0);

    // Terminate the string
    if (buf)
    {
        size_t k = MIN_(i, sz - 1);
        buf[k] = '\0';
    }

    // Return the number of characters that would have been written
    return i;
}

/* @brief Return an unsigned integer value as a string, formatted as decimal
 */
size_t utostr(char *buf, size_t sz, uint64_t v)
{
    assert(buf == NULL || sz > 0);
    size_t i = utostr_internal(buf, sz, 0, v, 0);

    // Terminate the string
    if (buf)
    {
        size_t k = MIN_(i, sz - 1);
        buf[k] = '\0';
    }

    // Return the number of characters that would have been written
    return i;
}

/* @brief Return an unsigned integer value as a string, formatted as binary
 */
size_t utostr_binary(char *buf, size_t sz, uint64_t v, uint8_t bits)
{
    assert(buf == NULL || sz > 0);
    size_t i = utostr_internal(buf, sz, 0, v, FUSE_PRINTF_FLAG_BIN | bits);

    // Terminate the string
    if (buf)
    {
        size_t k = MIN_(i, sz - 1);
        buf[k] = '\0';
    }

    // Return the number of characters that would have been written
    return i;
}

/* @brief Return an unsigned integer value as a string, formatted as hexidecimal
 */
size_t utostr_hex(char *buf, size_t sz, uint64_t v, uint8_t bits, bool upper)
{
    assert(buf == NULL || sz > 0);
    assert(bits % 4 == 0);

    fuse_printf_flags_t flags = FUSE_PRINTF_FLAG_HEX | (upper ? FUSE_PRINTF_FLAG_UPPER : 0);
    size_t i = utostr_internal(buf, sz, 0, v, flags | (bits >> 2));

    // Terminate the string
    if (buf)
    {
        size_t k = MIN_(i, sz - 1);
        buf[k] = '\0';
    }

    // Return the number of characters that would have been written
    return i;
}
