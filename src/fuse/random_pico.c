
///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

#ifdef TARGET_PICO
#include <stdint.h>
#include <pico.h>

inline uint32_t rand_u32() {
    return get_rand_u32();
}

inline uint64_t rand_u64() {
    return get_rand_u64();
}

#endif
