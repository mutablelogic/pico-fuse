
int TEST_DEC_001(fuse_t *self)
{
    // Cycle through all the uint8_t values
    fuse_debugf(self,"TEST_DEC_001 utostr uint8_t\n");

    for (size_t sz = 0; sz < 10; sz++)
    {
        char *buf = fuse_alloc(self, FUSE_MAGIC_DATA, (void *)sz);
        for (uint8_t v = 0;; v++)
        {
            size_t n = utostr(sz == 0 ? NULL : buf, sz, v);
            fuse_debugf(self,"  n=%u value=%u buf[%u]=%s\n", n, v, sz, buf);
            assert(n > 0);
            if (v == 0)
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
            if (v == UINT8_MAX)
                break;
        }
        fuse_free(self, buf);
    }

    // Return success
    return 0;
}

int TEST_DEC_002(fuse_t *self)
{
    // Cycle through all the uint16_t values
    fuse_debugf(self,"TEST_DEC_002 utostr uint16_t\n");

    for (size_t sz = 0; sz < 10; sz++)
    {
        char *buf = fuse_alloc(self, FUSE_MAGIC_DATA, (void *)sz);
        for (uint16_t v = 0;; v++)
        {
            size_t n = utostr(sz == 0 ? NULL : buf, sz, v);
            fuse_debugf(self,"  n=%u value=%u buf[%u]=%s\n", n, v, sz, buf);
            assert(n > 0);
            if (sz > 8)
            {
                char outbuf[10];
                snprintf(outbuf, 10, "%u", v);
                fuse_debugf(self,"   outbuf=%s\n", outbuf);
                assert_cstr_eq(outbuf, buf);
            }
            if (v == UINT16_MAX)
                break;
        }
        fuse_free(self, buf);
    }

    // Return success
    return 0;
}

int TEST_DEC_003(fuse_t *self)
{
    // Sample uint32_t values
    fuse_debugf(self,"TEST_DEC_003 utostr uint32_t\n");

    for (size_t sz = 0; sz < 20; sz++)
    {
        char *buf = fuse_alloc(self, FUSE_MAGIC_DATA, (void *)sz);
        for (int i = 0; i < 1000; i++)
        {
            uint32_t v = rand_u32();
            size_t n = utostr(sz == 0 ? NULL : buf, sz, v);
            fuse_debugf(self,"  n=%u value=%u buf[%u]=%s\n", n, v, sz, buf);
            assert(n > 0);
            if (sz > 12)
            {
                char outbuf[20];
                snprintf(outbuf, 20, "%u", v);
                fuse_debugf(self,"   outbuf=%s\n", outbuf);
                assert_cstr_eq(outbuf, buf);
            }
        }
        fuse_free(self, buf);
    }

    // Return success
    return 0;
}

int TEST_DEC_004(fuse_t *self)
{
    // Sample uint64_t values
    fuse_debugf(self,"TEST_DEC_004 utostr uint64_t\n");

    for (size_t sz = 0; sz < 30; sz++)
    {
        char *buf = fuse_alloc(self, FUSE_MAGIC_DATA, (void *)sz);
        for (int i = 0; i < 1000; i++)
        {
            uint64_t v = rand_u64();
            size_t n = utostr(sz == 0 ? NULL : buf, sz, v);
            fuse_debugf(self,"  n=%u value=%lu buf[%u]=%s\n", n, v, sz, buf);
            assert(n > 0);
            if (sz > 30)
            {
                char outbuf[30];
                snprintf(outbuf, 30, "%llu", v);
                fuse_debugf(self,"    outbuf=%s\n", outbuf);
                assert_cstr_eq(outbuf, buf);
            }
        }
        fuse_free(self, buf);
    }

    // Return success
    return 0;
}
