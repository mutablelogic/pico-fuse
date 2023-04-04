
#include <stdio.h>
#include <pico/stdlib.h>
#include "runloop.h"

///////////////////////////////////////////////////////////////////////////////
// EVENT_INIT handler

runloop_state_t main_init(runloop_t *runloop, runloop_state_t state, runloop_event_t event, void *data)
{
    ((runloop_init_t* )data)->appName = "runloop testing application";

    // Don't change the state
    return state;
}

///////////////////////////////////////////////////////////////////////////////
// main

int main()
{
    // Set up the runloop, with ADC_4 enabled (temperature sensor)
    runloop_t *runloop = runloop_init(ADC_4);
    if (runloop == NULL)
    {
        while (true)
        {
            printf("Failed to initialize runloop=%p\n", runloop);
            sleep_ms(1000);
        }
    }

    // Add init event for EVENT_INIT
    runloop_event(runloop, ANY, EVENT_INIT, main_init);

    // Run forever
    runloop_main(runloop);
}
