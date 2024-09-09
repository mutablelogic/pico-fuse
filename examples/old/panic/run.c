#include <picofuse/picofuse.h>

int run(fuse_t *fuse)
{
    fuse_panic("This is a really bad thing that happened!", __FILE__, __LINE__);
    return 0;
}
