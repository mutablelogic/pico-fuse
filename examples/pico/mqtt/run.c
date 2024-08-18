#include <picofuse/picofuse.h>

int run(fuse_t *fuse)
{
    fuse_wifi_new(FUSE_WIFI_STATION, "de");
    fuse_mqtt_new("192.168.86.12");

    int i = 0;
    while (true)
    {
        fuse_debugf("mqtt i=%d\n", i++);
        sleep_ms(2000);
    }

    fuse_mqtt_destroy();
    fuse_wifi_destroy();
    return 0;
}
