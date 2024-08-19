#include <picofuse/picofuse.h>

int run(fuse_t *fuse)
{
    // Register the wifi module for DE country code and station mode
    fuse_wifi_userdata_t params = {
        .country_code = "DE",
        .mode = FUSE_WIFI_STATION | FUSE_WIFI_SCAN};
    fuse_context_t *context = fuse_register(fuse, &wifi, &params);

    fuse_debugf("context=%p\n", context);

    return 0;
}
