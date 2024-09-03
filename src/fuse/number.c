#include <fuse/fuse.h>
#include "alloc.h"
#include "number.h"
#include "fuse.h"
#include "printf.h"

///////////////////////////////////////////////////////////////////////////////
// DECLARATIONS

static bool fuse_init_number(fuse_t *self, fuse_value_t *value, const void *user_data);
static size_t fuse_qstr_bool(fuse_t *self, char *buf, size_t sz, size_t i, fuse_value_t *v);
static size_t fuse_qstr_number(fuse_t *self, char *buf, size_t sz, size_t i, fuse_value_t *v);

///////////////////////////////////////////////////////////////////////////////
// LIFECYCLE

/** @brief Register type for integer values
 */
void fuse_register_value_int(fuse_t *self)
{
    assert(self);

    fuse_value_desc_t fuse_s8_type = {
        .size = sizeof(int8_t),
        .name = "S8",
        .init = fuse_init_number,
        .cstr = fuse_qstr_number,
        .qstr = fuse_qstr_number,
    };
    fuse_register_value_type(self, FUSE_MAGIC_S8, fuse_s8_type);

    fuse_value_desc_t fuse_s16_type = {
        .size = sizeof(int16_t),
        .name = "S16",
        .init = fuse_init_number,
        .cstr = fuse_qstr_number,
        .qstr = fuse_qstr_number,
    };
    fuse_register_value_type(self, FUSE_MAGIC_S16, fuse_s16_type);

    fuse_value_desc_t fuse_s32_type = {
        .size = sizeof(int32_t),
        .name = "S32",
        .init = fuse_init_number,
        .cstr = fuse_qstr_number,
        .qstr = fuse_qstr_number,
    };
    fuse_register_value_type(self, FUSE_MAGIC_S32, fuse_s32_type);

    fuse_value_desc_t fuse_s64_type = {
        .size = sizeof(int64_t),
        .name = "S64",
        .init = fuse_init_number,
        .cstr = fuse_qstr_number,
        .qstr = fuse_qstr_number,
    };
    fuse_register_value_type(self, FUSE_MAGIC_S64, fuse_s64_type);
}

/** @brief Register type for unsigned integer values
 */
void fuse_register_value_uint(fuse_t *self)
{
    assert(self);

    fuse_value_desc_t fuse_u8_type = {
        .size = sizeof(uint8_t),
        .name = "U8",
        .init = fuse_init_number,
        .cstr = fuse_qstr_number,
        .qstr = fuse_qstr_number,
    };
    fuse_register_value_type(self, FUSE_MAGIC_U8, fuse_u8_type);

    fuse_value_desc_t fuse_u16_type = {
        .size = sizeof(uint16_t),
        .name = "U16",
        .init = fuse_init_number,
        .cstr = fuse_qstr_number,
        .qstr = fuse_qstr_number,
    };
    fuse_register_value_type(self, FUSE_MAGIC_U16, fuse_u16_type);

    fuse_value_desc_t fuse_u32_type = {
        .size = sizeof(uint32_t),
        .name = "U32",
        .init = fuse_init_number,
        .cstr = fuse_qstr_number,
        .qstr = fuse_qstr_number,
    };
    fuse_register_value_type(self, FUSE_MAGIC_U32, fuse_u32_type);

    fuse_value_desc_t fuse_u64_type = {
        .size = sizeof(uint64_t),
        .name = "U64",
        .init = fuse_init_number,
        .cstr = fuse_qstr_number,
        .qstr = fuse_qstr_number,
    };
    fuse_register_value_type(self, FUSE_MAGIC_U64, fuse_u64_type);
}

/** @brief Register type for bool values
 */
void fuse_register_value_bool(fuse_t *self)
{
    assert(self);

    fuse_value_desc_t fuse_bool_type = {
        .size = sizeof(bool),
        .name = "BOOL",
        .init = fuse_init_number,
        .cstr = fuse_qstr_bool,
        .qstr = fuse_qstr_bool,
    };
    fuse_register_value_type(self, FUSE_MAGIC_BOOL, fuse_bool_type);
}

/** @brief Register type for float values
 */
void fuse_register_value_float(fuse_t *self)
{
    assert(self);

    fuse_value_desc_t fuse_f32_type = {
        .size = sizeof(float),
        .name = "F32",
        .init = fuse_init_number,
        .cstr = fuse_qstr_number,
        .qstr = fuse_qstr_number,
    };
    fuse_register_value_type(self, FUSE_MAGIC_F32, fuse_f32_type);

    fuse_value_desc_t fuse_f64_type = {
        .size = sizeof(double),
        .name = "F64",
        .init = fuse_init_number,
        .cstr = fuse_qstr_number,
        .qstr = fuse_qstr_number,
    };
    fuse_register_value_type(self, FUSE_MAGIC_F64, fuse_f64_type);
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE METHODS

/* @brief Initialise a number value
 */
static bool fuse_init_number(fuse_t *self, fuse_value_t *value, const void *user_data)
{
    assert(self);
    assert(value);

    uint16_t magic = fuse_allocator_magic(self->allocator, value);
    switch (magic)
    {
    case FUSE_MAGIC_U8:
        *(uint8_t *)value = (uint8_t)(uintptr_t)user_data;
        break;
    case FUSE_MAGIC_U16:
        *(uint16_t *)value = (uint16_t)(uintptr_t)user_data;
        break;
    case FUSE_MAGIC_U32:
        *(uint32_t *)value = (uint32_t)(uintptr_t)user_data;
        break;
    case FUSE_MAGIC_U64:
        *(uint64_t *)value = (uint64_t)(uintptr_t)user_data;
        break;
    case FUSE_MAGIC_S8:
        *(int8_t *)value = (int8_t)(intptr_t)user_data;
        break;
    case FUSE_MAGIC_S16:
        *(int16_t *)value = (int16_t)(intptr_t)user_data;
        break;
    case FUSE_MAGIC_S32:
        *(int32_t *)value = (int32_t)(intptr_t)user_data;
        break;
    case FUSE_MAGIC_S64:
        *(int64_t *)value = (int64_t)(intptr_t)user_data;
        break;
    case FUSE_MAGIC_BOOL:
        *(bool *)value = (bool)user_data;
        break;
    case FUSE_MAGIC_F32:
    case FUSE_MAGIC_F64:
    {
        size_t size = self->desc[magic].size;
        if (user_data != NULL)
        {
            memcpy(value, user_data, size);
        }
        else
        {
            memset(value, 0, size);
        }
        break;
    }
    default:
        return false;
    }

    // Return success
    return true;
}

/* @brief Output a bool as a cstr
 */
static size_t fuse_qstr_bool(fuse_t *self, char *buf, size_t sz, size_t i, fuse_value_t *v)
{
    assert(self);
    assert(v);
    assert(fuse_allocator_magic(self->allocator, v) == FUSE_MAGIC_BOOL);

    bool b = *(bool *)v;
    if (b)
    {
        return cstrtostr_internal(buf, sz, i, FUSE_PRINTF_TRUE);
    }
    else
    {
        return cstrtostr_internal(buf, sz, i, FUSE_PRINTF_FALSE);
    }
}

/* @brief Output an integer as a cstr
 */
static size_t fuse_qstr_number(fuse_t *self, char *buf, size_t sz, size_t i, fuse_value_t *v)
{
    assert(self);
    assert(v);

    switch (fuse_allocator_magic(self->allocator, v))
    {
    case FUSE_MAGIC_U8:
        return utostr_internal(buf, sz, i, *(uint8_t *)v, 0);
    case FUSE_MAGIC_U16:
        return utostr_internal(buf, sz, i, *(uint16_t *)v, 0);
    case FUSE_MAGIC_U32:
        return utostr_internal(buf, sz, i, *(uint32_t *)v, 0);
    case FUSE_MAGIC_U64:
        return utostr_internal(buf, sz, i, *(uint64_t *)v, 0);
    case FUSE_MAGIC_S8:
        return itostr_internal(buf, sz, i, *(int8_t *)v, 0);
    case FUSE_MAGIC_S16:
        return itostr_internal(buf, sz, i, *(int16_t *)v, 0);
    case FUSE_MAGIC_S32:
        return itostr_internal(buf, sz, i, *(int32_t *)v, 0);
    case FUSE_MAGIC_S64:
        return itostr_internal(buf, sz, i, *(int64_t *)v, 0);
    case FUSE_MAGIC_F32:
        return ftostr_internal(buf, sz, i, *(float *)v, 0);
    case FUSE_MAGIC_F64:
        return ftostr_internal(buf, sz, i, *(double *)v, 0);
    default:
        assert(false);
    }
    return 0;
}
