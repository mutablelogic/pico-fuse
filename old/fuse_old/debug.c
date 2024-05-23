#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <fuse/fuse.h>

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

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

inline void fuse_panic(const char *expr, const char *file, int line)
{
    if(file != NULL && line > 0) {
        printf("PANIC: %s (%s:%d)\n", expr, file, line);
    } else {
        printf("PANIC: %s\n", expr);
    }
    sleep_ms(1000);
    abort();
}
