#include <stddef.h>
#include <assert.h>
#include <stdlib.h>

#include <pico/stdlib.h>
#include <fuse/fuse.h>
#include "main.h"

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

/*
 * Create a new fuse application
 */
fuse_t *fuse_init(size_t size, fuse_flag_t flags)
{
    assert(size > 0);

    // Initialize stdio
    stdio_init_all();
    sleep_ms(1000);

    // Create a memory pool
    fuse_pool_t *pool = fuse_pool_new(size);
    if (pool == NULL)
    {
        return NULL;
    }

    // Allocate instance
    fuse_t *self = (fuse_t *)fuse_pool_alloc(pool, sizeof(fuse_t));
    if (self == NULL)
    {
        fuse_pool_delete(pool);
        return NULL;
    }

    // Set the instance properties
    self->pool = pool;
    self->flags = flags;

    // Return success
    return self;
}

/*
 * Free resources from a fuse application, and stop the application
 */
int fuse_exit(fuse_t *fuse)
{
    assert(fuse);
    assert(fuse->pool);

    // Set the return code
    int status = fuse->status;

    // Clear resources
    fuse_pool_delete(fuse->pool);

    // Return the return code from the application
    return status;
}

/*
 * Check for existence of fuse flags
 */
inline bool fuse_is(fuse_t *fuse, fuse_flag_t flag)
{
    assert(fuse);
    return (fuse->flags & flag) == flag;
}

/*
 * Register a driver with the fuse application
 */
void *fuse_register_driver(fuse_t *fuse, const fuse_driver_params_t *params, void *user_data)
{
    assert(fuse);
    assert(params->init);

    // Call the driver's init function
    void *instance = params->init(fuse, user_data);
    if (instance == NULL)
    {
        return NULL;
    }

    return instance;
}

/*
 * Process events until the application is stopped
 */
void fuse_main(fuse_t *fuse)
{
    assert(fuse);

    // Continue until the application is stopped
    int i = 0;
    while (fuse->status == 0)
    {
        fuse_debugf(fuse, "%04d Running loop\n", i++);
        sleep_ms(1000);
    }
}
