
#include <fuse/fuse.h>

int TEST_001()
{
    fuse_debugf(NULL, "Creating a fuse application\n");
    fuse_t *fuse = fuse_new(FUSE_FLAG_DEBUG);
    assert(fuse);

    // Create a list 
    fuse_debugf(NULL, "Creating an empty list\n");
    fuse_list_t *list = fuse_list_new(fuse);
    assert(list);
    assert(fuse_list_count(list) == 0);

    // Destroy the list
    fuse_list_destroy(fuse, list);

    // Return
    return fuse_destroy(fuse);
}

int TEST_002()
{
    fuse_debugf(NULL, "Creating a fuse application\n");
    fuse_t *fuse = fuse_new(FUSE_FLAG_DEBUG);
    assert(fuse);

    // Create a list 
    fuse_debugf(NULL, "Creating a list\n");
    fuse_list_t *list = fuse_list_new(fuse);
    assert(list);
    assert(fuse_list_count(list) == 0);

    // Push 10 items into the list
    fuse_debugf(NULL, "Pushing 10 items into the list\n");
    for (int i = 0; i < 10; i++)
    {
        fuse_list_push(list, (void *)(i + 1));
        assert(fuse_list_count(list) == i + 1);
    }

    // Destroy the list
    fuse_list_destroy(fuse, list);

    // Return
    return fuse_destroy(fuse);
}

int main()
{
    assert(TEST_001() == 0);
    assert(TEST_002() == 0);

    // Return success
    return 0;
}
