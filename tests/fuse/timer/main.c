#include <fuse/fuse.h>

void TEST_001_callback(fuse_t *self, fuse_event_t *evt, void *user_data)
{
    assert(self);
    assert(evt);

    // Print the event
    fuse_printf(self,"Event: %v\n", evt);
}

int TEST_001(fuse_t *self)
{
    fuse_printf(self,"TEST_001\n");

    // Register a callback for the timer event on Core 0
    assert(fuse_register_callback(self,FUSE_EVENT_TIMER,0,TEST_001_callback));

    // Schedule timer to run every second
    fuse_timer_t *timer = fuse_timer_schedule(self, 1000, true, (void* )100);
    assert(timer);

    // Start the run loop
    uint8_t i = 0;
    while (i < 5)
    {
        // Pop event from the event queue - core 0
        fuse_event_t *event = fuse_next_event(self, 0);
        if (event == NULL) {
            // TODO: Drain the pool occasionally
            sleep_ms(10);
            continue;
        }

        // Execute the event for core 0
        fuse_exec_event(self, 0, event);
        i++;
    }

    // Cancel the timer
    fuse_timer_cancel(self, timer);

    // Return success
    return 0;
}

int run(fuse_t *fuse)
{
    assert(fuse);
    assert(TEST_001(fuse) == 0);
    return 0;
}
