
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

int TEST_008()
{
    fuse_t *app = fuse_new();

    // Create a list value
    fuse_value_t *scalar = fuse_value_new_bool(app);
    assert(scalar != NULL);
    fuse_value_retain(app, scalar);

    // Debug
    char buffer[32];
    assert_cstr_eq("false", fuse_value_cstr(app, scalar, buffer, 32));

    // Free memory
    fuse_debugf("Releasing a scalar value\n");
    fuse_value_release(app, scalar);

    // Release resources
    return fuse_destroy(app);
}

int TEST_009()
{
    fuse_t *app = fuse_new();

    // Create a list value
    fuse_value_t *scalar = fuse_value_new_s8(app);
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


int TEST_010()
{
    fuse_t *app = fuse_new();

    // Create a list value
    fuse_value_t *scalar = fuse_value_new_s16(app);
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

int TEST_011()
{
    fuse_t *app = fuse_new();

    // Create a list value
    fuse_value_t *scalar = fuse_value_new_s32(app);
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

int TEST_012()
{
    fuse_t *app = fuse_new();

    // Create a list value
    fuse_value_t *scalar = fuse_value_new_s64(app);
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

int TEST_013()
{
    fuse_t *app = fuse_new();

    // Create a list value
    fuse_value_t *scalar = fuse_value_new_f32(app);
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

int TEST_014()
{
    fuse_t *app = fuse_new();

    // Create a list value
    fuse_value_t *scalar = fuse_value_new_f64(app);
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
    assert(TEST_008() == 0);
    assert(TEST_009() == 0);
    assert(TEST_010() == 0);
    assert(TEST_011() == 0);
    assert(TEST_012() == 0);
    assert(TEST_013() == 0);
    assert(TEST_014() == 0);

    // Return success
    return 0;
}
