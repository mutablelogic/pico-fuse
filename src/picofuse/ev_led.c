#include <picofuse/picofuse.h>
#include <pico/stdlib.h>

#ifdef PICO_CYW43_SUPPORTED
#include <pico/cyw43_arch.h>
#endif

#include "ev.h"
#include "debug.h"

///////////////////////////////////////////////////////////////////////////////
// Set LED value

static void picofuse_set_led(picofuse_led_t *data)
{
    if (data->cyw43_arch)
    {
#ifdef PICO_CYW43_SUPPORTED
        cyw43_arch_gpio_put(data->gpio, data->value ? 1 : 0);
#else
        picofuse_debug("picofuse_set_led: PICO_CYW43_SUPPORTED not defined\n");
#endif
    }
    else if (data->gpio != -1)
    {
        gpio_put(data->gpio, data->value);
    }
}

///////////////////////////////////////////////////////////////////////////////
// Handle the EVENT_LED_INIT event

void picofuse_handle_led_init(picofuse_t *self, picofuse_led_t *data)
{
#if defined(PICO_DEFAULT_LED_PIN)
    data->gpio = PICO_DEFAULT_LED_PIN;
#elif defined(CYW43_WL_GPIO_LED_PIN)
    data->cyw43_arch = true;
    data->gpio = CYW43_WL_GPIO_LED_PIN;
#else
    data->gpio = -1;
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

    // Set initial LED value
    picofuse_set_led(data);
}

///////////////////////////////////////////////////////////////////////////////
// Handle the EVENT_LED event

void picofuse_handle_led(picofuse_t *self, picofuse_led_t *data, bool value)
{
    // If the value hasn't changed, do nothing
    if (data->value == value)
    {
        return;
    }
    else
    {
        data->value = value;
    }

    // Call the registered event handler
    picofuse_callback(self, picofuse_event(self), (void *)(data));

    // Set LED value
    picofuse_set_led(data);
}
