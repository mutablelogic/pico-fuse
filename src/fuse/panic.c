#include <stdlib.h>

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
