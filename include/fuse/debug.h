
/** @file debug.h
 *  @brief Function prototypes for debugging
 *
 *  This contains the prototypes for debugging functions
 */
#ifndef FUSE_DEBUG_H
#define FUSE_DEBUG_H

/** @brief Prints formatted debugging messages
 *
 *  @param format The format string
 *  @param ... The arguments to the format string
 */
void fuse_debugf(const char *format, ...);

#endif
