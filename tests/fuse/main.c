
#include <fuse/fuse.h>
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
    fuse_value_t *null = fuse_value_new_null(app);
    assert(null != NULL);
    fuse_debugf("Retaining a null value\n");
    fuse_value_retain(app, null);

    // Debug
    char buffer[32];
    assert_cstr_eq("null", fuse_value_cstr(app, null, buffer, 32));

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
    fuse_value_t *list = fuse_value_new_list(app);
    assert(list != NULL);
    fuse_value_retain(app, list);

    // Debug
    char buffer[32];
    assert_cstr_eq("[]", fuse_value_cstr(app, list, buffer, 32));

    // Free memory
    fuse_debugf("Releasing a list value\n");
    fuse_value_release(app, list);

    // Release resources
    return fuse_destroy(app);
}

int TEST_004()
{
    fuse_t *app = fuse_new();

    // Create a list value
    fuse_value_t *scalar = fuse_value_new_u8(app);
    assert(scalar != NULL);
    fuse_value_retain(app, scalar);

    // Debug
    char buffer[32];
    assert_cstr_eq("0", fuse_value_cstr(app, scalar, buffer, 32));

    // Free memory
    fuse_debugf("Releasing a scalar value\n");
    fuse_value_release(app, scalar);

    // Release resources
    return fuse_destroy(app);
}


int TEST_005()
{
    fuse_t *app = fuse_new();

    // Create a list value
    fuse_value_t *scalar = fuse_value_new_u16(app);
    assert(scalar != NULL);
    fuse_value_retain(app, scalar);

    // Debug
    char buffer[32];
    assert_cstr_eq("0", fuse_value_cstr(app, scalar, buffer, 32));

    // Free memory
    fuse_debugf("Releasing a scalar value\n");
    fuse_value_release(app, scalar);

    // Release resources
    return fuse_destroy(app);
}


int TEST_006()
{
    fuse_t *app = fuse_new();

    // Create a list value
    fuse_value_t *scalar = fuse_value_new_u32(app);
    assert(scalar != NULL);
    fuse_value_retain(app, scalar);

    // Debug
    char buffer[32];
    assert_cstr_eq("0", fuse_value_cstr(app, scalar, buffer, 32));

    // Free memory
    fuse_debugf("Releasing a scalar value\n");
    fuse_value_release(app, scalar);

    // Release resources
    return fuse_destroy(app);
}

int TEST_007()
{
    fuse_t *app = fuse_new();

    // Create a list value
    fuse_value_t *scalar = fuse_value_new_u64(app);
    assert(scalar != NULL);
    fuse_value_retain(app, scalar);

    // Debug
    char buffer[32];
    assert_cstr_eq("0", fuse_value_cstr(app, scalar, buffer, 32));

    // Free memory
    fuse_debugf("Releasing a scalar value\n");
    fuse_value_release(app, scalar);

    // Release resources
    return fuse_destroy(app);
}

int main()
{
    assert(TEST_001() == 0);
    assert(TEST_002() == 0);
    assert(TEST_003() == 0);
    assert(TEST_004() == 0);
    assert(TEST_005() == 0);
    assert(TEST_006() == 0);
    assert(TEST_007() == 0);

    // Return success
    return 0;
}
