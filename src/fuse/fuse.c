#include <stdlib.h>
#include <fuse/fuse.h>

// Private includes
#include "fuse.h"

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

/*
 * Create a new fuse application
 */
fuse_t *fuse_init(fuse_flag_t flags)
{
    // Allocate instance
    fuse_t *self = (fuse_t *)malloc(sizeof(struct fuse_instance));
    if (self == NULL)
    {
        return NULL;
    }

    // Set the instance properties
    self->flags = flags;

    // Return success
    return self;
}

/*
 * Free resources from a fuse application, and stop the application
 */
void fuse_destroy(fuse_t *fuse)
{
    assert(fuse);

    // Free the application
    free(fuse);
}

/*
 * Check for existence of fuse flags
 */
inline bool fuse_is(fuse_t *fuse, fuse_flag_t flag)
{
    assert(fuse);
    return (fuse->flags & flag) == flag;
}
