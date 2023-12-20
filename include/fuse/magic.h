/** @file magic.h
 *  @brief Fuse magic numbers
 *
 *  This file contains the signatures for the magic numbers used
 */
#ifndef FUSE_MAGIC_H
#define FUSE_MAGIC_H

// Define the magic numbers for the memory blocks
#define FUSE_MAGIC_NULL 0x6570  ///< NULL value
#define FUSE_MAGIC_APP 0x6571   ///< Application
#define FUSE_MAGIC_BLOCK 0x6572 ///< Memory block
#define FUSE_MAGIC_U8 0x6573    ///< uint8_t
#define FUSE_MAGIC_U16 0x6574   ///< uint16_t
#define FUSE_MAGIC_U32 0x6575   ///< uint32_t
#define FUSE_MAGIC_U64 0x6576   ///< uint64_t
#define FUSE_MAGIC_S8 0x6577    ///< int8_t
#define FUSE_MAGIC_S16 0x6578   ///< int16_t
#define FUSE_MAGIC_S32 0x6579   ///< int32_t
#define FUSE_MAGIC_S64 0x657A   ///< int64_t
#define FUSE_MAGIC_F32 0x657B   ///< float32_t
#define FUSE_MAGIC_F64 0x657C   ///< float64_t
#define FUSE_MAGIC_BOOL 0x657D  ///< bool
#define FUSE_MAGIC_STR 0x657E   ///< string
#define FUSE_MAGIC_LIST 0x657F  ///< list[value]
#define FUSE_MAGIC_MAP 0x6580   ///< map[value][value]

// Define exit codes
#define FUSE_EXIT_SUCCESS 1     ///< Successful completion
#define FUSE_EXIT_MEMORYLEAKS 2 ///< Memory leaks detected in the application

/** @brief Convert a magic number to a string
 *
 *  @param magic The magic number
 *  @returns A zero-terminated string representation of the magic number
 */
const char *fuse_magic_cstr(uint16_t magic);

#endif /* FUSE_MAGIC_H */
