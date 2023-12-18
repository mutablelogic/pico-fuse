/** @file assert.h
 *  @brief Function prototypes for assertations
 *
 *  This contains the prototypes for assertation functions
 */
#ifndef FUSE_ASSERT_H
#define FUSE_ASSERT_H

/** @brief Panic and print an expression
 *
 *  @param expr The expression to test
 *  @param file The file where the expression is tested
 *  @param line The line of the file where the expression is tested
 */
void fuse_panic(const char *expr, const char *file, int line);

#ifdef assert
#undef assert
#endif /* assert */

#ifdef DEBUG
#define assert(e) \
    ((void)((e) ? 0 : fuse_panic(#e, __FILE__, __LINE__)))
#else
#pragma message("no debugging")
#define assert(e) \
    ((void)((e) ? 0 : fuse_panic(#e, NULL, 0)))
#endif /* DEBUG */

#endif /* FUSE_ALLOC_H */
