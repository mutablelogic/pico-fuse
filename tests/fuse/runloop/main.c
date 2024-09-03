#include <fuse/fuse.h>

void TEST_001_callback(fuse_t *self, fuse_event_t *evt, void *user_data)
{
    assert(self);
    assert(evt);

    static int i = 0;

    // Print the event
    fuse_printf(self,"Event: %v\n", evt);
    fuse_printf(self," Iter: %d\n", i++);
    if (i == 5)
    {
        fuse_exit(self, 0);
    }
}

int TEST_001(fuse_t *self)
{
    fuse_printf(self,"TEST_001\n");

    // Register a callback for the timer event on Core 0
    assert(fuse_register_callback(self,FUSE_EVENT_TIMER,0,TEST_001_callback));

    // Schedule timer to run every second
    fuse_timer_t *timer = fuse_timer_schedule(self, 1000, true, (void* )100);
    assert(timer);
    return 0;
}

int run(fuse_t *fuse)
{
    assert(fuse);
    assert(TEST_001(fuse) == 0);
    return 0;
}
