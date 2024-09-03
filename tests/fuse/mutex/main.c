#include <fuse/fuse.h>

int TEST_001(fuse_t *self)
{
    fuse_debugf(self, "TEST_001: mutex\n");

    // Make a mutex
    fuse_mutex_t *mutex = fuse_new_mutex(self);

    // Lock mutex
    fuse_mutex_lock(mutex);

    // Unlock mutex
    fuse_mutex_unlock(mutex);

    // Return success
    return 0;
}

int main()
{
    fuse_t *self = fuse_new();
    assert(self);
    assert(TEST_001(self) == 0);
    assert(fuse_destroy(self) == 0);
}
