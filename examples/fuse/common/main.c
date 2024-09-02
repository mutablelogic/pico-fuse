#include <fuse/fuse.h>
#include <pico/stdlib.h>
#include <stdio.h>

int run(fuse_t *fuse);

int main()
{
    fuse_t *fuse = fuse_new();
    assert(fuse);
    fuse_run(fuse, run);

    // Destroy the applicatiom, return exit code
    return fuse_destroy(fuse);
}
