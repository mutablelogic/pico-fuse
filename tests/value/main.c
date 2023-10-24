
#include <fuse/fuse.h>
#include <string.h>

int TEST_001()
{
    fuse_debugf(NULL, "Creating a fuse application\n");
    fuse_t *fuse = fuse_new(FUSE_FLAG_DEBUG);
    assert(fuse);

    // Create a NULL value
    fuse_debugf(NULL, "Creating a NULL value\n");
    fuse_value_t *value = fuse_new_null(fuse);
    assert(value);
    assert(fuse_equal_null(value));
    fuse_debugf(NULL, "NULL==%s\n", fuse_jsonstring(value));
    assert(strcmp(fuse_jsonstring(value), "null") == 0);

    // Return
    return fuse_destroy(fuse);
}

int TEST_002()
{
    fuse_debugf(NULL, "Creating a fuse application\n");
    fuse_t *fuse = fuse_new(FUSE_FLAG_DEBUG);
    assert(fuse);

    // Create a pointer value
    fuse_debugf(NULL, "Creating a NULL pointer value\n");
    fuse_value_t *value = fuse_new_ptr(fuse, 0);
    assert(value);
    assert(fuse_equal_null(value));
    fuse_debugf(NULL, "NULL==%s\n", fuse_jsonstring(value));
    assert(strcmp(fuse_jsonstring(value), "null") == 0);

    // Return
    return fuse_destroy(fuse);
}

int TEST_003()
{
    fuse_debugf(NULL, "Creating a fuse application\n");
    fuse_t *fuse = fuse_new(FUSE_FLAG_DEBUG);
    assert(fuse);

    // Create a pointer value
    fuse_debugf(NULL, "Creating a non-NULL pointer value\n");
    fuse_value_t *value = fuse_new_ptr(fuse, (void *)100);
    assert(value);
    assert(fuse_equal_null(value) == false);
    fuse_debugf(NULL, "value==%s\n", fuse_jsonstring(value));

    // Return
    return fuse_destroy(fuse);
}

int main()
{
    assert(TEST_001() == 0);
    assert(TEST_002() == 0);
    assert(TEST_003() == 0);

    // Return success
    return 0;
}
