
int TEST_BIN_001(fuse_t *self)
{
    // Cycle through all the uint8_t values
    fuse_debugf("TEST_BIN_001 utoa_binary uint8_t\n");

    for (size_t sz = 0; sz < 12; sz++)
    {
        char *buf = fuse_alloc(self, FUSE_MAGIC_DATA, (void *)sz);
        for (uint8_t v = 0;; v++)
        {
            size_t n = utoa_binary(sz == 0 ? NULL : buf, sz, v, 8);
            fuse_debugf("  n=%u value=%u buf[%u]=%s\n", n, v, sz, sz == 0 ? NULL : buf);
            assert(n == 8);
            if (v == UINT8_MAX)
                break;
        }
        fuse_free(self, buf);
    }

    // Return success
    return 0;
}

int TEST_BIN_002(fuse_t *self)
{
    // Cycle through all the uint16_t values
    fuse_debugf("TEST_BIN_002 utoa_binary uint16_t\n");

    for (size_t sz = 0; sz < 20; sz++)
    {
        char *buf = fuse_alloc(self, FUSE_MAGIC_DATA, (void *)sz);
        for (uint16_t v = 0;; v++)
        {
            size_t n = utoa_binary(sz == 0 ? NULL : buf, sz, v, 0);
            fuse_debugf("  n=%u value=%u buf[%u]=%s\n", n, v, sz, sz == 0 ? NULL : buf);
            if (v == UINT16_MAX)
                break;
        }
        fuse_free(self, buf);
    }

    // Return success
    return 0;
}

int TEST_BIN_003(fuse_t *self)
{
    // Cycle through all the uint16_t values, but only the lowest 8 bits
    fuse_debugf("TEST_BIN_003 utoa_binary uint16_t\n");

    for (size_t sz = 0; sz < 22; sz++)
    {
        char *buf = fuse_alloc(self, FUSE_MAGIC_DATA, (void *)sz);
        for (uint16_t v = 0;; v++)
        {
            size_t n = utoa_binary(sz == 0 ? NULL : buf, sz, v, 8);
            fuse_debugf("  n=%u value=%u buf[%u]=%s\n", n, v, sz, sz == 0 ? NULL : buf);
            assert(n == 8);
            
            if (v == UINT16_MAX)
                break;
        }
        fuse_free(self, buf);
    }

    // Return success
    return 0;
}
