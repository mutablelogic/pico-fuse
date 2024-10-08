
#include <fuse/fuse.h>
#include <stdio.h>

int TEST_null_001()
{
    fuse_t *app = fuse_new();

    // Create a null value
    fuse_value_t *null = fuse_value_new_null(app);
    assert(null != NULL);
    fuse_value_retain(app, null);

    // Debug
    char buffer[32];
    assert_cstr_eq("null", fuse_value_cstr(app, null, buffer, 32));
    fuse_debugf("buffer=%s\n",buffer);

    // Free memory
    fuse_value_release(app, null);

    // Release resources
    return fuse_destroy(app);
}
