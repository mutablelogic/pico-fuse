#ifndef FUSE_PRIVATE_MAIN_H
#define FUSE_PRIVATE_MAIN_H

#include <fuse/fuse.h>

/*
 * Represents an instance of a fuse application
 */
struct fuse_instance
{
    // The flags which affect the behaviour of the application
    fuse_flag_t flags;
};

#endif
