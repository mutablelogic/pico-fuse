#include <fuse/fuse.h>
#include <stdio.h>

int TEST_001(fuse_t *self)
{
    // Schedule timer to run every second
    fuse_timer_t *timer = fuse_timer_schedule(self, 1000, true, 45);
    assert(timer);

    // Sleep for 5 seconds
    sleep_ms(5000);

    // Cancel the timer
    fuse_timer_cancel(self, timer);

    // Return success
    return 0;
}

int main()
{
    fuse_t *self = fuse_new();
    assert(self);
    assert(TEST_001(self) == 0);
    assert(fuse_destroy(self) == 0);
}
