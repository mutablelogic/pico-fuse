#include <limits.h>
#include <time.h>
#include <fuse/fuse.h>

// Private includes
#include "random.h"

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

#ifdef TARGET_POSIX
#include <stdint.h>
#include "random.h"

// Seed
static struct fuse_random_state seed = {0};

/** @brief Initalize a random number generator
 * 
 *  @param self A pointer to the random number generator to initialize
 *  @param seed The seed to use for the random number generator, or zero to use the current time
 */
void fuse_random_init(struct fuse_random_state *self, uint64_t seed)
{
    assert(self);

    if (self->seed == 0)
    {
        time_t t = time(NULL);
        self->seed = (uint64_t)t;
    } else {
        self->seed = seed;
    }
}

/** @brief Generate a uint64 random number between 0 and UINT64_MAX using a generator seed
 * 
 *  @param self A pointer to the random number generator to use
 *  @return A random number between 0 and UINT64_MAX
 */
uint64_t fuse_random_u64(struct fuse_random_state *self)
{
    assert(self);

    self->seed = self->seed + 0x9E3779B97F4A7C15;
    uint64_t value = self->seed;
    value = (value ^ (value >> 30)) * 0xBF58476D1CE4E5B9;
    value = (value ^ (value >> 27)) * 0x94D049BB133111EB;
    value = value ^ (value >> 31);
    return value;
}

inline uint32_t rand_u32()
{
    if(seed.seed == 0)
    {
        fuse_random_init(&seed, 0);
    }
    return fuse_random_u64(&seed);
}


inline uint64_t rand_u64()
{
    if(seed.seed == 0)
    {
        fuse_random_init(&seed, 0);
    }
    return fuse_random_u64(&seed);
}

#endif
