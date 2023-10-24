/** @file list.h
 *  @brief Fuse linked list function prototypes
 *
 *  This file contains the function prototypes for creating and destroying
 *  linked lists.
 */
#ifndef FUSE_LIST_H
#define FUSE_LIST_H
#include "fuse.h"

/** @brief Linked list defintion
 */
typedef struct fuse_list_instance fuse_list_t;

/** @brief Create a new empty linked list
 *  @param fuse The fuse application
 *  @returns A new empty linked list, or NULL if memory could not be allocated
 */
fuse_list_t *fuse_list_new(fuse_t *fuse);

/** @brief Deallocate a fuse linked list
 *
 *  @param self The linked list to deallocate
 */
void fuse_list_destroy(fuse_list_t *self);

/** @brief Return the number of nodes in the list
 *
 *  @param self The linked list
 *  @returns The number of nodes in the list
 */
void fuse_list_count(fuse_list_t *self);

/** @brief Return the next node in the list
 *
 *  @param self The linked list
 *  @param ptr  The current node, or NULL if the first node should be returned
 *  @returns The next node in the linked list, or NULL if there is no next node
 */
void *fuse_list_next(fuse_list_t *self, void *ptr);

/** @brief Return the previous node in the list
 *
 *  @param self The linked list
 *  @param ptr  The current node, or NULL if the last node should be returned
 *  @returns The previous node in the linked list, or NULL if there is no previous node
 */
void *fuse_list_prev(fuse_list_t *self, void *ptr);

/** @brief Add a node at the end of the list
 *
 *  @param self The linked list
 *  @param ptr  The new node to add to the end of the list
 *  @returns Returns true if successful, false otherwise
 */
void *fuse_list_push(fuse_list_t *self, void *ptr);

/** @brief Remove a node from the beginning of the list
 *
 *  @param self The linked list
 *  @returns Returns the node removed from the beginning of the list, or NULL if the list is empty
 */
void *fuse_list_pop(fuse_list_t *self);

#endif // FUSE_LIST_H
