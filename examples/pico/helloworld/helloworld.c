#include <picofuse/picofuse.h>

int run(fuse_t* fuse) {
    fuse_debugf("Hello, World!\n");
    return 0;
}

void main() {
    fuse_main(run);
}
