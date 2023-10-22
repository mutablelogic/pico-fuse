
#include <fuse/fuse.h>
#include <picofuse/picofuse.h>

int run(fuse_t *fuse)
{
    fuse_debugf(fuse, "In run()\n");
    bool value = true;
    for(int i = 0; i < 10; i++) {
        picofuse_led_set(!value);
        value = !value;
        sleep_ms(500);
    }
    return 0;
}

int main()
{
    return picofuse_main(FUSE_FLAG_DEBUG, run);
}
