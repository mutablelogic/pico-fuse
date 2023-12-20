
#include <fuse/fuse.h>
#include <signal.h>
#include <stdlib.h>

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
    signal(SIGABRT, abort_handler);
    assert(false);
    return -1;
}

