/** @file list.h
 *  @brief Private function prototypes and structure definitions for linked lists
 */
#ifndef FUSE_PRIVATE_LIST_H
#define FUSE_PRIVATE_LIST_H

#include <fuse/fuse.h>

/* @brief Represents an instance of a fuse linked list
 */
struct fuse_list_instance
{
    struct fuse_list_node *first; ///< The first node in the list, or NULL if the list is empty
    struct fuse_list_node *last;  ///< The last node in the list, or NULL if the list is empty
    size_t count;                 ///< The number of nodes in the list
};

/* @brief Represents a node in a fuse linked list
 */
struct fuse_list_node
{
    void *ptr;                   ///< The pointer to the node data
    struct fuse_list_node *prev; ///< The previous node in the list, or NULL if this is the first node
    struct fuse_list_node *next; ///< The next node in the list, or NULL if this is the last node
};

#endif
