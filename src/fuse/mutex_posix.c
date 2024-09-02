#if defined(TARGET_DARWIN) || defined(TARGET_LINUX)
#include <fuse/fuse.h>
#include <pthread.h>
#include "fuse.h"
#include "mutex.h"

///////////////////////////////////////////////////////////////////////////////
// DEFINITIONS

struct mutex_context
{
    pthread_mutex_t mutex;
};

static bool mutex_init(fuse_t *self, fuse_value_t *value, const void *user_data);

///////////////////////////////////////////////////////////////////////////////
// LIFECYCLE

/** @brief Register value primitive for a mutex
 */
void fuse_register_value_mutex(fuse_t *self)
{
    assert(self);

    // Register mutex type
    fuse_value_desc_t fuse_mutex_type = {
        .size = sizeof(struct mutex_context),
        .name = "LOCK",
        .init = mutex_init,
    };
    fuse_register_value_type(self, FUSE_MAGIC_MUTEX, fuse_mutex_type);
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE METHODS

/* @brief Initialise a mutex lock
 */
static bool mutex_init(fuse_t *self, fuse_value_t *value, const void *user_data) {
    assert(self);
    assert(value);

    // Initialize the mutex context
    struct mutex_context* ctx = (struct mutex_context*)value;
    pthread_mutex_init ( &ctx->mutex, NULL);

    // Return success
    return true;
}

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

/* @brief Lock critical section of code
 */
inline void fuse_mutex_lock(fuse_mutex_t* mutex) {
    struct mutex_context* ctx = (struct mutex_context*)mutex;
    pthread_mutex_lock(&ctx->mutex);
}

/* @brief Unlock critical section of code
 */
inline void fuse_mutex_unlock(fuse_mutex_t* mutex) {
    struct mutex_context* ctx = (struct mutex_context*)mutex;
    pthread_mutex_unlock(&ctx->mutex);
}

#endif
