#include <fuse/fuse.h>
#include <stdio.h>

int TEST_001(fuse_t *self)
{
    const int sz = 128;
    char buf[128];

    // Test quoted values
    fuse_debugf(self, "TEST_001 base64\n");

    // Test empty string
    {
        size_t n = b64tostr(buf, sz, "", 0);
        fuse_debugf(self, "  n=%u buf=%s\n", n, buf);
        assert(n == 0); // null
        assert_cstr_eq("", buf);
    }

    // Test "Many hands make light work."
    {
        const char *in = "Many hands make light work.";
        const char *out = "TWFueSBoYW5kcyBtYWtlIGxpZ2h0IHdvcmsu";
        size_t n = b64tostr(buf, sz, (void *)in, strlen(in));
        fuse_debugf(self, "  n=%u buf=%s\n", n, buf);
        assert(n == strlen(out)); // null
        assert_cstr_eq(out, buf);
    }

    // Test "light work."
    {
        const char *in = "light work.";
        const char *out = "bGlnaHQgd29yay4=";
        size_t n = b64tostr(buf, sz, (void *)in, strlen(in));
        fuse_debugf(self, "  n=%u buf=%s\n", n, buf);
        assert(n == strlen(out)); // null
        assert_cstr_eq(out, buf);
    }

    // Test "light work"
    {
        const char *in = "light work";
        const char *out = "bGlnaHQgd29yaw==";
        size_t n = b64tostr(buf, sz, (void *)in, strlen(in));
        fuse_debugf(self, "  n=%u buf=%s\n", n, buf);
        assert(n == strlen(out)); // null
        assert_cstr_eq(out, buf);
    }

    // Test "light wor"
    {
        const char *in = "light wor";
        const char *out = "bGlnaHQgd29y";
        size_t n = b64tostr(buf, sz, (void *)in, strlen(in));
        fuse_debugf(self, "  n=%u buf=%s\n", n, buf);
        assert(n == strlen(out)); // null
        assert_cstr_eq(out, buf);
    }

    // Test "light wo"
    {
        const char *in = "light wo";
        const char *out = "bGlnaHQgd28=";
        size_t n = b64tostr(buf, sz, (void *)in, strlen(in));
        fuse_debugf(self, "  n=%u buf=%s\n", n, buf);
        assert(n == strlen(out)); // null
        assert_cstr_eq(out, buf);
    }

    // Test "light w"
    {
        const char *in = "light w";
        const char *out = "bGlnaHQgdw==";
        size_t n = b64tostr(buf, sz, (void *)in, strlen(in));
        fuse_debugf(self, "  n=%u buf=%s\n", n, buf);
        assert(n == strlen(out)); // null
        assert_cstr_eq(out, buf);
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
