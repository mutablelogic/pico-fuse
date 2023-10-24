
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


int TEST_003(uint64_t sz)
{
    fuse_debugf(NULL, "Creating a fuse application\n");
    fuse_t *fuse = fuse_new(FUSE_FLAG_DEBUG);
    assert(fuse);

    // Generate some random numbers between 0 and sz
    uint64_t *numbers = fuse_alloc(fuse, sizeof(uint64_t) * sz);
    assert(numbers);
    for(uint64_t i = 0; i < sz; i++) {
        uint64_t r = 0;
        while(r==0) {
            r = rand_u64();
        }
        numbers[i] = r;
        fuse_debugf(NULL, "  number[%lu]=%lu\n",i,numbers[i]);
    }

    // Create a map
    fuse_debugf(NULL, "Creating a map of %lu items\n",sz);
    fuse_map_t *map = fuse_map_new(fuse, sz);
    assert(map);

    // Set the numbers
    for(uint64_t i = 0; i < sz; i++) {
        fuse_debugf(NULL, "  Setting number[%lu]=%lu\n",i,numbers[i]);
        assert(fuse_map_set(map, (void *)i+1, (void *)numbers[i]));
    }

    // Assert count
    assert(fuse_map_stats(map, NULL) == sz);

    // Get the numbers
    for(uint64_t i = 0; i < sz; i++) {
        fuse_debugf(NULL, "  Getting number[%lu]\n",i);
        assert(fuse_map_get(map, (void *)i+1) == (void *)numbers[i]);
    }

    // Randomly delete numbers until the map is empty
    while(fuse_map_stats(map, NULL) > 0) {
        uint64_t i = rand_u64() % sz;
        fuse_debugf(NULL, "  Deleting number[%lu]\n",i);
        fuse_debugf(NULL, "    Count is %lu\n",fuse_map_stats(map, NULL));
        assert(fuse_map_set(map, (void *)i+1, 0));
    }

    // Return
    return fuse_destroy(fuse);
}


int main()
{
    assert(TEST_001() == 0);
    assert(TEST_002(10) == 0);
    assert(TEST_002(100) == 0);
    assert(TEST_002(1000) == 0);
    assert(TEST_003(1000) == 0);

    // Return success
    return 0;
}
