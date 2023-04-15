#include <stdio.h>
#include <stdarg.h>

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

