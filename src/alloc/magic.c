
#include <alloc/fuse.h>


inline const char* fuse_magic_cstr(uint16_t magic) {
    switch(magic) {
        case (uint16_t)(FUSE_MAGIC_NULL):
            return "NUL";
        case (uint16_t)(FUSE_MAGIC_APP):
            return "APP";
        case (uint16_t)(FUSE_MAGIC_BLOCK):
            return "MEM";
        default:
            return "???";
    }
}
