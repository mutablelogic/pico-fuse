#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <fuse/fuse.h>

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

/*
 * Debug to stdout if the debug flag is set
 *
 * @param fuse The fuse application
 * @param format The format string
 * @param ... The format arguments
 */
inline void fuse_debugf(fuse_t *fuse, const char *format, ...)
{
    if (fuse==NULL || fuse_is(fuse, FUSE_FLAG_DEBUG))
    {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }
}

/*
 * Panic and abort, printing out the expression, file and line number
 *
 * @param expr The expression that failed
 * @param file The file in which the expression failed
 * @param line The line number on which the expression failed
 */
inline void fuse_panic(const char *expr, const char *file, int line)
{
    printf("PANIC: %s (%s:%d)\n", expr, file, line);
    sleep_ms(1000);
    abort();
}
