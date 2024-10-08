
#include <fuse/fuse.h>
#if TARGET == pico
#include <picofuse/picofuse.h>
#endif

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

void TEST_003()
{
    fuse_debugf(NULL, "Allocating 64k bytes in the pool\n");
    fuse_pool_t *pool = fuse_pool_new(64 * 1024, FUSE_FLAG_DEBUG);
    assert(pool);

    // Allocate ten regions
    for(int i = 0; i < 10; i++) {
        size_t size = 1024 * i;
        fuse_debugf(NULL, "%d: Allocating %lu bytes from the pool\n",i,size);
        void *ptr = fuse_pool_alloc(pool, size);
        assert(ptr);

        // Free memory
        fuse_pool_free(pool, ptr);
    }

    // Destroy the pool    
    fuse_pool_destroy(pool);
}


void TEST_004()
{
    fuse_debugf(NULL, "Allocating 64k bytes in the pool\n");
    fuse_pool_t *pool = fuse_pool_new(64 * 1024, FUSE_FLAG_DEBUG);
    assert(pool);

    // Store pointers
    const int count = 10;
    void *ptrs[count];

    // Allocate ten regions
    for(int i = 0; i < count; i++) {
        size_t size = 1024 * i;
        fuse_debugf(NULL, "%d: Allocating %lu bytes from the pool\n",i,size);
        void *ptr = fuse_pool_alloc(pool, size);
        assert(ptr);
        fuse_debugf(NULL, "  ptr %p\n",ptr);
        ptrs[i] = ptr;
    }

    // Free ten regions - forwards
    for(int i = 0; i < count; i++) {
        fuse_debugf(NULL, "%d: Free\n",i);
        fuse_pool_free(pool, ptrs[i]);
    }

    // Destroy the pool    
    fuse_pool_destroy(pool);
}


void TEST_005()
{
    fuse_debugf(NULL, "Allocating 64k bytes in the pool\n");
    fuse_pool_t *pool = fuse_pool_new(64 * 1024, FUSE_FLAG_DEBUG);
    assert(pool);

    // Store pointers
    const int count = 10;
    void *ptrs[count];

    // Allocate ten regions
    for(int i = 0; i < count; i++) {
        size_t size = 1024 * i;
        fuse_debugf(NULL, "%d: Allocating %lu bytes from the pool\n",i,size);
        void *ptr = fuse_pool_alloc(pool, size);
        assert(ptr);
        fuse_debugf(NULL, "  ptr %p\n",ptr);
        ptrs[i] = ptr;
    }

    // Free ten regions - forwards
    for(int i = count -1 ; i >= 0; i--) {
        fuse_debugf(NULL, "%d: Free\n",i);
        fuse_pool_free(pool, ptrs[i]);
    }

    // Destroy the pool    
    fuse_pool_destroy(pool);
}

int main()
{    
    TEST_001();
    TEST_002();
    TEST_003();
    TEST_004();
    TEST_005();

    // Endless loop on the pico
#ifdef TARGET_PICO    
    while(1) {
        sleep_ms(1000);
    }
#endif

    // Return success
    return 0;
}
