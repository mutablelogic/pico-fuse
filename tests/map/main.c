
#include <fuse/fuse.h>

int TEST_001()
{
    fuse_debugf(NULL, "Creating a fuse application\n");
    fuse_t *fuse = fuse_new(FUSE_FLAG_DEBUG);
    assert(fuse);

    // Create a map and then destroy it
    fuse_debugf(NULL, "Creating a map of 10 items\n");
    fuse_map_t *map = fuse_map_new(fuse, 10);
    assert(map);

    // Number of items should be zero
    size_t size;
    size_t count = fuse_map_stats(map, NULL);
    assert(count == 0);

    count = fuse_map_stats(map, &size);
    assert(count == 0);
    assert(size == 10);

    // Destroy the map
    fuse_map_destroy(fuse, map);

    // Return
    return fuse_destroy(fuse);
}

int TEST_002(uint64_t sz)
{
    fuse_debugf(NULL, "Creating a fuse application\n");
    fuse_t *fuse = fuse_new(FUSE_FLAG_DEBUG);
    assert(fuse);

    // Create a map
    fuse_debugf(NULL, "Creating a map of %lu items\n",sz);
    fuse_map_t *map = fuse_map_new(fuse, sz);
    assert(map);

    // Add items
    for (uint64_t i = 1; i <= sz; i++)
    {
        fuse_debugf(NULL, "Adding item %lu=> %lu\n", i, i);
        assert(fuse_map_set(map, (void *)i, (void *)i));
        assert(fuse_map_stats(map, NULL) == i);
    }

    // Get items
    for (uint64_t i = 1; i <= sz; i++)
    {
        void *j = fuse_map_get(map, (void *)i);
        assert(j == (void *)i);
        fuse_debugf(NULL, "Getting item %lu => %lu\n", i, j);
    }

    // Deleting items
    for (uint64_t i = 1; i <= sz; i++)
    {
        assert(fuse_map_set(map, (void *)i, 0));
        void *j = fuse_map_get(map, (void *)i);
        fuse_debugf(NULL, "Deleting item %lu => %lu (should be zero)\n", i, j);
        assert(j == 0);
        assert(fuse_map_stats(map, NULL) == sz - i);
    }

    // Destroy the map
    fuse_map_destroy(fuse, map);

    // Return
    return fuse_destroy(fuse);
}

int main()
{
    assert(TEST_001() == 0);
    assert(TEST_002(10) == 0);
    assert(TEST_002(100) == 0);
    assert(TEST_002(1000) == 0);

    // Return success
    return 0;
}
