/** @file debug.h
 *  @brief Function prototypes for debugging
 *
 *  This contains the prototypes for debugging functions
 */
#ifndef FUSE_DEBUG_H
#define FUSE_DEBUG_H

/** @brief Prints formatted debugging message if FUSE_FLAG_DEBUG
 *         is set in fuse->flags, or if the fuse argument is NULL
 *
 *  @param fuse The application instance, or NULL
 *  @param format The format string
 *  @param ... The arguments to the format string
 */
void fuse_debugf(fuse_t *fuse, const char *format, ...);

/** @brief Panic and print an expression
 *
 *  @param expr The expression to test
 *  @param file The file where the expression is tested
 *  @param line The line of the file where the expression is tested
 */
void fuse_panic(const char *expr, const char *file, int line);

#ifdef assert
#undef assert
#endif
#define assert(e) \
    ((void)((e) ? 0 : fuse_panic(#e, __FILE__, __LINE__)))

#endif
