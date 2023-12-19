/** @file magic.h
 *  @brief Fuse magic numbers
 *
 *  This file contains the signatures for the magic numbers used
 */
#ifndef FUSE_MAGIC_H
#define FUSE_MAGIC_H

#define FUSE_MAGIC_NULL 0x00000000  ///< NULL value
#define FUSE_MAGIC_APP 0x65730000   ///< Application
#define FUSE_MAGIC_BLOCK 0x65730001 ///< Memory block
#define FUSE_MAGIC_U8 0x65730002    ///< uint8_t
#define FUSE_MAGIC_U16 0x65730003   ///< uint16_t
#define FUSE_MAGIC_U32 0x65730004   ///< uint32_t
#define FUSE_MAGIC_U64 0x65730005   ///< uint64_t
#define FUSE_MAGIC_S8 0x65730006    ///< int8_t
#define FUSE_MAGIC_S16 0x65730007   ///< int16_t
#define FUSE_MAGIC_S32 0x65730008   ///< int32_t
#define FUSE_MAGIC_S64 0x65730009   ///< int64_t
#define FUSE_MAGIC_F32 0x6573000A   ///< float32_t
#define FUSE_MAGIC_F64 0x6573000B   ///< float64_t
#define FUSE_MAGIC_BOOL 0x6573000C  ///< bool
#define FUSE_MAGIC_STR 0x6573000D   ///< string
#define FUSE_MAGIC_LIST 0x6573000E  ///< list[value]
#define FUSE_MAGIC_MAP 0x6573000F   ///< map[value][value]

/** @brief Convert a magic number to a string
 *
 *  @param magic The magic number
 *  @returns A zero-terminated string representation of the magic number
 */
const char *fuse_magic_cstr(uint16_t magic);

#endif /* FUSE_MAGIC_H */
