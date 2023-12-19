
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
    fuse_debugf("Creating a null value\n");
    fuse_value_t* null = fuse_value_new(app, FUSE_MAGIC_NULL,0);
    assert(null != NULL);
    fuse_debugf("Retaining a null value\n");
    fuse_value_retain(app, null);

    // Debug
    fuse_debugf("null: %s\n", fuse_value_cstr(app, null, NULL, 0));

    // Free memory
    fuse_debugf("Releasing a null value\n");
    fuse_value_release(app, null);

    // Release resources
    return fuse_destroy(app);
}

int TEST_003()
{
    fuse_t *app = fuse_new();

    // Create a list value
    fuse_value_t* list = fuse_value_new(app, FUSE_MAGIC_LIST,0);
    assert(list != NULL);
    fuse_value_retain(app, list);

    // Debug
    fuse_debugf("list: %s count: %u\n", fuse_value_cstr(app, list, NULL, 0),fuse_value_count(app, list));

    // Free memory
    fuse_debugf("Releasing a list value\n");
    fuse_value_release(app, list);

    // Release resources
    return fuse_destroy(app);
}

int main()
{
    assert(TEST_001() == 0);
    assert(TEST_002() == 0);
    assert(TEST_003() == 0);

    // Return success
    return 0;
}
