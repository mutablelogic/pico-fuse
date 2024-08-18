#include <picofuse/picofuse.h>

int run(fuse_t* fuse);

void main() {
    fuse_main(run);
}
