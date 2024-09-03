
#include <fuse/fuse.h>
#include <stdio.h>

#define START_TEST(name) \
    int name##_##app##_test() { \
        printf("Starting test: %s\n", #name);
        fuse_t *fuse = fuse_new();

#define END_TEST() \
        return fuse_destroy(fuse); \
    }

START_TEST(list_001)
    // Create a list value
    fuse_value_t *list = fuse_value_new_list(fuse);
    assert(list != NULL);
    fuse_value_retain(fuse, list);

    // Debug
    char buffer[32];
    assert_cstr_eq("[ ]", fuse_value_cstr(fuse, list, buffer, 32));
    fuse_debugf("buffer=%s\n", fuse_value_cstr(fuse, list, buffer, 32));

    // Free memory
    fuse_value_release(fuse, list);
END_TEST()

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
    fuse_debugf("buffer=%s\n", fuse_value_cstr(app, list, buffer, 32));

    // Free memory
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
    fuse_value_append(app, list, null);

    // Debug
    char buffer[32];
    assert_cstr_eq("[ null ]", fuse_value_cstr(app, list, buffer, 32));
    fuse_debugf("buffer=%s\n", fuse_value_cstr(app, list, buffer, 32));

    // Free memory
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
    fuse_value_append(app, list, fuse_value_new_null(app));
    fuse_value_append(app, list, fuse_value_new_null(app));

    // Debug
    char buffer[32];
    assert_cstr_eq("[ null, null ]", fuse_value_cstr(app, list, buffer, 32));
    fuse_debugf("buffer=%s\n", fuse_value_cstr(app, list, buffer, 32));

    // Free memory
    fuse_value_release(app, list);

    // Release resources
    return fuse_destroy(app);
}

int TEST_list_004()
{
    fuse_t *app = fuse_new();

    // Create a list value
    fuse_value_t *list = fuse_value_new_list(app);
    assert(list != NULL);
    fuse_value_retain(app, list);

    // Append zero-value scalars
    fuse_value_append(app, list, fuse_value_new_null(app));
    fuse_value_append(app, list, fuse_value_new_u8(app));
    fuse_value_append(app, list, fuse_value_new_u16(app));
    fuse_value_append(app, list, fuse_value_new_u32(app));
    fuse_value_append(app, list, fuse_value_new_u64(app));
    fuse_value_append(app, list, fuse_value_new_bool(app));

    // Check count
    assert(fuse_value_count(app, list) == 6);

    // Check contents
    char buffer[32];
    assert_cstr_eq("[ null, 0, 0, 0, 0, false ]", fuse_value_cstr(app, list, buffer, 32));
    fuse_debugf("buffer=%s\n", fuse_value_cstr(app, list, buffer, 32));

    // Free memory
    fuse_value_release(app, list);

    // Release resources
    return fuse_destroy(app);
}

int TEST_list_005()
{
    fuse_t *app = fuse_new();

    // Create a list value
    fuse_value_t *list = fuse_value_new_list(app);
    assert(list != NULL);
    fuse_value_retain(app, list);

    // Append zero-value scalars
    fuse_value_append(app, list, fuse_value_new_s8(app));
    fuse_value_append(app, list, fuse_value_new_s16(app));
    fuse_value_append(app, list, fuse_value_new_s32(app));
    fuse_value_append(app, list, fuse_value_new_s64(app));

    // Check count
    assert(fuse_value_count(app, list) == 4);

    // Check contents
    char buffer[32];
    assert_cstr_eq("[ 0, 0, 0, 0 ]", fuse_value_cstr(app, list, buffer, 32));
    fuse_debugf("buffer=%s\n", fuse_value_cstr(app, list, buffer, 32));

    // Free memory
    fuse_value_release(app, list);

    // Release resources
    return fuse_destroy(app);
}

int TEST_list_006()
{
    fuse_t *app = fuse_new();

    // Create a list value
    fuse_value_t *list = fuse_value_new_list(app);
    assert(list != NULL);
    fuse_value_retain(app, list);

    // Append zero-value scalars
    fuse_value_append(app, list, fuse_value_new_f32(app));
    fuse_value_append(app, list, fuse_value_new_f64(app));

    // Check count
    assert(fuse_value_count(app, list) == 2);

    // Check contents
    char buffer[32];
    assert_cstr_eq("[ 0, 0 ]", fuse_value_cstr(app, list, buffer, 32));
    fuse_debugf("buffer=%s\n", fuse_value_cstr(app, list, buffer, 32));

    // Free memory
    fuse_value_release(app, list);

    // Release resources
    return fuse_destroy(app);
}
