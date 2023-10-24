/** @file flags.h
 *  @brief Feature flags for fuse
 *
 *  This file contains the feature flags for fuse
 */
#ifndef FUSE_FLAG_H
#define FUSE_FLAG_H

/** @brief Flags for the fuse application
 */
typedef enum
{
    FUSE_FLAG_NONE = 0,       ///< No flags
    FUSE_FLAG_DEBUG = 1 << 0, ///< Debug mode - prints out more verbose output

} fuse_flag_t;

/** @brief Check for existence of fuse flags
 *
 *  @param fuse The fuse application
 *  @param flag The flags to check for
 *  @returns True if all the flags are set, false otherwise
 */
bool fuse_is(fuse_t *fuse, fuse_flag_t flag);

#endif
