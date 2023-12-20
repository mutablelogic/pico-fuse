
#include <fuse/fuse.h>
#include <stdio.h>

int TEST_list_001()
{
    fuse_t *app = fuse_new();

    // Create a list value
    fuse_value_t *list = fuse_value_new_list(app);
    assert(list != NULL);
    fuse_value_retain(app, list);

    // Debug
    char buffer[32];
    assert_cstr_eq("[ ]", fuse_value_cstr(app, list, buffer, 32));

    // Free memory
    fuse_debugf("Releasing a list value\n");
    fuse_value_release(app, list);

    // Release resources
    return fuse_destroy(app);
}

int TEST_list_002()
{
    fuse_t *app = fuse_new();

    // Create a list value
    fuse_value_t *list = fuse_value_new_list(app);
    assert(list != NULL);
    fuse_value_retain(app, list);

    // Create a null value
    fuse_value_t *null = fuse_value_new_null(app);
    assert(null != NULL);

    // Append null to list
    fuse_debugf("Appending a null value to a list\n");
    fuse_value_append(app, list, null);
    
    // Debug
    char buffer[32];
    assert_cstr_eq("[ null ]", fuse_value_cstr(app, list, buffer, 32));

    // Free memory
    fuse_debugf("Releasing a list value\n");
    fuse_value_release(app, list);

    // Release resources
    return fuse_destroy(app);
}

int TEST_list_003()
{
    fuse_t *app = fuse_new();

    // Create a list value
    fuse_value_t *list = fuse_value_new_list(app);
    assert(list != NULL);
    fuse_value_retain(app, list);

    // Append two null values to list
    fuse_debugf("Appending two null values to a list\n");
    fuse_value_append(app, list, fuse_value_new_null(app));
    fuse_value_append(app, list, fuse_value_new_null(app));

    // Debug
    char buffer[32];
    assert_cstr_eq("[ null, null ]", fuse_value_cstr(app, list, buffer, 32));

    // Free memory
    fuse_debugf("Releasing a list value\n");
    fuse_value_release(app, list);

    // Release resources
    return fuse_destroy(app);
}
