
#include <alloc/fuse.h>
#include <stdio.h>

int TEST_001()
{
    fuse_t *app = fuse_new();
    assert(app != NULL);
    return fuse_destroy(app);
}

int main()
{
    assert(TEST_001() == 0);

    // Return success
    return 0;
}
