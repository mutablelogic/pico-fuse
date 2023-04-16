#include <fuse/fuse.h>
#include <rp2040/gpio.h>
#include <rp2040/led.h>
#include <rp2040/timer.h>
#include <rp2040/pwm.h>

void pin_init(fuse_t *fuse, fuse_driver_t *driver, fuse_event_t event, void *data)
{
    gpio_pin_t *pin = (gpio_pin_t *)data;
    if (pin->gpio == 23)
    {
        // BOOTSEL input
        pin->func = GPIO_INPUT;
        pin->irqfall = true;
        pin->irqrise = true;
    }
    if (pin->gpio == 25)
    {
        // PWM
        pin->func = GPIO_PWM;
    }
}

void main_init(fuse_t *fuse, fuse_driver_t *driver, fuse_event_t event, void *data)
{
    // Switch LED on
    fuse_event_fire_bool(fuse, EV_LED, true);

    // Create a timer which fires every second
    static fuse_timer_t timer = {
        .id = 1,
        .delay_ms = 1000,
        .repeat = true,
    };

    //fuse_event_fire(fuse, EV_TIMER_INIT, &timer);
}

void timer_fired(fuse_t *fuse, fuse_driver_t *driver, fuse_event_t event, void *data)
{
    fuse_timer_t *timer = (fuse_timer_t *)data;
    static int count = 0;

    fuse_debugf(fuse, "timer_fired with count=%d\n", count++);
    if (count > 10)
    {
        timer->repeat = false;
    }
}

void bootsel_select(fuse_t *fuse, fuse_driver_t *driver, fuse_event_t event, void *data)
{
    gpio_pin_t *pin = (gpio_pin_t *)data;
    if (pin->gpio == 23)
    {
        if (pin->irqrise)
        {
            // Switch LED on
            fuse_event_fire_bool(fuse, EV_LED, true);
        }
        if (pin->irqfall)
        {
            // Switch LED off
            fuse_event_fire_bool(fuse, EV_LED, false);
        }
    }
}

void pwm_init(fuse_t *fuse, fuse_driver_t *driver, fuse_event_t event, void *data)
{
    pwm_t *pwm = data;

    // Set 75% duty cycle at 1Hz
    pwm->freq = 8;
    pwm->duty_cycle = 1;
}

int main()
{
    // Create application
    fuse_t *app = fuse_init(64 * 1024, FUSE_FLAG_DEBUG);

    // Register drivers
    fuse_register_driver(app, &rp2040_gpio, NULL);
    fuse_register_driver(app, &rp2040_led, NULL);
    fuse_register_driver(app, &rp2040_timer, NULL);
    fuse_register_driver(app, &rp2040_pwm, NULL);

    // Register actions
    fuse_register_action(app, EV_INIT, NULL, main_init);
    fuse_register_action(app, EV_GPIO_INIT, NULL, pin_init);
    fuse_register_action(app, EV_GPIO, NULL, bootsel_select);
    fuse_register_action(app, EV_TIMER, NULL, timer_fired);
    fuse_register_action(app, EV_PWM_INIT, NULL, pwm_init);

    // Runloop until exit
    fuse_main(app);

    // Exit
    return fuse_exit(app);
}
