
#include <fuse/fuse.h>
#include <stdio.h>

int TEST_001()
{
    printf("Returning 100 random numbers - uint32\n");

    for(int i = 0; i < 100; i++) {
        printf("Random number %d: %u\n", i, rand_u32());
    }

    // Return
    return 0;
}

int TEST_002()
{
    printf("Returning 100 random numbers - uint64\n");

    for(int i = 0; i < 100; i++) {
        printf("Random number %d: %lu\n", i, rand_u64());
    }

    // Return
    return 0;
}

int main()
{
    assert(TEST_001() == 0);
    assert(TEST_002() == 0);

    // Return success
    return 0;
}
