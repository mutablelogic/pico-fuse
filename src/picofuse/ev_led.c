#include <picofuse/picofuse.h>
#include <pico/stdlib.h>

#include "ev.h"
#include "debug.h"

///////////////////////////////////////////////////////////////////////////////
// Handle the EVENT_LED_INIT event

void picofuse_handle_led_init(picofuse_t * self, picofuse_led_t * data) {
#if defined(PICO_DEFAULT_LED_PIN)
    data->gpio = PICO_DEFAULT_LED_PIN;
#elif defined(CYW43_WL_GPIO_LED_PIN)
    data->cyw43_arch = true;
    data->gpio = CYW43_WL_GPIO_LED_PIN;
#endif

    // Call the registered event handler
    picofuse_callback(self, picofuse_event(self), (void *)(data));

    // Perform any initialization
    if (data->gpio != 0)
    {
        if (!data->cyw43_arch)
        {
            gpio_init(data->gpio);
            gpio_set_dir(data->gpio, GPIO_OUT);
        }
    }    
}

///////////////////////////////////////////////////////////////////////////////
// Handle the EVENT_LED event

void picofuse_handle_led(picofuse_t * self, picofuse_led_t * data) {
    // Call the registered event handler
    picofuse_callback(self, picofuse_event(self), (void *)(data));

    // Set the LED value
    if (data->gpio != 0)
    {
        if (data->cyw43_arch)
        {
#ifdef PICO_CYW43_SUPPORTED
            cyw43_arch_gpio_put(data->gpio, data->value);
#endif
        }
        else
        {
            gpio_put(data->gpio, data->value);
        }
    }
}
