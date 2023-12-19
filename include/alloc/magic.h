/** @file magic.h
 *  @brief Fuse magic numbers
 *
 *  This file contains the signatures for the magic numbers used
 */
#ifndef FUSE_MAGIC_H
#define FUSE_MAGIC_H

#define FUSE_MAGIC_NULL 0x00000000  ///< NULL value
#define FUSE_MAGIC_APP 0x65735546   ///< Application
#define FUSE_MAGIC_BLOCK 0x65735547 ///< Memory block

/** @brief Convert a magic number to a string
 *
 *  @param magic The magic number
 *  @returns A zero-terminated string representation of the magic number
 */
const char* fuse_magic_cstr(uint16_t magic);

#endif /* FUSE_MAGIC_H */
