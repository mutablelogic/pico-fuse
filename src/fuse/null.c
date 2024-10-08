#include <fuse/fuse.h>
#include "alloc.h"
#include "fuse.h"
#include "null.h"
#include "printf.h"

///////////////////////////////////////////////////////////////////////////////
// DECLARATIONS

static bool fuse_init_null(fuse_t *self, fuse_value_t *value, const void *user_data);
static size_t fuse_str_null(fuse_t *self, char *buf, size_t sz, size_t i, fuse_value_t *v, bool json);

///////////////////////////////////////////////////////////////////////////////
// LIFECYCLE

/** @brief Register type for a null value
 */
void fuse_register_value_null(fuse_t *self)
{
    assert(self);

    // Register type
    fuse_value_desc_t fuse_null_type = {
        .size = 0,
        .name = "NULL",
        .str = fuse_str_null,
    };
    fuse_register_value_type(self, FUSE_MAGIC_NULL, fuse_null_type);
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE METHODS

/* @brief Output a null as a string
 */
static inline size_t fuse_str_null(fuse_t *self, char *buf, size_t sz, size_t i, fuse_value_t *v, bool json)
{
    if (json) {
        return cstrtostr_internal(buf, sz, i, FUSE_PRINTF_NULL_JSON);
    } else {
        return cstrtostr_internal(buf, sz, i, NULL);
    }
}
