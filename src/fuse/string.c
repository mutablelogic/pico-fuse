#include <fuse/fuse.h>
#include "alloc.h"
#include "fuse.h"
#include "string.h"
#include "printf.h"

///////////////////////////////////////////////////////////////////////////////
// DECLARATIONS

static bool fuse_init_cstr(fuse_t *self, fuse_value_t *value, const void *user_data);
static size_t fuse_cstr_cstr(fuse_t *self, char *buf, size_t sz, size_t i, fuse_value_t *v);
static size_t fuse_qstr_cstr(fuse_t *self, char *buf, size_t sz, size_t i, fuse_value_t *v);

///////////////////////////////////////////////////////////////////////////////
// LIFECYCLE

/** @brief Register type for string values
 */
void fuse_register_value_string(fuse_t *self)
{
    assert(self);

    // Register type
    fuse_value_desc_t fuse_cstr_type = {
        .size = sizeof(const char *),
        .name = "CSTR",
        .init = fuse_init_cstr,
        .cstr = fuse_cstr_cstr,
        .qstr = fuse_qstr_cstr,
    };
    fuse_register_value_type(self, FUSE_MAGIC_CSTR, fuse_cstr_type);
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE METHODS

/* @brief Initialise by setting the string pointer
 */
static inline bool fuse_init_cstr(fuse_t *self, fuse_value_t *value, const void *user_data)
{
    assert(self);
    assert(value);

    // Set the string pointer
    *(const char **)value = (const char *)user_data;

    // Return success
    return true;
}

/* @brief Output a pointer to a null-terminated string as a cstr
 */
static inline size_t fuse_cstr_cstr(fuse_t *self, char *buf, size_t sz, size_t i, fuse_value_t *v)
{
    assert(self);
    assert(v);
    assert(fuse_allocator_magic(self->allocator, v) == FUSE_MAGIC_CSTR);

    const char *vp = *(const char **)v;
    return cstrtostr_internal(buf, sz, i, vp);
}

/* @brief Output a pointer to a null-terminated string as a quoted cstr
 */
static inline size_t fuse_qstr_cstr(fuse_t *self, char *buf, size_t sz, size_t i, fuse_value_t *v)
{
    assert(self);
    assert(v);
    assert(fuse_allocator_magic(self->allocator, v) == FUSE_MAGIC_CSTR);

    const char *vp = *(const char **)v;
    return qstrtostr_internal(buf, sz, i, vp);
}
