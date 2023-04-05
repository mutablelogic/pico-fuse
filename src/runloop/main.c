
#include <stdio.h>
#include <pico/stdlib.h>
#include "runloop.h"
#include "opts.h"

// GPIO pin definitions
runloop_gpio_t bootsel = {.pinName = "BOOTSEL", .gpio = 23, .irqrise = true, .irqfall = true};

// Timer
runloop_timer_t timer = {.delay_ms = 1000};

///////////////////////////////////////////////////////////////////////////////
// EVENT_INIT handler

runloop_state_t my_main_init(runloop_t *runloop, runloop_state_t state, runloop_event_t event, void *data)
{
    // Set application name
    ((runloop_init_t *)data)->appName = "runloop testing application";

    // Enable GPIO23 (the BOOTSEL button) as input
    runloop_fire(runloop, EVENT_GPIO_INIT, &bootsel);

    // Enable a timer event every 1000ms
    runloop_fire(runloop, EVENT_TIMER_INIT, &timer);

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
// EVENT_GPIO_INIT and EVENT_GPIO

runloop_state_t my_gpio_init(runloop_t *runloop, runloop_state_t state, runloop_event_t event, void *data)
{
    runloop_gpio_t *gpio = (runloop_gpio_t *)data;
    printf("Called EVENT_GPIO_INIT handler for GPIO %d\n", gpio->gpio);

    // Return success
    return state;
}

runloop_state_t my_gpio_change(runloop_t *runloop, runloop_state_t state, runloop_event_t event, void *data)
{
    printf("Called EVENT_GPIO handler\n");

    // Switch LED on
    runloop_fire(runloop, EVENT_LED, (void *)true);

    // Switch LED off
    runloop_fire(runloop, EVENT_LED, (void *)false);

    // Return success
    return state;
}

///////////////////////////////////////////////////////////////////////////////
// EVENT_LED_INIT and EVENT_LED

runloop_state_t my_led_init(runloop_t *runloop, runloop_state_t state, runloop_event_t event, void *data)
{
    runloop_led_t *led = (runloop_led_t *)data;
    printf("Called EVENT_LED_INIT handler for gpio=%d, cyw43_arch=%d\n", led->gpio, led->cyw43_arch);

    // Set LED on
    led->value = true;
    runloop_fire(runloop, EVENT_LED, led);

    // Return success
    return state;
}

runloop_state_t my_led_change(runloop_t *runloop, runloop_state_t state, runloop_event_t event, void *data)
{
    runloop_led_t *led = (runloop_led_t *)data;
    printf("Called EVENT_LED handler for gpio=%d, cyw43_arch=%d value=%d\n", led->gpio, led->cyw43_arch, led->value);

    // Return success
    return state;
}

///////////////////////////////////////////////////////////////////////////////
// EVENT_TIMER

runloop_state_t my_gpio_timer(runloop_t *runloop, runloop_state_t state, runloop_event_t event, void *data)
{
    printf("Called EVENT_GPIO handler\n");

    // Switch LED on
    runloop_fire(runloop, EVENT_LED, (void *)true);

    // Return success
    return state;
}

///////////////////////////////////////////////////////////////////////////////
// EVENT_WIFI_INIT

runloop_state_t my_wifi_init(runloop_t *runloop, runloop_state_t state, runloop_event_t event, void *data)
{
    runloop_wifi_t *wifi = (runloop_wifi_t *)data;
#if defined(WIFI_SSID)
    wifi->ssidName = WIFI_SSID;    
#endif
#if defined(WIFI_PASSWORD)
    wifi->ssidPassword = WIFI_PASSWORD;    
#endif
    printf("Called EVENT_WIFI_INIT handler with ssid=%s\n", wifi->ssidName);

    // Return success
    return state;
}

///////////////////////////////////////////////////////////////////////////////
// main

int main()
{
    // Set up the runloop, with ADC_4 enabled (temperature sensor)
    runloop_t *runloop = runloop_init(WIFI | ADC_4 | LED);

    // Event handlers
    runloop_event(runloop, ANY, EVENT_INIT, my_main_init);
    runloop_event(runloop, ANY, EVENT_ADC_INIT, my_adc_init);
    runloop_event(runloop, ANY, EVENT_GPIO_INIT, my_gpio_init);
    runloop_event(runloop, ANY, EVENT_GPIO, my_gpio_change);
    runloop_event(runloop, ANY, EVENT_TIMER, my_gpio_timer);
    runloop_event(runloop, ANY, EVENT_LED_INIT, my_led_init);
    runloop_event(runloop, ANY, EVENT_LED, my_led_change);
    runloop_event(runloop, ANY, EVENT_WIFI_INIT, my_wifi_init);

    // Run forever
    runloop_main(runloop);
}
