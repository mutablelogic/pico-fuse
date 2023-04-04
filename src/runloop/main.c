
#include <stdio.h>
#include <pico/stdlib.h>
#include "runloop.h"

// GPIO pin definitions
runloop_gpio_t bootsel = {.pinName = "BOOTSEL", .gpio = 23, .irqrise = true, .irqfall = true};

///////////////////////////////////////////////////////////////////////////////
// EVENT_INIT handler

runloop_state_t my_main_init(runloop_t *runloop, runloop_state_t state, runloop_event_t event, void *data)
{
    // Set application name
    ((runloop_init_t *)data)->appName = "runloop testing application";

    // Enable GPIO23 (the BOOTSEL button) as input
    runloop_push(runloop, EVENT_GPIO_INIT, &bootsel);

    // Return success
    return state;
}

///////////////////////////////////////////////////////////////////////////////
// EVENT_ADC_INIT handler

runloop_state_t my_adc_init(runloop_t *runloop, runloop_state_t state, runloop_event_t event, void *data)
{
    runloop_adc_t *adc = (runloop_adc_t *)data;
    printf("Called EVENT_ADC_INIT handler for channel %d gpio %d\n", adc->channel, adc->gpio);

    // Return success
    return state;
}

///////////////////////////////////////////////////////////////////////////////
// EVENT_GPIO_INIT handler

runloop_state_t my_gpio_init(runloop_t *runloop, runloop_state_t state, runloop_event_t event, void *data)
{
    runloop_gpio_t *gpio = (runloop_gpio_t *)data;
    printf("Called EVENT_GPIO_INIT handler for GPIO %d\n", gpio->gpio);

    // Return success
    return state;
}

///////////////////////////////////////////////////////////////////////////////
// main

int main()
{
    // Set up the runloop, with ADC_4 enabled (temperature sensor)
    runloop_t *runloop = runloop_init(ADC_4);

    // Event handlers
    runloop_event(runloop, ANY, EVENT_INIT, my_main_init);
    runloop_event(runloop, ANY, EVENT_ADC_INIT, my_adc_init);
    runloop_event(runloop, ANY, EVENT_GPIO_INIT, my_gpio_init);

    // Run forever
    runloop_main(runloop);
}
