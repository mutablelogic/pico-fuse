
// Includes
#include <fuse/fuse.h>
#include "list.h"

////////////////////////////////////////////////////////////////////////////////
// LIFECYCLE

/* @brief Initialise a list
 */
bool fuse_init_list(fuse_t *self, fuse_value_t *value, const void *user_data) {
    fuse_debugf("fuse_init_list\n");

    // Allocate a new list
    return true;
}

/* @brief Destroy the list
 */
void fuse_destroy_list(fuse_t *self, fuse_value_t *value) {
    fuse_debugf("fuse_destroy_list\n");
}
