#include <stdbool.h>
#include <stdio.h>
#include <fuse/fuse.h>

void abort(void);

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

inline void fuse_panic(const char *expr, const char *file, int line)
{
    if (file != NULL && line > 0)
    {
        printf("PANIC: %s (%s:%d)\n", expr, file, line);
    }
    else
    {
        printf("PANIC: %s\n", expr);
    }
#ifdef TARGET_PICO
    while (true)
    {
        sleep_ms(1000);
    }
#else
    abort();
#endif
}
