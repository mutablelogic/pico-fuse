#include <string.h>
#include <fuse/fuse.h>

// Private includes
#include "value.h"

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

inline fuse_value_t *fuse_new_null(fuse_t *fuse)
{
    assert(fuse);
    return fuse_new_ptr(fuse, NULL);
}

fuse_value_t *fuse_new_ptr(fuse_t *fuse, void *ptr) {
    assert(fuse);

    // Allocate memory for the value
    fuse_value_t *value = fuse_alloc(fuse, sizeof(fuse_value_t));
    if (value == NULL)
    {
        return NULL;
    }

    // Zero all data structures
    memset(value, 0, sizeof(fuse_value_t));

    // Set the pointer
    value->ptr = ptr;

    // Set the reference count to zero - which means
    // the ptr does not have an allocation
    value->ref = 0;

    // Return the value
    return value;
}

inline bool fuse_equal_null(fuse_value_t *value)
{
    assert(value);
    return value->ptr == NULL;
}

inline void* fuse_ptr(fuse_value_t *value)
{
    assert(value);
    return value->ptr;
}

const char *fuse_jsonstring(fuse_value_t *value)
{
    assert(value);
    // Simple case where value is NULL
    if(value->ptr == NULL)
    {
        return "null";
    }

    // Other use-cases here
    assert(false);
    return NULL;
}
