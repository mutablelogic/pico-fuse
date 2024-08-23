#include <fuse/fuse.h>

const int n = 128;
char buf[n] = {0};

int TEST_001(fuse_t *self)
{
    fuse_debugf("NULL value\n");

    // Make and free a NULL value
    fuse_value_t *value = fuse_alloc(self, FUSE_MAGIC_NULL, NULL);

    // sprintf the value
    assert(fuse_sprintf(self, buf, n, "%v", value) > 0);
    fuse_debugf("  value=%s\n", buf);
    assert_cstr_eq("NULL", buf);

    // sprintf the json value
    assert(fuse_sprintf(self, buf, n, "%q", value) > 0);
    fuse_debugf("  quoted=%s\n", buf);
    assert_cstr_eq("null", buf);

    // Free the value
    fuse_free(self, value);

    // Return success
    return 0;
}

int TEST_002(fuse_t *self)
{
    fuse_debugf("TRUE value\n");

    // Make and free a NULL value
    fuse_value_t *value = fuse_alloc(self, FUSE_MAGIC_BOOL, (void* )true);

    // sprintf the value
    assert(fuse_sprintf(self, buf, n, "%v", value) > 0);
    fuse_debugf("  value=%s\n", buf);
    assert_cstr_eq("true", buf);

    // Free the value
    fuse_free(self, value);

    // Return success
    return 0;
}

int TEST_003(fuse_t *self)
{
    fuse_debugf("FALSE value\n");

    // Make and free a NULL value
    fuse_value_t *value = fuse_alloc(self, FUSE_MAGIC_BOOL, (void* )false);

    // sprintf the value
    assert(fuse_sprintf(self, buf, n, "%v", value) > 0);
    fuse_debugf("  value=%s\n", buf);
    assert_cstr_eq("false", buf);

    // Free the value
    fuse_free(self, value);

    // Return success
    return 0;
}

int TEST_004(fuse_t *self)
{
    fuse_debugf("int8_t value\n");

    // Make a S8 value
    fuse_value_t *value = fuse_alloc(self, FUSE_MAGIC_S8, NULL);

    // Iterate over all possible values
    for (int8_t i = INT8_MIN; i < INT8_MAX; i++)
    {
        // Set the value
        *(int8_t *)value = i;

        // sprintf the value
        assert(fuse_sprintf(self, buf, n, "%v", value) > 0);
        fuse_debugf("  value=%s\n", buf);
    }

    // Free the value
    fuse_free(self, value);

    // Return success
    return 0;
}

int main()
{
    fuse_t *self = fuse_new();
    assert(self);
    assert(TEST_001(self) == 0);
    assert(TEST_002(self) == 0);
    assert(TEST_003(self) == 0);
    assert(TEST_004(self) == 0);
    assert(fuse_destroy(self) == 0);
}
