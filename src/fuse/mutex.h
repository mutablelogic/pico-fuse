/** @file mutex.h
 *  @brief Private function prototypes and structure definitions for mutexes.
 */
#ifndef FUSE_PRIVATE_MUTEX_H
#define FUSE_PRIVATE_MUTEX_H

#include <fuse/fuse.h>

/** @brief Register value type for mutexes
 */
void fuse_register_value_mutex(fuse_t *self);

#endif
