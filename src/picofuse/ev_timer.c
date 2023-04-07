#include <picofuse/picofuse.h>
#include <pico/stdlib.h>

#include "ev.h"
#include "debug.h"

///////////////////////////////////////////////////////////////////////////////
// Handle the EVENT_TIMER_INIT event

void picofuse_handle_timer_init(picofuse_t * self, picofuse_timer_t * data) {
    // Call the registered event handler
    picofuse_callback(self, picofuse_event(self), (void *)(data));
}

///////////////////////////////////////////////////////////////////////////////
// Handle the EVENT_TIMER event

void picofuse_handle_timer(picofuse_t * self, picofuse_timer_t * data) {
    // Call the registered event handler
    picofuse_callback(self, picofuse_event(self), (void *)(data));
}
