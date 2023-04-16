#include <stdio.h>
#include <stdarg.h>

#include <pico/stdlib.h>
#include <fuse/fuse.h>
#include "main.h"

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

/*
 * Debug to stdout if the debug flag is set
 */
inline void fuse_debugf(fuse_t *fuse, const char *format, ...)
{
    if (fuse && fuse_is(fuse, FUSE_FLAG_DEBUG))
    {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }
}

/*
 * Debug to stdout if the debug flag is set
 */
inline void fuse_panic(const char *expr, const char *file, int line)
{
    printf("PANIC: %s (%s:%d)\n", expr, file, line);
    sleep_ms(1000);
    panic(expr);
}
