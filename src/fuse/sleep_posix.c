
#ifndef TARGET_PICO

#include <stdint.h>
#include <time.h>
#include <errno.h>

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

/*
 * Pause for a certain number of milliseconds
 *
 * @param ms The number of milliseconds to pause
 */
void sleep_ms(uint32_t ms)
{
    struct timespec ts;
    int result = 0;

    // Convert milliseconds to seconds and nanoseconds
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000;

    do {
        result = nanosleep(&ts, &ts);
    } while (result && errno == EINTR);  // Repeat if interrupted by a signal
}

#endif
