
#include <fuse/fuse.h>

void TEST_001()
{
    fuse_debugf(NULL, "Allocating 64k bytes in the pool\n");
    fuse_pool_t *pool = fuse_pool_new(64 * 1024, FUSE_FLAG_DEBUG);
    assert(pool);
    fuse_pool_destroy(pool);
}

void TEST_002()
{
    fuse_debugf(NULL, "Allocating 64k bytes in the pool\n");
    fuse_pool_t *pool = fuse_pool_new(64 * 1024, FUSE_FLAG_DEBUG);
    assert(pool);

    // Allocate 32k bytes
    fuse_debugf(NULL, "Allocating 32k bytes from the pool\n");
    void *ptr = fuse_pool_alloc(pool, 32 * 1024);
    assert(ptr);

    // Free memory
    fuse_debugf(NULL, "Free memory in the pool\n");
    fuse_pool_free(pool, ptr);

    // Destroy the pool    
    fuse_pool_destroy(pool);
}

int main()
{
    TEST_001();
    TEST_002();

    // Return success
    return 0;
}
