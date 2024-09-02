#include <fuse/fuse.h>
#include <stdio.h>

int TEST_001(fuse_t *self)
{
    fuse_debugf("TEST_001 ftostr float\n");

    const int sz = 20;
    char buf[sz];
    for (size_t i = 0; i < 100; i++)
    {
        float v = (float)i / 10.0;
        size_t n = ftostr(buf, sz, v);
        fuse_debugf("  n=%u value=%.1f buf[%u]=%s\n", n, v, sz, buf);
        assert(n > 0);
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
