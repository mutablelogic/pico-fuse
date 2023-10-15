
#ifndef FUSE_FLAG_H
#define FUSE_FLAG_H

/*
 * Flags for the fuse application
 */
typedef enum
{
    // No flags
    FUSE_FLAG_NONE = 0,

    // Debug mode - prints out more verbose output
    FUSE_FLAG_DEBUG = 1 << 0,
} fuse_flag_t;

/*
 * Check for existence of fuse flags
 * 
 * @param fuse The fuse application
 * @param flag The flags to check for
 * @returns True if all the flags are set, false otherwise
 */
bool fuse_is(fuse_t *fuse, fuse_flag_t flag);

#endif
