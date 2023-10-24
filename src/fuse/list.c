#include <string.h>
#include <fuse/fuse.h>

// Private includes
#include "list.h"

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

fuse_list_t *fuse_list_new_ex(fuse_t *fuse, const char *file, int line)
{
    assert(fuse);

    // Create the list
    struct fuse_list_instance *self = fuse_alloc_ex(fuse, sizeof(struct fuse_list_instance), file, line);
    if (self == NULL)
    {
        return NULL;
    }

    // Set the instance properties
    self->count = 0;
    self->first = NULL;
    self->last = NULL;

    // Return success
    return self;
}

void fuse_list_destroy(fuse_t *fuse, fuse_list_t *self)
{
    assert(fuse);
    assert(self);

    // Free the instance
    fuse_free(fuse, self);
}

inline size_t fuse_list_count(fuse_list_t *self)
{
    assert(self);
    return self->count;
}

