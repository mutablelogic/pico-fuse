/** @file list.h
 *  @brief Linked list
 *
 *  This file contains the declaration for a linked list of values
 */
#ifndef FUSE_LIST_H
#define FUSE_LIST_H

#include "fuse.h"

/** @brief The abstract list structure
 */ 
typedef struct fuse_list fuse_list_t;

/** @brief Append an element to the end of a list
 *
 *  The function will append a element to the end of a list and return that element.
 *
 *  @param self The fuse instance
 *  @param list The list
 *  @param element The element to append
 *  @returns The appended element or NULL if the operation failed
 */
fuse_value_t *fuse_list_append(fuse_t *self, fuse_list_t *list, fuse_value_t *value);

/** @brief Append an element to the beginning of a list
 *
 *  The function will append a element to the beginning of a list and return that element.
 *
 *  @param self The fuse instance
 *  @param list The list
 *  @param element The element to add to the beginning of the list
 *  @returns The added element or NULL if the operation failed
 */
fuse_value_t *fuse_list_push(fuse_t *self, fuse_list_t *list, fuse_value_t *value);

/** @brief Remove an element from the end of the list
 *
 *  The function will remove an element from the end of a list and return that element.
 *
 *  @param self The fuse instance
 *  @param list The list
 *  @returns The removed element or NULL if the operation failed (for example, if the list was empty)
 */
fuse_value_t *fuse_list_pop(fuse_t *self, fuse_list_t *list);

/** @brief Return the number of elements in the list
 *
 *  The function will return the number of elements in the list
 *
 *  @param self The fuse instance
 *  @param list The list
 *  @returns The count of elements
 */
size_t fuse_list_count(fuse_t *self, fuse_list_t *list);

#endif /* FUSE_LIST_H */
