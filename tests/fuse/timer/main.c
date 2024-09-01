#include <fuse/fuse.h>
#include <stdio.h>

int TEST_001(fuse_t *self)
{
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
            sleep_ms(10);
            continue;
        }

        // Print the event
        fuse_printf(self,"Core 0: Event: %v\n", event);
        i++;
    }

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
