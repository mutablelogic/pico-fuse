/** @file string.h
 *  @brief String storage and manipulation
 *
 *  This file contains the function prototypes for working with UTF-8 strings
 */
#ifndef FUSE_STRING_H
#define FUSE_STRING_H

/** @brief The representation of a fuse string
 *
 *  A string is a UTF-8 string, which can be immutable or mutable
 */
typedef struct fuse_str fuse_str_t;

/** @brief Create a new string from a c-string
 *
 *  @param fuse The fuse application
 *  @param str Pointer to the c-string, or NULL if an zero-sized string is to be created
 *  @param file The file where the string is created
 *  @param line The line of the file where the string is created
 *  @return Returns a pointer to a string instance, or NULL if the string could not be allocated
 */
fuse_str_t *fuse_str_new_ex(fuse_t *fuse, const char *str, const char *file, int line);

#ifdef DEBUG
#define fuse_str_new(self, ptr) \
    (fuse_str_new_ex((self), (ptr), __FILE__, __LINE__))
#else
#define fuse_str_new(self, ptr) \
    (fuse_str_new_ex((self), (ptr), 0, 0))
#endif

/** @brief Deallocate a string
 *
 *  @param ptr Pointer to the string
 */
fuse_str_t *fuse_str_destroy(fuse_t *fuse, fuse_str_t *str);

#endif
