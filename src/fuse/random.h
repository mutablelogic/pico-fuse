/** @file random.h
 *  @brief Private function prototypes and structure definitions for random numbers.
 */
#ifndef FUSE_PRIVATE_RANDOM_H
#define FUSE_PRIVATE_RANDOM_H

#include <stdint.h>

struct fuse_random_state {
    uint64_t seed;
};

#endif
