#include <pico/stdlib.h>
#include <fuse/fuse.h>

#ifdef PICO_CYW43_SUPPORTED
#include <pico/cyw43_arch.h>
#endif

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

void fuse_led_new()
{
#if defined(PICO_DEFAULT_LED_PIN)
        fuse_debugf(NULL, "picofuse_led_new: gpio %d\n",PICO_DEFAULT_LED_PIN);
        gpio_init(PICO_DEFAULT_LED_PIN);
        gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
#endif
}

void fuse_led_destroy()
{
#if defined(PICO_DEFAULT_LED_PIN)
        fuse_debugf(NULL, "picofuse_led_destroy: gpio %d\n",PICO_DEFAULT_LED_PIN);
        gpio_deinit(PICO_DEFAULT_LED_PIN);
#endif
}

void fuse_led_set(bool value)
{
#if defined(CYW43_WL_GPIO_LED_PIN)
        fuse_debugf(NULL, "picofuse_led_set: %s on cyw43 gpio %d\n", value ? "true" : "false", CYW43_WL_GPIO_LED_PIN);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, value ? 1 : 0);
#elif defined(PICO_DEFAULT_LED_PIN)
        fuse_debugf(NULL, "picofuse_led_set: %s on gpio %d\n", value ? "true" : "false", PICO_DEFAULT_LED_PIN);
        gpio_put(PICO_DEFAULT_LED_PIN, value ? 1 : 0);
#else
        fuse_debugf(NULL, "picofuse_led_set: missing led\n");
#endif
}
