#include <fuse/fuse.h>

const int n = 128;
char buf[128];

int TEST_001(fuse_t *self)
{
    fuse_debugf("TEST_001: zero-sized list value\n");

    // Make an empty list
    fuse_value_t *value = fuse_alloc(self, FUSE_MAGIC_LIST, NULL);

    // check count
    assert(fuse_count(self, value) == 0);

    // sprintf the value
    assert(fuse_sprintf(self, buf, n, "%v", value) > 0);
    fuse_debugf("  value=%s\n", buf);
    assert_cstr_eq("[]", buf);

    // Free the list
    fuse_free(self, value);

    // Return success
    return 0;
}

int TEST_002(fuse_t *self)
{
    fuse_debugf("TEST_002: list value with 10 nulls\n");

    // Make an empty list
    // TODO: Use fuse_new_list which autoreleases the value
    fuse_value_t *value = fuse_alloc(self, FUSE_MAGIC_LIST, NULL);

    // check count
    assert(fuse_count(self, value) == 0);

    // append 10 NULL values
    for (size_t i = 0; i < 10; i++)
    {
        // TODO: Use fuse_new_null which autoreleases the value
        fuse_value_t *v = fuse_alloc(self, FUSE_MAGIC_NULL, NULL);
        assert(fuse_list_append(self, value, v) == v);
    }

    // check count
    assert(fuse_count(self, value) == 10);

    // sprintf the value
    assert(fuse_sprintf(self, buf, n, "%v", value) > 0);
    fuse_debugf("  value=%s\n", buf);
    assert_cstr_eq("[null,null,null,null,null,null,null,null,null,null]", buf);

    // Free the list (which also frees the values)
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
    assert(fuse_destroy(self) == 0);
}
