#include <fuse/fuse.h>
#include <stdio.h>

#include "decimal.c"
#include "binary.c"
#include "hex.c"

int main()
{
    fuse_t *self = fuse_new();
    assert(self);

    // decimal
    assert(TEST_DEC_001(self) == 0);
    assert(TEST_DEC_002(self) == 0);
    assert(TEST_DEC_003(self) == 0);
    assert(TEST_DEC_004(self) == 0);

    // binary
    assert(TEST_BIN_001(self) == 0);
    assert(TEST_BIN_002(self) == 0);
    assert(TEST_BIN_003(self) == 0);

    // hex
    assert(TEST_HEX_001(self) == 0);
    assert(TEST_HEX_002(self) == 0);
    assert(TEST_HEX_003(self) == 0);

    assert(fuse_destroy(self) == 0);
}
