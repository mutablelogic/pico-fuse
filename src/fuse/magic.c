#include <fuse/fuse.h>

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

inline size_t fuse_sizeof(uint16_t magic) {
    switch(magic) {
        case (uint16_t)(FUSE_MAGIC_U8):
            return sizeof(uint8_t);
        case (uint16_t)(FUSE_MAGIC_U16):
            return sizeof(uint16_t);
        case (uint16_t)(FUSE_MAGIC_U32):
            return sizeof(uint32_t);
        case (uint16_t)(FUSE_MAGIC_U64):
            return sizeof(uint64_t);
        case (uint16_t)(FUSE_MAGIC_S8):
            return sizeof(int8_t);
        case (uint16_t)(FUSE_MAGIC_S16):
            return sizeof(int16_t);
        case (uint16_t)(FUSE_MAGIC_S32):
            return sizeof(int32_t);
        case (uint16_t)(FUSE_MAGIC_S64):
            return sizeof(int64_t);
        case (uint16_t)(FUSE_MAGIC_F32):
            return sizeof(float);
        case (uint16_t)(FUSE_MAGIC_F64):
            return sizeof(double);
        case (uint16_t)(FUSE_MAGIC_BOOL):
            return sizeof(bool);
        default:
            return 0;
    }
}

inline const char* fuse_magic_cstr(uint16_t magic) {
    switch(magic) {
        case (uint16_t)FUSE_MAGIC_NULL:
            return "NUL";
        case (uint16_t)FUSE_MAGIC_APP:
            return "APP";
        case (uint16_t)(FUSE_MAGIC_BLOCK):
            return "MEM";
        case (uint16_t)(FUSE_MAGIC_U8):
            return "U8";
        case (uint16_t)(FUSE_MAGIC_U16):
            return "U16";
        case (uint16_t)(FUSE_MAGIC_U32):
            return "U32";
        case (uint16_t)(FUSE_MAGIC_U64):
            return "U64";
        case (uint16_t)(FUSE_MAGIC_S8):
            return "S8";
        case (uint16_t)(FUSE_MAGIC_S16):
            return "S16";
        case (uint16_t)(FUSE_MAGIC_S32):
            return "S32";
        case (uint16_t)(FUSE_MAGIC_S64):
            return "S64";
        case (uint16_t)(FUSE_MAGIC_F32):
            return "F32";
        case (uint16_t)(FUSE_MAGIC_F64):
            return "F64";
        case (uint16_t)(FUSE_MAGIC_BOOL):
            return "BOOL";
        case (uint16_t)(FUSE_MAGIC_STR):
            return "STR";
        case (uint16_t)(FUSE_MAGIC_LIST):
            return "LIST";
        case (uint16_t)(FUSE_MAGIC_MAP):
            return "MAP";
        case (uint16_t)(FUSE_MAGIC_WIFI):
            return "WFI";
        default:
            return "???";
    }
}
