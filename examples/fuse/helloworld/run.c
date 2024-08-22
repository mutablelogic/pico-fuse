#include <fuse/fuse.h>

int run(fuse_t* fuse) {
    fuse_debugf("Hello, World!\n");
    return 0;
}
