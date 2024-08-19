#include <fuse/fuse.h>

///////////////////////////////////////////////////////////////////////////////
// GLOBALS

fuse_context_t *fuse_register(fuse_t *fuse, fuse_device_t *device, void *user_data)
{
    assert(device);
    assert(device->init);

    fuse_context_t *context = device->init(fuse, user_data);
    if (context)
    {
        fuse_debugf("fuse_register: %s context=%p\n", device->name, context);
    }
    else
    {
        fuse_debugf("fuse_register: %s failed\n", device->name);
    }

    // Return success
    return context;
}
