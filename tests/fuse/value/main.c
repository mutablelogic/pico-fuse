#include <fuse/fuse.h>

const int n = 128;
char buf[128];

int TEST_001(fuse_t *self)
{
    fuse_debugf("NULL value\n");

    // Make and free a NULL value
    fuse_value_t *value = fuse_alloc(self, FUSE_MAGIC_NULL, NULL);

    // sprintf the value
    assert(fuse_sprintf(self, buf, n, "%v", value) > 0);
    fuse_debugf("  value=%s\n", buf);
    assert_cstr_eq("(null)", buf);

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
    fuse_value_t *value = fuse_alloc(self, FUSE_MAGIC_BOOL, (void *)true);

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
    fuse_value_t *value = fuse_alloc(self, FUSE_MAGIC_BOOL, (void *)false);

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
    }

    // Free the value
    fuse_free(self, value);

    // Return success
    return 0;
}

int TEST_005(fuse_t *self)
{
    fuse_debugf("int16_t value\n");

    // Make a S16 value
    fuse_value_t *value = fuse_alloc(self, FUSE_MAGIC_S16, NULL);

    // Iterate over all possible values
    for (int16_t i = INT16_MIN; i < INT16_MAX; i++)
    {
        // Set the value
        *(int16_t *)value = i;

        // sprintf the value
        assert(fuse_sprintf(self, buf, n, "%v", value) > 0);
    }

    // Free the value
    fuse_free(self, value);

    // Return success
    return 0;
}

int TEST_006(fuse_t *self)
{
    fuse_debugf("int32_t value\n");

    // Make a S32 value
    fuse_value_t *value = fuse_alloc(self, FUSE_MAGIC_S32, NULL);

    // Iterate over 1000 random values
    for (int i = 0; i < 1000; i++)
    {
        // Set the value
        uint32_t u32 = rand_u32();
        *(int32_t *)value = (int32_t)((u32 >> 1) ^ (-(u32 & 1)));

        // sprintf the value
        assert(fuse_sprintf(self, buf, n, "%v", value) > 0);
    }

    // Free the value
    fuse_free(self, value);

    // Return success
    return 0;
}

int TEST_007(fuse_t *self)
{
    fuse_debugf("int64_t value\n");

    // Make a S32 value
    fuse_value_t *value = fuse_alloc(self, FUSE_MAGIC_S64, NULL);

    // Iterate over 1000 random values
    for (int i = 0; i < 1000; i++)
    {
        // Set the value
        uint64_t u64 = rand_u64();
        *(int64_t *)value = (int64_t)((u64 >> 1) ^ (-(u64 & 1)));

        // sprintf the value
        assert(fuse_sprintf(self, buf, n, "%v", value) > 0);
    }

    // Free the value
    fuse_free(self, value);

    // Return success
    return 0;
}

int TEST_008(fuse_t *self)
{
    fuse_debugf("uint8_t value\n");

    // Make a U8 value
    fuse_value_t *value = fuse_alloc(self, FUSE_MAGIC_U8, NULL);

    // Iterate over almost all uint8_t values
    for (uint8_t i = 0; i < UINT8_MAX; i++)
    {
        // Set the value
        *(uint8_t *)value = i;

        // sprintf the value
        assert(fuse_sprintf(self, buf, n, "%v", value) > 0);
    }

    // Free the value
    fuse_free(self, value);

    // Return success
    return 0;
}

int TEST_009(fuse_t *self)
{
    fuse_debugf("uint16_t value\n");

    // Make a U16 value
    fuse_value_t *value = fuse_alloc(self, FUSE_MAGIC_U16, NULL);

    // Iterate over almost all uint8_t values
    for (uint16_t i = 0; i < UINT16_MAX; i++)
    {
        // Set the value
        *(uint16_t *)value = i;

        // sprintf the value
        assert(fuse_sprintf(self, buf, n, "%v", value) > 0);
    }

    // Free the value
    fuse_free(self, value);

    // Return success
    return 0;
}

int TEST_010(fuse_t *self)
{
    fuse_debugf("uint32_t value\n");

    // Make a U32 value
    fuse_value_t *value = fuse_alloc(self, FUSE_MAGIC_U16, NULL);

    // Iterate over almost all uint8_t values
    for (int i = 0; i < 1000; i++)
    {
        // Set the value
        *(uint32_t *)value = rand_u32();

        // sprintf the value
        assert(fuse_sprintf(self, buf, n, "%v", value) > 0);
    }

    // Free the value
    fuse_free(self, value);

    // Return success
    return 0;
}

int TEST_011(fuse_t *self)
{
    fuse_debugf("uint64_t value\n");

    // Make a U64 value
    fuse_value_t *value = fuse_alloc(self, FUSE_MAGIC_U64, NULL);

    // Iterate over almost all uint8_t values
    for (int i = 0; i < 1000; i++)
    {
        // Set the value
        *(uint64_t *)value = rand_u64();

        // sprintf the value
        assert(fuse_sprintf(self, buf, n, "%v", value) > 0);
    }

    // Free the value
    fuse_free(self, value);

    // Return success
    return 0;
}

int TEST_012(fuse_t *self)
{
    fuse_debugf("cstr value\n");

    // Make a CSTR value
    fuse_value_t *value = fuse_alloc(self, FUSE_MAGIC_CSTR, "hello, world!");

    assert(fuse_sprintf(self, buf, n, "%v", value) > 0);
    assert_cstr_eq("hello, world!", buf);

    // Free the value
    fuse_free(self, value);

    // Return success
    return 0;
}

int TEST_013(fuse_t *self)
{
    fuse_debugf("cstr value\n");

    // Make two CSTR values
    fuse_value_t *value1 = fuse_alloc(self, FUSE_MAGIC_CSTR, "hello");
    fuse_value_t *value2 = fuse_alloc(self, FUSE_MAGIC_CSTR, "world");

    assert(fuse_sprintf(self, buf, n, "%v %v", value1, value2) > 0);
    assert_cstr_eq("hello world", buf);

    // Free the values
    fuse_free(self, value1);
    fuse_free(self, value2);

    // Return success
    return 0;
}

int TEST_014(fuse_t *self)
{
    fuse_debugf("qstr value\n");

    // Make a CSTR value
    fuse_value_t *value = fuse_alloc(self, FUSE_MAGIC_CSTR, "hello, world");

    assert(fuse_sprintf(self, buf, n, "%q", value) > 0);
    assert_cstr_eq("\"hello, world\"", buf);

    // Free the values
    fuse_free(self, value);

    // Return success
    return 0;
}

int TEST_015(fuse_t *self)
{
    fuse_debugf("qstr value with control characters\n");

    // Make a CSTR value
    fuse_value_t *value = fuse_alloc(self, FUSE_MAGIC_CSTR, " \b \f \n \r \t \\ ");

    assert(fuse_sprintf(self, buf, n, "%q", value) > 0);
    assert_cstr_eq("\" \\b \\f \\n \\r \\t \\ \"", buf);

    // Free the values
    fuse_free(self, value);

    // Return success
    return 0;
}

int TEST_016(fuse_t *self)
{
    fuse_debugf("qstr value with quotes\n");

    // Make a CSTR value
    fuse_value_t *value = fuse_alloc(self, FUSE_MAGIC_CSTR, "\"hello, world\n\"");

    assert(fuse_sprintf(self, buf, n, "%q", value) > 0);
    assert_cstr_eq("\"\\\"hello, world\\n\\\"\"", buf);

    // Free the values
    fuse_free(self, value);

    // Return success
    return 0;
}

int TEST_017(fuse_t *self)
{
    fuse_debugf("qstr value with hex characters\n");

    // Make a CSTR value
    fuse_value_t *value = fuse_alloc(self, FUSE_MAGIC_CSTR, "\x01\x02");

    assert(fuse_sprintf(self, buf, n, "%q", value) > 0);
    fuse_debugf("  value=%s\n", buf);
    assert_cstr_eq("\"\\u0001\\u0002\"", buf);

    // Free the values
    fuse_free(self, value);

    // Return success
    return 0;
}

int TEST_018(fuse_t *self)
{
    fuse_debugf("data value\n");

    // Make a DATA value
    const char *data = "\x01\x02";
    fuse_value_t *value = fuse_alloc(self, FUSE_MAGIC_DATA, strlen(data) + 1);
    memcpy(value, data, strlen(data) + 1);

    // cstr
    assert(fuse_sprintf(self, buf, n, "%v", value) > 0);
    fuse_debugf("  value=%s\n", buf);
    assert_cstr_eq("010200", buf);

    // qstr
    assert(fuse_sprintf(self, buf, n, "%q", value) > 0);
    fuse_debugf("  value=%s\n", buf);
    assert_cstr_eq("AQIA", buf);

    // Free the values
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
    assert(fuse_destroy(self) == 0);
}
