/** @file number.h
 *  @brief Private function prototypes and structure definitions for integer, unsigned integer, bool and float values.
 */
#ifndef FUSE_PRIVATE_NUMBER_H
#define FUSE_PRIVATE_NUMBER_H

#include <fuse/fuse.h>

/** @brief Register value type for integer values
 */
void fuse_register_value_int(fuse_t *self);

/** @brief Register value type for unsigned integer values
 */
void fuse_register_value_uint(fuse_t *self);

/** @brief Register value type for bool values
 */
void fuse_register_value_bool(fuse_t *self);

/** @brief Register value type for bool values
 */
void fuse_register_value_float(fuse_t *self);

#endif
