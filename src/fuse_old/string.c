#include <string.h>
#include <fuse/fuse.h>

// Private includes
#include "string.h"

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

fuse_str_t *fuse_str_new_ex(fuse_t *fuse, const char *ptr, const char *file, int line)
{
    assert(fuse);

    // Calculate size of string
    size_t size = 0;
    if (ptr != NULL)
    {
        size = strlen(ptr);
    }

    // Create the header
    fuse_str_t *str = fuse_alloc_ex(fuse, size, file, line);
    if (size > 0)
    {
        str->ptr = ptr;
    }
    else
    {
        str->ptr = NULL;
    }
    str->size = size;
    str->constant = true;

    // Return the string
    return str;
}

void fuse_str_destroy(fuse_t *fuse, fuse_str_t *str)
{
    assert(fuse);
    assert(str);

    // If the string isn't constant, free the memory
    if (str->constant == false)
    {
        fuse_free(fuse, str->ptr);
    }

    // Free the string
    fuse_free(fuse,str);
}
