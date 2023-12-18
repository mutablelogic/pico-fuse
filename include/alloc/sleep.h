/** @file sleep.h
 *  @brief Function prototypes for pausing execution
 */
#ifndef FUSE_SLEEP_H
#define FUSE_SLEEP_H

#include <stdint.h>

/** @brief Pause for a certain number of milliseconds
 *
 *  @param ms The number of milliseconds to pause
 */
void sleep_ms(uint32_t ms);

#endif
