/** @file value.h
 *  @brief Value function prototypes
 *
 *  This file contains the function prototypes for arbitary values,
 *  which are used for linked lists.
 */
#ifndef FUSE_VALUE_H
#define FUSE_VALUE_H

#include "fuse.h"

/** @brief Value defintion
 */
typedef struct fuse_value fuse_value_t;

/** @brief Create a new NULL value
 *  @param fuse The fuse application
 *  @returns A NULL value
 */
fuse_value_t *fuse_new_null(fuse_t *fuse);

/** @brief Create a new pointer value
 *
 *  The pointer value can be used to store a pointer to an arbitary value,
 *  including a NULL pointer.
 *
 *  @param fuse The fuse application
 *  @param ptr  The pointer to store
 *  @returns A pointer value
 */
fuse_value_t *fuse_new_ptr(fuse_t *fuse, void *ptr);

/** @brief Return the pointer value
 */
void *fuse_ptr(fuse_value_t *value);

/** @brief Return the value as a zero-terminated JSON value
 */
const char *fuse_jsonstring(fuse_value_t *value);

/** @brief Return true if the value contains NULL
 */
bool fuse_equal_null(fuse_value_t *value);

#endif // FUSE_VALUE_H
