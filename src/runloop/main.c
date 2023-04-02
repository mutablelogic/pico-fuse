
#include <stdio.h>
#include <pico/stdlib.h>
#include "runloop.h"

int main()
{
    // Initialize the board
    stdio_init_all();
    sleep_ms(250);

    // Set up the runloop
    runloop_t* runloop = runloop_init();

    // Run forever
    printf("Running the runloop forever...\n");
    runloop_run(runloop);
}
