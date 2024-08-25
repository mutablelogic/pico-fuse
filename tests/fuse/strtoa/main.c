#include <fuse/fuse.h>
#include <stdio.h>

int TEST_001(fuse_t *self)
{
    const int sz = 128;
    char buf[sz] = {0};

    // Test quoted values
    fuse_debugf("TEST_001 strtoa\n");

    // Test NULL
    {
        const char *str = NULL;
        size_t n = strtoa(NULL, 0, str);
        fuse_debugf("  n=%u\n", n);
        assert(n == 4); // null
    }

    // Test NULL
    {
        const char *str = NULL;
        size_t n = strtoa(buf, sz, str);
        fuse_debugf("  n=%u buf=%s\n", n, buf);
        assert(n == 4); // null
        assert_cstr_eq("null", buf);
    }

    // Test empty string
    {
        const char *str = "";
        size_t n = strtoa(buf, sz, str);
        fuse_debugf("  n=%u buf=%s\n", n, buf);
        assert(n == 2); 
        assert_cstr_eq("\"\"", buf);
    }


    // Test hello
    {
        const char *str = "hello";
        size_t n = strtoa(buf, sz, str);
        fuse_debugf("  n=%u buf=%s\n", n, buf);
        assert(n == 7);
        assert_cstr_eq("\"hello\"", buf);
    }

    // Test single quote
    {
        const char *str = "\"";
        size_t n = strtoa(buf, sz, str);
        fuse_debugf("  n=%u buf=%s\n", n, buf);
        assert(n == 4);
        assert_cstr_eq("\"\\\"\"", buf);
    }

    // Test control characters
    {
        const char *str = "\b\f\n\r\t";
        size_t n = strtoa(buf, sz, str);
        fuse_debugf("  n=%u buf=%s\n", n, buf);
        assert(n == 12);
        assert_cstr_eq("\"\\b\\f\\n\\r\\t\"", buf);
    }    

    // Test specials
    {
        const char *str = "\x7F";
        size_t n = strtoa(buf, sz, str);
        fuse_debugf("  n=%u buf=%s\n", n, buf);
        assert(n == 8);
        assert_cstr_eq("\"\\u007F\"", buf);
    }    


    // Return success
    return 0;
}

int main()
{
    fuse_t *self = fuse_new();
    assert(self);

    assert(TEST_001(self) == 0);

    assert(fuse_destroy(self) == 0);
}
