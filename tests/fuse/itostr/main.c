#include <fuse/fuse.h>
#include <stdio.h>

int TEST_001(fuse_t *self)
{
    // Cycle through all the int8_t values
    fuse_debugf("TEST_001 itostr int8_t\n");

    for (size_t sz = 0; sz < 10; sz++)
    {
        char *buf = fuse_alloc(self, FUSE_MAGIC_DATA, (void *)sz);
        for (int8_t v = INT8_MIN;; v++)
        {
            size_t n = itostr(sz == 0 ? NULL : buf, sz, v);
            fuse_debugf("  n=%u value=%d buf[%u]=%s\n", n, v, sz, buf);
            assert(n > 0);
            if (v < -99)
            {
                assert(n == 4);
            }
            else if (v < -9)
            {
                assert(n == 3);
            }
            else if (v < 0)
            {
                assert(n == 2);
            }
            else if (v == 0)
            {
                assert(n == 1);
            }
            else if (v < 10)
            {
                assert(n == 1);
            }
            else if (v < 100)
            {
                assert(n == 2);
            }
            else
            {
                assert(n == 3);
            }
            if (v == INT8_MAX)
                break;
        }
        fuse_free(self, buf);
    }

    // Return success
    return 0;
}

int TEST_002(fuse_t *self)
{
    // Cycle through all the int16_t values
    fuse_debugf("TEST_002 itostr int16_t\n");

    for (size_t sz = 0; sz < 10; sz++)
    {
        char *buf = fuse_alloc(self, FUSE_MAGIC_DATA, (void *)sz);
        for (int16_t v = INT16_MIN;; v++)
        {
            size_t n = itostr(sz == 0 ? NULL : buf, sz, v);
            fuse_debugf("  n=%u value=%d buf[%u]=%s\n", n, v, sz, buf);
            assert(n > 0);
            if (sz > 8)
            {
                char outbuf[10];
                snprintf(outbuf, 10, "%d", v);
                fuse_debugf("   outbuf=%s\n", outbuf);
                assert_cstr_eq(outbuf, buf);
            }
            if (v == INT16_MAX)
                break;
        }
        fuse_free(self, buf);
    }

    // Return success
    return 0;
}

int TEST_003(fuse_t *self)
{
    // Sample int32_t values
    fuse_debugf("TEST_003 itostr int32_t\n");

    for (size_t sz = 0; sz < 20; sz++)
    {
        char *buf = fuse_alloc(self, FUSE_MAGIC_DATA, (void *)sz);
        for (int i = 0; i < 1000; i++)
        {
            uint32_t u32 = rand_u32();
            int32_t v = (int32_t)((u32 >> 1) ^ (-(u32 & 1)));
            size_t n = itostr(sz == 0 ? NULL : buf, sz, v);
            fuse_debugf("  n=%u value=%d buf[%u]=%s\n", n, v, sz, buf);
            assert(n > 0);
            if (sz > 12)
            {
                char outbuf[20];
                snprintf(outbuf, 20, "%d", v);
                fuse_debugf("   outbuf=%s\n", outbuf);
                assert_cstr_eq(outbuf, buf);
            }
        }
        fuse_free(self, buf);
    }

    // Return success
    return 0;
}

int TEST_004(fuse_t *self)
{
    // Sample int64_t values
    fuse_debugf("TEST_004 itostr int64_t\n");

    for (size_t sz = 0; sz < 40; sz++)
    {
        char *buf = fuse_alloc(self, FUSE_MAGIC_DATA, (void *)sz);
        for (int i = 0; i < 1000; i++)
        {
            uint64_t u64 = rand_u64();
            int64_t v = (int64_t)((u64 >> 1) ^ (-(u64 & 1)));
            size_t n = itostr(sz == 0 ? NULL : buf, sz, v);
            fuse_debugf("  n=%u value=%ld buf[%u]=%s\n", n, v, sz, buf);
            assert(n > 0);
            if (sz > 20)
            {
                char outbuf[40];
                snprintf(outbuf, 40, "%lld", v);
                fuse_debugf("   outbuf=%s\n", outbuf);
                assert_cstr_eq(outbuf, buf);
            }
        }
        fuse_free(self, buf);
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
    assert(TEST_004(self) == 0);
    assert(fuse_destroy(self) == 0);
}
