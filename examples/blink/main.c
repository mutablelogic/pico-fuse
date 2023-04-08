
#include <picofuse/picofuse.h>
#include <stdio.h>

// Timer which is used to toggle the LED
picofuse_timer_t timer = {
    .delay_ms = 1000,
    .periodic = true,
};

// Create timer on initialization
picofuse_state_t main_init(picofuse_t *self, picofuse_event_t event, void *data)
{
    picofuse_fire(self, EV_TIMER_INIT, &timer);
    return ANY;
}

// Toggle the LED
picofuse_state_t main_timer(picofuse_t *self, picofuse_event_t event, void *data)
{
    picofuse_timer_t *timer = (picofuse_timer_t *)data;

    // Cancel timer after 10 firings
    if (timer->id++ > 10)
    {
        // Cancel timer
        timer->periodic = false;
    }

    // Set the LED value
    picofuse_fire_bool(self, EV_LED, picofuse_state(self) ? 1 : 0);

    // Set the state to the alternative value
    return picofuse_state(self) ? 0 : 1;
}

int main()
{
    // Initialize the picofuse object with debugging enabled
    picofuse_t *picofuse = picofuse_init(PICOFUSE_DEBUG);

    // Register things we're going to use
    picofuse_register(picofuse, ANY, EV_INIT, main_init);
    picofuse_register(picofuse, ANY, EV_TIMER, main_timer);
    picofuse_register(picofuse, ANY, EV_LED, NULL);

    // Call main loop
    picofuse_main(picofuse);
}
