
#include <fuse/fuse.h>
#include <stdio.h>

int TEST_scalar_001()
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


int TEST_scalar_002()
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


int TEST_scalar_003()
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

int TEST_scalar_004()
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

int TEST_scalar_005()
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

int TEST_scalar_006()
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


int TEST_scalar_007()
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

int TEST_scalar_008()
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

int TEST_scalar_009()
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

int TEST_scalar_010()
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

int TEST_scalar_011()
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
