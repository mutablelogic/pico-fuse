/** @file list.h
 *  @brief Private function prototypes for lists
 */
#ifndef FUSE_PRIVATE_LIST_H
#define FUSE_PRIVATE_LIST_H

/** @brief Represents a linked list
 */
struct fuse_list
{
    size_t count;           ///< The number of elements in the list
};


/** @brief Register type for list values
 */
void fuse_register_value_list(fuse_t *self);

#endif
