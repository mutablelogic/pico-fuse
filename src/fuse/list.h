/** @file list.h
 *  @brief Private function prototypes for lists
 */
#ifndef FUSE_PRIVATE_LIST_H
#define FUSE_PRIVATE_LIST_H

/** @brief Represents a linked list
 */
struct fuse_list
{
};

/** @brief Initialize a list
 *
 *  The function will initialize a list and return the list.
 *
 *  @param self The fuse instance
 *  @param list The list to initialize
 *  @param user_data The user data (unused for lists)
 *  @returns True if the operation was successful, otherwise false
 */
bool fuse_init_list(fuse_t *self, fuse_value_t *list, const void *user_data);

/** @brief Destroy a list
 *
 *  The function will destroy a list, release all elements and free all memory associated with it.
 *
 *  @param self The fuse instance
 *  @param list The list to destroy
 */
void fuse_destroy_list(fuse_t *self, fuse_value_t *list);

#endif
