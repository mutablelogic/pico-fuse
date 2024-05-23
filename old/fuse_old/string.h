/** @file string.h
 *  @brief Private function prototypes and structure definitions for strings
 */
#ifndef FUSE_PRIVATE_STRING_H
#define FUSE_PRIVATE_STRING_H

#include <fuse/fuse.h>

/** @brief Represents a string
 *
 *  This data structure represents a UTF-8 encoded string. If the string is zero-sized then
 *  the ptr element is NULL and the size is zero.
 */
struct fuse_str
{
    void *ptr;     ///< A pointer to the string storage
    size_t size;   ///< The size of the string, in bytes
    bool constant; ///< Whether the string is constant or not
};

#endif
