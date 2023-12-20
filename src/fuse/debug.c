#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

// Includes
#include <fuse/fuse.h>

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

inline void fuse_debugf(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}
