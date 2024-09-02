
int TEST_HEX_001(fuse_t *self)
{
    // Cycle through all the uint8_t values
    fuse_debugf("TEST_HEX_001 utostr_hex uint8_t\n");

    for (size_t sz = 0; sz < 12; sz++)
    {
        char *buf = fuse_alloc(self, FUSE_MAGIC_DATA, (void *)sz);
        for (uint8_t v = 0;; v++)
        {
            size_t n = utostr_hex(sz == 0 ? NULL : buf, sz, v, 8, false);
            fuse_debugf("  n=%u value=%u buf[%u]=%s\n", n, v, sz, sz == 0 ? NULL : buf);
            assert(n == 2);
            if (v == UINT8_MAX)
                break;
        }
        fuse_free(self, buf);
    }

    // Return success
    return 0;
}

int TEST_HEX_002(fuse_t *self)
{
    // Cycle through all the uint8_t values
    fuse_debugf("TEST_HEX_002 utostr_hex uint8_t bits=16\n");

    for (size_t sz = 0; sz < 12; sz++)
    {
        char *buf = fuse_alloc(self, FUSE_MAGIC_DATA, (void *)sz);
        for (uint8_t v = 0;; v++)
        {
            size_t n = utostr_hex(sz == 0 ? NULL : buf, sz, v, 16, false);
            fuse_debugf("  n=%u value=%u buf[%u]=%s\n", n, v, sz, sz == 0 ? NULL : buf);
            assert(n == 4);

            if (sz > 7)
            {
                char outbuf[10];
                snprintf(outbuf, 10, "%04x", v);
                fuse_debugf("   outbuf=%s\n", outbuf);
                assert_cstr_eq(outbuf, buf);
            }

            if (v == UINT8_MAX)
                break;
        }
        fuse_free(self, buf);
    }

    // Return success
    return 0;
}

int TEST_HEX_003(fuse_t *self)
{
    // Cycle through all the uint8_t values
    fuse_debugf("TEST_HEX_003 utostr_hex uint8_t bits=12 upper\n");

    for (size_t sz = 0; sz < 12; sz++)
    {
        char *buf = fuse_alloc(self, FUSE_MAGIC_DATA, (void *)sz);
        for (uint8_t v = 0;; v++)
        {
            size_t n = utostr_hex(sz == 0 ? NULL : buf, sz, v, 12, true);
            fuse_debugf("  n=%u value=%u buf[%u]=%s\n", n, v, sz, sz == 0 ? NULL : buf);
            assert(n == 3);

            if (sz > 7)
            {
                char outbuf[10];
                snprintf(outbuf, 10, "%03X", v);
                fuse_debugf("   outbuf=%s\n", outbuf);
                assert_cstr_eq(outbuf, buf);
            }

            if (v == UINT8_MAX)
                break;
        }
        fuse_free(self, buf);
    }

    // Return success
    return 0;
}
