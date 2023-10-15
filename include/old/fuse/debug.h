
#ifndef FUSE_DEBUG_H
#define FUSE_DEBUG_H

#include "main.h"

/*
 * Prints formatted debugging message if FUSE_FLAG_DEBUG
 * is set in fuse->flags.
 */
void fuse_debugf(fuse_t *fuse, const char *format, ...);

/*
 * Panic and print an expression
 */
void fuse_panic(const char *expr, const char *file, int line);

#ifdef assert
#undef assert
#endif
#define assert(e) \
    ((void) ((e) ? 0 : fuse_panic(#e, __FILE__, __LINE__)))

#endif
