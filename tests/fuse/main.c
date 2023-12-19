
#include <alloc/fuse.h>
#include <stdio.h>

int TEST_001()
{
    fuse_t *app = fuse_new();
    assert(app != NULL);
    return fuse_destroy(app);
}

int TEST_002()
{
    fuse_t *app = fuse_new();

    // Create a null value
    fuse_value_t* null = fuse_value_new(app, FUSE_MAGIC_NULL,0);
    assert(null != NULL);

    // Debug
    fuse_debugf("null: %s\n", fuse_value_cstr(app, null, NULL, 0));

    // Release the value
    fuse_value_release(app, null);

    // Release resources
    return fuse_destroy(app);
}

int main()
{
    assert(TEST_001() == 0);
    assert(TEST_002() == 0);

    // Return success
    return 0;
}
