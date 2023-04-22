#include <fuse/fuse.h>
#include <pimoroni/inkypack.h>

int main()
{
    // Create application
    fuse_t *app = fuse_init(64 * 1024, FUSE_FLAG_DEBUG);

    // Register drivers
    fuse_register_driver(app, &pimoroni_inkypack, NULL);

    // Runloop until exit
    fuse_main(app);

    // Exit
    return fuse_exit(app);
}
