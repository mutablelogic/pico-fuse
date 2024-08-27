
// Public headers
#include <fuse/fuse.h>

/* @brief Retain a value
 */
fuse_value_t* fuse_value_retain(fuse_t* self, fuse_value_t* value) {
    assert(self);
    assert(value);

    // TODO

    return value;
}

/* @brief Release a value
 */
void fuse_value_release(fuse_t* self, fuse_value_t* value) {
    assert(self);
    assert(value);

    // TODO
}
