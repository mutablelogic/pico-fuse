#include <fuse/fuse.h>

const int n = 128;
char buf[128];

int TEST_001(fuse_t *self)
{
    fuse_debugf("TEST_001: zero-sized list value\n");

    // Make an empty list
    fuse_list_t *list = fuse_new_list(self);

    // check count
    assert(fuse_list_count(self, list) == 0);

    // sprintf the value
    assert(fuse_sprintf(self, buf, n, "%v", list) > 0);
    fuse_debugf("  value=%s\n", buf);
    assert_cstr_eq("[]", buf);

    // Return success
    return 0;
}

int TEST_002(fuse_t *self)
{
    fuse_debugf("TEST_002: list value with 10 nulls\n");

    // Make an empty list
    fuse_list_t *list = fuse_new_list(self);

    // check count
    assert(fuse_list_count(self, list) == 0);

    // append 10 NULL values
    for (size_t i = 0; i < 10; i++)
    {
        // Append a new NULL value
        assert(fuse_list_append(self, list, fuse_new_null(self)));
    }

    // check count
    assert(fuse_list_count(self, list) == 10);

    // sprintf the value
    assert(fuse_sprintf(self, buf, n, "%v", list) > 0);
    fuse_debugf("  value=%s\n", buf);
    assert_cstr_eq("[null,null,null,null,null,null,null,null,null,null]", buf);

    // Return success
    return 0;
}

int TEST_003(fuse_t *self)
{
    fuse_debugf("TEST_003: push 100 values into a list and pop them\n");

    // Make an empty list
    fuse_list_t *list = fuse_new_list(self);

    // check count
    assert(fuse_list_count(self, list) == 0);

    // append 10 values
    for (uint8_t i = 0; i < 100; i++)
    {
        // Append a new value
        assert(fuse_list_push(self, list, fuse_new_u8(self, i)));
    }

    // sprintf the value
    fuse_printf(self, " list=%v\n", list);

    // check count
    assert(fuse_list_count(self, list) == 100);

    // pop
    for (uint8_t i = 0; i < 100; i++)
    {
        assert(fuse_list_count(self, list) == 100 - i);
        fuse_value_t *v = fuse_list_pop(self, list);
        assert(v);
        fuse_printf(self, " popped=%v\n", v);
    }

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
    assert(fuse_destroy(self) == 0);
}
