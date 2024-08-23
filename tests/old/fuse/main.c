
#include <fuse/fuse.h>

int TEST_null_001();
int TEST_list_001();
int TEST_list_002();
int TEST_list_003();
int TEST_list_004();
int TEST_list_005();
int TEST_list_006();
int TEST_scalar_001();
int TEST_scalar_002();
int TEST_scalar_003();
int TEST_scalar_004();
int TEST_scalar_005();
int TEST_scalar_006();
int TEST_scalar_007();
int TEST_scalar_008();
int TEST_scalar_009();
int TEST_scalar_010();
int TEST_scalar_011();

int main()
{
    assert(TEST_null_001() == 0);
    assert(TEST_list_001() == 0);
    assert(TEST_list_002() == 0);
    assert(TEST_list_003() == 0);
    assert(TEST_list_004() == 0);
    assert(TEST_list_005() == 0);
    assert(TEST_list_006() == 0);
    assert(TEST_scalar_001() == 0);
    assert(TEST_scalar_002() == 0);
    assert(TEST_scalar_003() == 0);
    assert(TEST_scalar_004() == 0);
    assert(TEST_scalar_005() == 0);
    assert(TEST_scalar_006() == 0);
    assert(TEST_scalar_007() == 0);
    assert(TEST_scalar_008() == 0);
    assert(TEST_scalar_009() == 0);
    assert(TEST_scalar_010() == 0);
    assert(TEST_scalar_011() == 0);

    // Return success
    return 0;
}
