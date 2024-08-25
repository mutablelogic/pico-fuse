
#ifndef TARGET_PICO

#define _BSD_SOURCE
#include <stdint.h>
#include <unistd.h>
#include <errno.h>

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

/*
 * Pause for a certain number of milliseconds
 *
 * @param ms The number of milliseconds to pause
 */
inline void sleep_ms(uint32_t ms)
{
    if (ms == 0)
    {
        return;
    }

    // usleep takes microseconds as input
    usleep(ms * 1000);
}

#endif
