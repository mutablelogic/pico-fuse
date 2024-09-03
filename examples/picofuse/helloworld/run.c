#include <picofuse/picofuse.h>

int run(fuse_t* fuse) {
    fuse_debugf(fuse, "Hello, World!\n");
    return 0;
}
