#include <fuse/fuse.h>

int run(fuse_t *fuse);

void main()
{
    fuse_t *fuse = fuse_new();
    assert(fuse);

    fuse_run(fuse, run);
}
