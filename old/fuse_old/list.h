/** @file list.h
 *  @brief Private function prototypes and structure definitions for linked lists
 */
#ifndef FUSE_PRIVATE_LIST_H
#define FUSE_PRIVATE_LIST_H

#include "value.h"

/* @brief Represents an instance of a fuse linked list
 */
struct fuse_list_instance
{
    struct fuse_value_instance *head; ///< The first node in the list, or NULL if the list is empty
    struct fuse_value_instance *tail;  ///< The last node in the list, or NULL if the list is empty
    size_t count;                 ///< The number of nodes in the list
};

#endif
