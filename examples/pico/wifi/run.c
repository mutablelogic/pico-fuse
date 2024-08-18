#include <picofuse/picofuse.h>

int run(fuse_t *fuse)
{
    fuse_wifi_new(FUSE_WIFI_STATION, "de");

    int i = 0;
    while (true)
    {
        fuse_debugf("Wifi i=%d\n", i++);
        fuse_wifi_scan();
        sleep_ms(1000);
    }

    fuse_wifi_destroy();
    return 0;
}
