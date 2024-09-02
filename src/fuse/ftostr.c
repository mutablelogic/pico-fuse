#include <float.h>
#include <stdbool.h>
#include <fuse/fuse.h>

// Private headers
#include "printf.h"

#define is_nan(x) __builtin_isnan(x)

/* @brief Append an float32 to a string buffer
 */
size_t ftostr_internal(char *buf, size_t sz, size_t i, double v, fuse_printf_flags_t flags)
{
    assert(buf == NULL || sz > 0);

    // test for special values
    if (is_nan(v))
    {
        return cstrtostr_internal(buf, sz, 0, FUSE_PRINTF_NAN);
    }
    if (v < -DBL_MAX)
    {
        return cstrtostr_internal(buf, sz, 0, FUSE_PRINTF_MINUS_INF);
    }
    if (v > DBL_MAX)
    {
        return cstrtostr_internal(buf, sz, 0, FUSE_PRINTF_PLUS_INF);
    }

    // test for negative values
    bool neg = false;
    if (v < 0)
    {
        neg = true;
        v = -v;
    }

    // get whole and fractional parts
    uint64_t whole = (int64_t)v;
    double frac = v - (double)whole;

    // precision up to 10 decimal places
    int prec = MIN_(flags & 0xF, 10);
    static const double pow10[] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};

    // output the negative sign
    if (neg)
    {
        i = chtostr_internal(buf, sz, i, '-');
    }

    // output the whole part
    i = utostr_internal(buf, sz, i, whole, 0);

    if(prec == 0)
    {
        prec = 5;
    }

    // output the fractional part
    if (prec > 0 || frac > 0)
    {
        i = chtostr_internal(buf, sz, i, '.');
        i = utostr_internal(buf, sz, i, frac * pow10[prec], prec | FUSE_PRINTF_FLAG_ZEROPAD);
    }

    // return the new index
    return i;
}

/* @brief Return an double float value as a string
 */
size_t ftostr(char *buf, size_t sz, double v)
{
    assert(buf == NULL || sz > 0);
    size_t i = ftostr_internal(buf, sz, 0, v, 0);

    // Terminate the string
    if (buf)
    {
        size_t k = MIN_(i, sz - 1);
        buf[k] = '\0';
    }

    // Return the number of characters that would have been written
    return i;
}
