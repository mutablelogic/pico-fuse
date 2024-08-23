
#include <fuse/fuse.h>
#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>

/*
 * handle abort signal
 */
void abort_handler(int sig) {
    fuse_debugf("Abort signal received\n");
    exit(sig);
}

/*
 * Immediately causes a panic
 */
int main()
{
    fuse_debugf("starting test\n");
    signal(SIGABRT, abort_handler);
    fuse_debugf("asserting false\n");
    assert(false);
    return -1;
}
