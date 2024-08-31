/** @file list.h
 *  @brief Linked list
 *
 *  This file contains the declaration for a linked list of values
 */
#ifndef FUSE_LIST_H
#define FUSE_LIST_H

#include "value.h"

#ifdef DEBUG
#define fuse_new_list(self) \
    (fuse_new_value_ex((self), (FUSE_MAGIC_LIST), (0), __FILE__, __LINE__))
#else
#define fuse_new_list(self) \
    (fuse_new_value_ex((self), (FUSE_MAGIC_LIST), (0), 0, 0))
#endif

/** @brief Append an element to the end of a list
 *
 *  The function will append a element to the end of a list and return that element.
 *
 *  @param self The fuse instance
 *  @param list The list
 *  @param element The element to append
 *  @returns The appended element or NULL if the operation failed
 */
fuse_value_t *fuse_list_append(fuse_t *self, fuse_value_t *list, fuse_value_t *element);

/** @brief Append an element to the beginning of a list
 *
 *  The function will append a element to the beginning of a list and return that element.
 *
 *  @param self The fuse instance
 *  @param list The list
 *  @param element The element to add to the beginning of the list
 *  @returns The added element or NULL if the operation failed
 */
fuse_value_t *fuse_list_push(fuse_t *self, fuse_value_t *list, fuse_value_t *element);

/** @brief Remove an element from the end of the list
 *
 *  The function will remove an element from the end of a list and return that element.
 *
 *  @param self The fuse instance
 *  @param list The list
 *  @returns The removed element or NULL if the operation failed (for example, if the list was empty)
 */
fuse_value_t *fuse_list_pop(fuse_t *self, fuse_value_t *list);

/** @brief Return the next element in a list
 *
 *  Return the next element in a list. If the element is NULL, the first element is returned.
 *  If the element is the last element, NULL is returned.
 *
 *  @param self The fuse instance
 *  @param list The list
 *  @param elem The current element, or NULL to get the first element
 *  @returns The next element or NULL if the list is empty or the current element is the last element
 */
fuse_value_t *fuse_list_next(fuse_t *self, fuse_value_t *list, fuse_value_t *elem);

/** @brief Return the number of elements in the list or map
 *
 *  The function will return the number of elements in the list or map
 *
 *  @param self The fuse instance
 *  @param value The list or map
 *  @returns The count of elements
 */
size_t fuse_count(fuse_t *self, fuse_value_t *value);

#endif /* FUSE_LIST_H */
