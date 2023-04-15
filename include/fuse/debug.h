
#ifndef FUSE_DEBUG_H
#define FUSE_DEBUG_H

#include "main.h"

/*
 * Prints formatted debugging message if FUSE_FLAG_DEBUG
 * is set in fuse->flags.
 */
void fuse_debugf(fuse_t *fuse, const char *format, ...);

#endif
