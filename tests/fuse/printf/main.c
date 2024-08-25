#include <fuse/fuse.h>
#include <stdio.h>

const int n = 128;
char buf[n] = {0};

int TEST_002(fuse_t *self)
{
    const int sz = 1;
    char buf[sz];

    fuse_debugf("TEST_002 buffer with only a terminator\n");

    // sprintf the value
    assert(fuse_sprintf(self, buf, sz, "test") == 0);
    assert_cstr_eq("", buf);

    // Return success
    return 0;
}

int TEST_003(fuse_t *self)
{
    const int sz = 2;
    char buf[sz];

    fuse_debugf("TEST_003 buffer with only a %%\n");

    // sprintf the value
    assert(fuse_sprintf(self, buf, sz, "%") == 1);
    assert_cstr_eq("%", buf);

    // Return success
    return 0;
}

int TEST_004(fuse_t *self)
{
    const int sz = 2;
    char buf[sz];

    fuse_debugf("TEST_004 buffer quoting a %%\n");

    // sprintf the value
    assert(fuse_sprintf(self, buf, sz, "%%") == 1);
    assert_cstr_eq("%", buf);

    // Return success
    return 0;
}

int TEST_005(fuse_t *self)
{
    const int sz = 4;
    char buf[sz];

    fuse_debugf("TEST_005 buffer quoting a %% twice\n");

    // sprintf the value
    assert(fuse_sprintf(self, buf, sz, "%%%%") == 2);
    assert_cstr_eq("%%", buf);

    // Return success
    return 0;
}

int TEST_006(fuse_t *self)
{
    const int sz = 5;
    char buf[sz];

    fuse_debugf("TEST_006 buffer curtailment\n");

    // sprintf the value
    assert(fuse_sprintf(self, buf, sz, "hello, world") == 4);
    assert_cstr_eq("hell", buf);

    // Return success
    return 0;
}

int TEST_007(fuse_t *self)
{
    const int sz = 5;
    char buf[sz];

    fuse_debugf("TEST_007 buffer curtailment with string\n");

    // sprintf the value
    assert(fuse_sprintf(self, buf, sz, "%s", "hello, world") == 4);
    assert_cstr_eq("hell", buf);

    // Return success
    return 0;
}

int TEST_008(fuse_t *self)
{
    const int sz = 7;
    char buf[sz];

    fuse_debugf("TEST_008 null string\n");

    // sprintf the value
    assert(fuse_sprintf(self, buf, sz, "%s", NULL) == 6);
    assert_cstr_eq("<NULL>", buf);

    // Return success
    return 0;
}

int TEST_009(fuse_t *self)
{
    const int sz = 7;
    char inbuf[sz];
    char outbuf[sz];

    fuse_debugf("TEST_009 (uint8) string\n");

    for (uint8_t i = 0; i <= UINT8_MAX; i++)
    {
        assert(fuse_sprintf(self, inbuf, sz, "%u", i) != 0);
        assert(snprintf(outbuf, sz, "%u", i) != 0);
        assert_cstr_eq(outbuf, inbuf);
        if (i == UINT8_MAX)
            break;
    }

    // Return success
    return 0;
}

int TEST_010(fuse_t *self)
{
    const int sz = 7;
    char inbuf[sz];
    char outbuf[sz];

    fuse_debugf("TEST_010 (int8) string\n");

    for (int8_t i = INT8_MIN; i <= INT8_MAX; i++)
    {
        assert(fuse_sprintf(self, inbuf, sz, "%d", i) != 0);
        assert(snprintf(outbuf, sz, "%d", i) != 0);
        assert_cstr_eq(outbuf, inbuf);
        if (i == INT8_MAX)
            break;
    }

    // Return success
    return 0;
}

int TEST_011(fuse_t *self)
{
    const int sz = 7;
    char inbuf[sz];
    char outbuf[sz];

    fuse_debugf("TEST_011 (int16) string\n");

    for (int16_t i = INT16_MIN; i <= INT16_MAX; i++)
    {
        assert(fuse_sprintf(self, inbuf, sz, "%d", i) != 0);
        assert(snprintf(outbuf, sz, "%d", i) != 0);
        assert_cstr_eq(outbuf, inbuf);
        if (i == INT16_MAX)
            break;
    }

    // Return success
    return 0;
}

int TEST_012(fuse_t *self)
{
    const int sz = 12;
    char inbuf[sz];
    char outbuf[sz];

    fuse_debugf("TEST_012 (int32) string\n");

    for (int i = 0; i < 1000; i++)
    {
        uint32_t u32 = rand_u32();
        int32_t v = (int32_t)((u32 >> 1) ^ (-(u32 & 1)));
        assert(fuse_sprintf(self, inbuf, sz, "%d", v) != 0);
        assert(snprintf(outbuf, sz, "%d", v) != 0);
        assert_cstr_eq(outbuf, inbuf);
    }

    // Return success
    return 0;
}

int TEST_013(fuse_t *self)
{
    const int sz = 24;
    char inbuf[sz];
    char outbuf[sz];

    fuse_debugf("TEST_013 (int64) string\n");

    for (int i = 0; i < 1000; i++)
    {
        uint64_t u64 = rand_u64();
        int64_t v = (int64_t)((u64 >> 1) ^ (-(u64 & 1)));
        assert(fuse_sprintf(self, inbuf, sz, "%ld", v) != 0);
        assert(snprintf(outbuf, sz, "%lld", v) != 0);
        assert_cstr_eq(outbuf, inbuf);
    }

    // Return success
    return 0;
}

int TEST_014(fuse_t *self)
{
    const int sz = 24;
    char inbuf[sz];
    char outbuf[sz];

    fuse_debugf("TEST_014 (uint16) string\n");

    for (int i = 0; i < 1000; i++)
    {
        uint16_t v = rand_u64() & 0xFFFF;
        assert(fuse_sprintf(self, inbuf, sz, "%u", v) != 0);
        assert(snprintf(outbuf, sz, "%u", v) != 0);
        assert_cstr_eq(outbuf, inbuf);
    }

    // Return success
    return 0;
}

int TEST_015(fuse_t *self)
{
    const int sz = 24;
    char inbuf[sz];
    char outbuf[sz];

    fuse_debugf("TEST_015 (uint16) hex string\n");

    for (int i = 0; i < 1000; i++)
    {
        uint16_t v = rand_u64() & 0xFFFF;
        assert(fuse_sprintf(self, inbuf, sz, "%x", v) != 0);
        assert(snprintf(outbuf, sz, "%x", v) != 0);
        assert_cstr_eq(outbuf, inbuf);
    }

    // Return success
    return 0;
}

int TEST_016(fuse_t *self)
{
    const int sz = 24;
    char inbuf[sz];
    char outbuf[sz];

    fuse_debugf("TEST_016 (uint32) uppercase hex string\n");

    for (int i = 0; i < 1000; i++)
    {
        uint32_t v = rand_u64() & 0xFFFFFFFF;
        assert(fuse_sprintf(self, inbuf, sz, "%X", v) != 0);
        assert(snprintf(outbuf, sz, "%X", v) != 0);
        assert_cstr_eq(outbuf, inbuf);
    }

    // Return success
    return 0;
}

int TEST_017(fuse_t *self)
{
    const int sz = 24;
    char inbuf[sz];
    char outbuf[sz];

    fuse_debugf("TEST_017 (uint64) string\n");

    for (int i = 0; i < 1000; i++)
    {
        uint64_t v = rand_u64();
        assert(fuse_sprintf(self, inbuf, sz, "%lu", v) != 0);
        assert(snprintf(outbuf, sz, "%llu", v) != 0);
        assert_cstr_eq(outbuf, inbuf);
    }

    // Return success
    return 0;
}

int TEST_018(fuse_t *self)
{
    const int sz = 24;
    char inbuf[sz];
    char outbuf[sz];

    fuse_debugf("TEST_018 ptr\n");

    for (uintptr_t p = 0; p < 1000; p++)
    {
        assert(fuse_sprintf(self, inbuf, sz, "%p", (void *)p) != 0);
        assert(snprintf(outbuf, sz, "%p", (void *)p) != 0);
        fuse_debugf("  inbuf: %s\n", inbuf);
        fuse_debugf("  outbuf: %s\n", outbuf);
        assert_cstr_eq(outbuf, inbuf);
    }

    // Return success
    return 0;
}

int TEST_019(fuse_t *self)
{
    fuse_debugf("TEST_019 stdout\n");

    for (size_t p = 0; p < 1000; p++)
    {
        fuse_value_t *value = fuse_alloc(self, FUSE_MAGIC_DATA, (void *)p);
        for (size_t i = 0; i < p; i++)
        {
            ((char*)value)[i] = (uint8_t)i;
        }

        assert(fuse_printf(self, "%lu => %v", p, value) != 0);
        fuse_free(self, value);
    }

    // Return success
    return 0;
}

int main()
{
    fuse_t *self = fuse_new();
    assert(self);
    assert(TEST_002(self) == 0);
    assert(TEST_003(self) == 0);
    assert(TEST_004(self) == 0);
    assert(TEST_005(self) == 0);
    assert(TEST_006(self) == 0);
    assert(TEST_007(self) == 0);
    assert(TEST_008(self) == 0);
    assert(TEST_009(self) == 0);
    assert(TEST_010(self) == 0);
    assert(TEST_011(self) == 0);
    assert(TEST_012(self) == 0);
    assert(TEST_013(self) == 0);
    assert(TEST_014(self) == 0);
    assert(TEST_015(self) == 0);
    assert(TEST_016(self) == 0);
    assert(TEST_017(self) == 0);
    assert(TEST_018(self) == 0);
    assert(TEST_019(self) == 0);
    assert(fuse_destroy(self) == 0);
}
