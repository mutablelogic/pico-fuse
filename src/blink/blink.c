
#include <picofuse/picofuse.h>
#include <stdio.h>
#include "opts.h"

picofuse_timer_t timer_1 = {
    .id = 1,
    .delay_ms = 100,
    .periodic = false,
};

picofuse_timer_t timer_2 = {
    .id = 2,
    .delay_ms = 100,
    .periodic = true,
};

// Set countrycode and create two timers
picofuse_state_t main_init(picofuse_t *self, picofuse_event_t event, void *data)
{
    ((picofuse_init_t *)data)->countryCode = "DE";
    picofuse_fire(self, EV_TIMER_INIT, &timer_1);
    picofuse_fire(self, EV_TIMER_INIT, &timer_2);
    return ANY;
}

// Set initial state of the LED
picofuse_state_t main_led_init(picofuse_t *self, picofuse_event_t event, void *data)
{
    ((picofuse_led_t *)data)->value = 1;
    return ANY;
}

// Timer fired
picofuse_state_t main_timer(picofuse_t *self, picofuse_event_t event, void *data)
{
    picofuse_timer_t *timer = (picofuse_timer_t *)data;
    timer->id++;
    if (timer->id == 10)
    {
        timer->periodic = false;
    }

    // Set the LED value
    picofuse_fire(self, EV_LED, picofuse_state(self) ? (void*)1 : (void*)0);

    // Set the state to an alternative value
    return picofuse_state(self) ? 0 : 1;
}

// WiFi initialization
picofuse_state_t main_wifi_init(picofuse_t *self, picofuse_event_t event, void *data)
{
    picofuse_wifi_t *wifi = (picofuse_wifi_t *)data;
#ifdef WIFI_SSID
    wifi->ssid = WIFI_SSID;
#ifdef WIFI_PASSWORD
    wifi->password = WIFI_PASSWORD;
#endif
#endif
    return ANY;
}

int main()
{
    // Initialize the picofuse object with debugging enabled
    picofuse_t *picofuse = picofuse_init(PICOFUSE_DEBUG);

    // Register callbacks
    picofuse_register(picofuse, ANY, EV_INIT, main_init);
    picofuse_register(picofuse, ANY, EV_LED_INIT, main_led_init);
    picofuse_register(picofuse, ANY, EV_WIFI_INIT, main_wifi_init);
    picofuse_register(picofuse, ANY, EV_TIMER, main_timer);

    // Call main loop
    int errorCode = picofuse_main(picofuse);

    // Free resources
    picofuse_free(picofuse);

    // Return error code
    return errorCode;
}
