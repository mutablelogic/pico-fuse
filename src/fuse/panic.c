#include <stdlib.h>
#include <stdio.h>

// Includes
#include <fuse/fuse.h>

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

#ifdef TARGET_POSIX
/*
 * Debug to stdout if the debug flag is set
 */
void panic(const char* unused)
{
    abort();
}

#endif

inline void fuse_panic(const char *expr, const char *file, int line)
{
    if(file != NULL && line > 0) {
        printf("PANIC: %s (%s:%d)\n", expr, file, line);
    } else {
        printf("PANIC: %s\n", expr);
    }
    while (true) {
        sleep_ms(1000);
    }
}
