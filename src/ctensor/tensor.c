#include <ctensor/ctensor.h>
#include <fuse/fuse.h>
#include "tensor.h"

tensor_t *ctensor_new(fuse_t *fuse, uint16_t magic, uint32_t *shape, void *elems, const char *file, int line)
{
    assert(fuse);
    assert(magic);

    // Determine the number of elements in the tensor, and the number of dimensions
    size_t size = 0;
    uint8_t ndims = -1;
    if (shape == NULL || shape[0] == 0)
    {
        size = 1;
        ndims = 0;
    }
    else
    {
        while (shape[++ndims] != 0)
        {
            if (size == 0)
            {

                size = shape[ndims];
            }
            else
            {
                size *= shape[ndims];
            }
        }
    }

    // Check for limits
    assert(size <= UINT32_MAX);
    assert(ndims <= TENSOR_MAX_DIMS);

    // If no size, then return NULL
    if (size == 0)
    {
        fuse_debugf("ctensor_new failed, zero sized tensor\n");
        return NULL;
    }

    // Allocate memory for tensor
    tensor_t *t = fuse_alloc_ex(fuse, sizeof(tensor_t) + fuse_sizeof(magic) * size, FUSE_MAGIC_TENSOR, file, line);
    if (t == NULL)
    {
        fuse_debugf("ctensor_new failed, out of memory allocating %ld bytes\n", sizeof(tensor_t) + fuse_sizeof(magic) * size);
        return NULL;
    }

    // TODO

    // Return success
    return t;
}
