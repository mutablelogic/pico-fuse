
#include <fuse/fuse.h>

int TEST_001()
{
    fuse_debugf(NULL, "Creating a fuse application\n");
    fuse_t* fuse = fuse_new(FUSE_FLAG_DEBUG);
    assert(fuse);
    return fuse_destroy(fuse);
}

int TEST_002()
{
    fuse_debugf(NULL, "Creating a fuse application\n");
    fuse_t* fuse = fuse_new(FUSE_FLAG_DEBUG);
    assert(fuse);

    // Allocate two memory blocks and free them
    void* ptr1 = fuse_alloc(fuse,32 * 1024);
    assert(ptr1);
    void* ptr2 = fuse_alloc(fuse,32 * 1024);
    assert(ptr2);

    // Free them reverse
    fuse_free(fuse,ptr2);
    fuse_free(fuse,ptr1);

    // Release app resources
    return fuse_destroy(fuse);
}

int main()
{
    assert(TEST_001() == 0);
    assert(TEST_002() == 0);

    // Return success
    return 0;
}
