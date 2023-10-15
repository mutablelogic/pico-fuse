
#include <fuse/fuse.h>

int main()
{
    fuse_debugf(NULL, "Allocating 64k bytes in the pool\n");
    fuse_pool_t* pool = fuse_pool_new(64 * 1024, 0);
    assert(pool);
    fuse_pool_destroy(pool);

    // Return success
    return 0;
}

