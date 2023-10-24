
#include <fuse/fuse.h>

int TEST_001()
{
    fuse_debugf(NULL, "Creating a fuse application\n");
    fuse_t* fuse = fuse_new(FUSE_FLAG_DEBUG);
    assert(fuse);

    // Create a map and then destroy it
    fuse_debugf(NULL, "Creating a map of 10 items\n");
    fuse_map_t* map = fuse_map_new(fuse, 10);
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

int main()
{
    assert(TEST_001() == 0);

    // Return success
    return 0;
}
