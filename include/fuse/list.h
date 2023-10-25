/** @file list.h
 *  @brief Fuse linked list function prototypes
 *
 *  This file contains the function prototypes for creating and destroying
 *  linked lists.
 */
#ifndef FUSE_LIST_H
#define FUSE_LIST_H
#include "fuse.h"
#include "value.h"

/** @brief Linked list defintion
 */
typedef struct fuse_list_instance fuse_list_t;

/** @brief Create a new empty linked list
 *  @param fuse The fuse application
 *  @returns A new empty linked list, or NULL if memory could not be allocated
 */
fuse_list_t *fuse_list_new(fuse_t *fuse);

#ifndef NO_DOC
#ifdef DEBUG
#define fuse_list_new(self) \
    (fuse_list_new_ex((self), __FILE__, __LINE__))
#else
#define fuse_list_new(self) \
    (fuse_list_new_ex((self), NULL, 0))
#endif /* DEBUG */
#endif /* NO_DOC */

/** @brief Create a new empty linked list
 *  @param fuse The fuse application
 *  @param file The file from which the linked list is allocated
 *  @param line The line in the file from which the linked list is allocated
 *  @returns A new empty linked list, or NULL if memory could not be allocated
 */
fuse_list_t *fuse_list_new_ex(fuse_t *fuse, const char *file, int line);

/** @brief Deallocate a fuse linked list
 *
 *  @param fuse The fuse application
 *  @param self The linked list to deallocate
 */
void fuse_list_destroy(fuse_t *fuse, fuse_list_t *self);

/** @brief Return the number of values in the list
 *
 *  @param self The linked list
 *  @returns The number of values in the list
 */
size_t fuse_list_count(fuse_list_t *self);

/** @brief Return the next value in the list
 *
 *  @param self The linked list
 *  @param cur  The current value, or NULL if the first value should be returned
 *  @returns The next value in the linked list, or NULL if there is no next value
 */
fuse_value_t *fuse_list_next(fuse_list_t *self, fuse_value_t *cur);

/** @brief Return the previous value in the list
 *
 *  @param self The linked list
 *  @param ptr  The current value, or NULL if the last value should be returned
 *  @returns The previous value in the linked list, or NULL if there is no previous value
 */
fuse_value_t *fuse_list_prev(fuse_list_t *self, fuse_value_t *ptr);

/** @brief Add a value at the end of the list
 *
 *  @param self The linked list
 *  @param ptr  The new value to add to the end of the list
 *  @returns Returns true if successful, false otherwise
 */
fuse_value_t *fuse_list_push(fuse_list_t *self, fuse_value_t *ptr);

/** @brief Remove a value from the beginning of the list
 *
 *  @param self The linked list
 *  @returns Returns the value removed from the beginning of the list, or NULL if the list is empty
 */
fuse_value_t *fuse_list_pop(fuse_list_t *self);

#endif // FUSE_LIST_H
