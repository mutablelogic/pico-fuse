
#include <alloc/alloc.h>
#include <stdio.h>

void fuse_allocator_walk_callback(void *ptr, size_t size, uint16_t magic, const char *file, int line, void *data)
{
    uint32_t *count = (uint32_t *)data;
    (*count)++;
}

int TEST_001()
{
    fuse_allocator_t *builtin = fuse_allocator_builtin_new();
    assert(builtin != NULL);
    fuse_allocator_destroy(builtin);
    return 0;
}

int TEST_002()
{
    fuse_allocator_t *pool = fuse_allocator_builtin_new();
    assert(pool != NULL);

    void *ptrs[64];

    // Create 64 memory blocks
    for (int i = 0; i < 64; i++)
    {
        void *ptr = fuse_allocator_malloc(pool, 1024, 0xFF, __FILE__, __LINE__);
        assert(ptr != NULL);
        ptrs[i] = ptr;
    }

    // Walk
    void *ctx = NULL;
    uint32_t count = 0;
    while ((ctx = fuse_allocator_walk(pool, ctx, fuse_allocator_walk_callback, &count)) != NULL)
    {
        // Do nothing
    }
    assert(count == 64);

    // Free 64 memory blocks
    for (int i = 0; i < 64; i++)
    {
        fuse_allocator_free(pool, ptrs[i]);
    }

    count = 0;
    ctx = NULL;
    while ((ctx = fuse_allocator_walk(pool, ctx, fuse_allocator_walk_callback, &count)) != NULL)
    {
        // Do nothing
    }
    assert(count == 0);

    // Destroy the pool
    fuse_allocator_destroy(pool);
    return 0;
}

int main()
{
    assert(TEST_001() == 0);
    assert(TEST_002() == 0);

    // Return success
    return 0;
}
