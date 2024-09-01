#include <fuse/fuse.h>
#include <stdio.h>

int TEST_001(fuse_t *self)
{
    fuse_debugf("TEST_001 event\n");

    // Push event onto the event queue
    fuse_event_t *event = fuse_new_event(self, (fuse_value_t* )self, FUSE_EVENT_NULL, NULL);
    assert(event);

    // Pop event from the event queue - core 0
    fuse_event_t *popped = fuse_next_event(self, 0);
    assert(popped);
    assert(popped == event);

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
