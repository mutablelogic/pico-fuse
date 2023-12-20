/** @file random.h
 *  @brief Function prototypes for random numbers. 
 * 
 *  The first call to the generator creates a seed for subsequent calls.
 *  On the Pico, the documentation states that "methods may be safely called from either core
 *  or from an IRQ, but be careful in the latter case as the calls may block for a number of 
 *  microseconds waiting on more entropy"
 */
#ifndef FUSE_RANDOM_H
#define FUSE_RANDOM_H
#include <stdint.h>

/** @brief Return a random unsigned 32 bit number
 *
 * @return An unsigned 32 bit number
 */
uint32_t rand_u32();


/** @brief Return a random unsigned 64 bit number
 *
 * @return An unsigned 64 bit number
 */
uint64_t rand_u64();

#endif /* FUSE_RANDOM_H */
