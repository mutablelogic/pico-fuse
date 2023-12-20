/** @file flags.h
 *  @brief Feature flags for fuse
 *
 *  This file contains the feature flags for fuse
 */
#ifndef FUSE_FLAG_H
#define FUSE_FLAG_H

/** @brief Flags for the fuse application or memory pool
 */
typedef enum
{
    FUSE_NONE = 0x000,       ///< No flags
    FUSE_FLAG_DEBUG = 0x100, ///< Debug mode - prints out more verbose output
    FUSE_CLASS_MASK = 0x0FF, ///< Mask for the class value
    FUSE_CLASS_PTR = 0x001,  ///< Pointer class
    FUSE_CLASS_CSTR = 0x002, ///< Immutable UTF-8 string class
    FUSE_CLASS_USTR = 0x002, ///< Mutable UTF-8 string class
    FUSE_CLASS_U8 = 0x003,   ///< uint8_t class
    FUSE_CLASS_U16 = 0x004,  ///< uint16_t class
    FUSE_CLASS_U32 = 0x005,  ///< uint32_t class
    FUSE_CLASS_U64 = 0x006,  ///< uint64_t class
    FUSE_CLASS_S8 = 0x007,   ///< int8_t class
    FUSE_CLASS_S16 = 0x008,  ///< int16_t class
    FUSE_CLASS_S32 = 0x009,  ///< int32_t class
    FUSE_CLASS_S64 = 0x00A,  ///< int64_t class
    FUSE_CLASS_F32 = 0x00B,  ///< float32_t class
    FUSE_CLASS_MAP = 0x00C,  ///< fuse_map_t class
    FUSE_CLASS_LIST = 0x00D  ///< fuse_list_t class
} fuse_flag_t;

/** @brief Check for existence of fuse flags
 *
 *  @param fuse The fuse application
 *  @param flag The flags to check for
 *  @returns True if all the flags are set, false otherwise
 */
bool fuse_is(fuse_t *fuse, fuse_flag_t flag);

#endif
