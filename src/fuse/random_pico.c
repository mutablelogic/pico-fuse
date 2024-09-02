#if defined(TARGET_PICO)

#include <stdint.h>
#include <pico/rand.h>

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

uint32_t rand_u32() {
    return get_rand_32();
}

uint64_t rand_u64() {
    return get_rand_64();
}

#endif
