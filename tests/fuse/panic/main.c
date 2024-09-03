
#include <fuse/fuse.h>
#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

/*
 * handle abort signal
 */
void abort_handler(int sig) {
    printf("Abort signal received\n");
    exit(sig);
}

/*
 * Immediately causes a panic
 */
int main()
{
    printf("starting test\n");
    signal(SIGABRT, abort_handler);
    printf("asserting false\n");
    assert(false);
    return -1;
}
