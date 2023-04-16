#include <pico/stdlib.h>
#include <hardware/pwm.h>
#include <hardware/irq.h>
#include <hardware/clocks.h>
#include <fuse/fuse.h>
#include <rp2040/pwm.h>
#include <rp2040/gpio.h>

#include <stdio.h>

///////////////////////////////////////////////////////////////////////////////

struct pwm_instance
{
    fuse_t *fuse;
};

// The number of PWM slices
#define PWM_COUNT NUM_PWM_SLICES
#define TOP_MAX 65534

// The global PWM driver
struct pwm_instance *_pwm = NULL;

///////////////////////////////////////////////////////////////////////////////
// PRIVATE METHODS

/*
 * Handle GPIO IRQ
 */
void rp2040_pwm_irq()
{
    assert(_pwm);
    assert(_pwm->fuse);

    // Clear the interrupt flag that brought us here
    pwm_clear_irq(pwm_gpio_to_slice_num(PIN));
    fuse_event_fire(_gpio->fuse, EV_PWM, pin);
}

void on_pwm_wrap()
{
    // printf("Wrapping\n");
}

// Returns: floor((16*F + offset) / div16)
// Avoids overflow in the numerator that would occur if
//   16*F + offset > 2**32
//   F + offset/16 > 2**28 = 268435456 (approximately, due to flooring)
uint32_t get_slice_hz(uint32_t offset, uint32_t div16)
{
    uint32_t source_hz = clock_get_hz(clk_sys);
    if (source_hz + offset / 16 > 268000000)
    {
        return (16 * (uint64_t)source_hz + offset) / div16;
    }
    else
    {
        return (16 * source_hz + offset) / div16;
    }
}

// Returns 16*F / denom, rounded.
static uint32_t get_slice_hz_round(uint32_t div16)
{
    return get_slice_hz(div16 / 2, div16);
}

// Returns ceil(16*F / denom).
static uint32_t get_slice_hz_ceil(uint32_t div16)
{
    return get_slice_hz(div16 - 1, div16);
}

static bool pwm_set_freq(int slice, int channel, uint32_t freq)
{
    assert(slice >= 0 && slice < PWM_COUNT);
    assert(channel >= 0 && channel < 2);
    assert(freq > 0);

    uint32_t source_hz = clock_get_hz(clk_sys);
    uint32_t div16;
    uint32_t top;
    if ((source_hz + freq / 2) / freq < TOP_MAX)
    {
        div16 = 16;
        top = (source_hz + freq / 2) / freq - 1;
    }
    else
    {
        div16 = get_slice_hz_ceil(TOP_MAX * freq);
        top = get_slice_hz_round(div16 * freq) - 1;
    }

    if (div16 < 16)
    {
        return false;
    }
    else if (div16 >= 256 * 16)
    {
        return false;
    }

    printf("Slice %d, Channel %d, Freq %d, Divider %d/%d\n", slice, channel, freq, div16, top);
    pwm_set_clkdiv_int_frac(slice, div16, top);
    return true;
}

/*
 * Initialize all GPIO pins
 */
static void rp2040_pwm_init(fuse_t *fuse, fuse_driver_t *driver, fuse_event_t event, void *data)
{
    assert(fuse);
    assert(driver);
    assert(event == EV_GPIO_PWM_INIT);

    gpio_pin_t *pin = data;
    pwm_t *pwm = fuse_malloc(fuse, sizeof(pwm_t));
    if (pwm == NULL)
    {
        fuse_debugf(fuse, "rp2040_pwm_init: failed to allocate memory for pwm\n");
        return;
    }

    // Initialize
    pwm->gpio = pin->gpio;
    pwm->slice = pwm_gpio_to_slice_num(pin->gpio);
    pwm->channel = pwm_gpio_to_channel(pin->gpio);
    pwm->freq = 0;
    pwm->duty_cycle = 75;

    // Fire EV_PWM_INIT
    if (!fuse_event_fire(fuse, EV_PWM_INIT, pwm))
    {
        fuse_debugf(fuse, "rp2040_pwm_init: failed to fire EV_PWM_INIT event\n");
    }
}

/*
 * Finalizer for PWM
 */
static void rp2040_pwm_finalizer(fuse_t *fuse, fuse_driver_t *driver, fuse_event_t event, void *data)
{
    assert(fuse);
    assert(driver);
    assert(event == EV_PWM_INIT);

    // Check gpio, slice and channel
    pwm_t *pwm = data;
    if (pwm_gpio_to_channel(pwm->gpio) != pwm->channel || pwm_gpio_to_slice_num(pwm->gpio) != pwm->slice)
    {
        fuse_debugf(fuse, "rp2040_pwm_finalizer: invalid gpio pin %d\n", pwm->gpio);
        return;
    }

    // Check duty cycle, frequency
    if (pwm->duty_cycle < 0 || pwm->duty_cycle > 100 || pwm->freq < 0)
    {
        fuse_debugf(fuse, "rp2040_pwm_finalizer: invalid duty_cycle for gpio pin %d\n", pwm->gpio);
        return;
    }

    // Clear any existing IRQ
    pwm_clear_irq(pwm->slice);
    irq_set_enabled(PWM_IRQ_WRAP, false);

    // Check frequency
    if (pwm->freq == 0)
    {
        pwm_set_enabled(pwm->slice, false);
    }
    else
    {
        fuse_debugf(fuse, "rp2040_pwm_finalizer: gpio pin %d, freq %d, duty_cycle %d\n", pwm->gpio, pwm->freq, pwm->duty_cycle);
        if (!pwm_set_freq(pwm->slice, pwm->channel, pwm->freq))
        {
            fuse_debugf(fuse, "rp2040_pwm_finalizer: failed to set frequency %dHz for gpio pin %d\n", pwm->freq, pwm->gpio);
            return;
        }
        else
        {
            pwm_set_wrap(pwm->slice, 65535);
            pwm_set_chan_level(pwm->slice, pwm->channel, (uint16_t)(65535 * pwm->duty_cycle / 100));
            pwm_set_enabled(pwm->slice, true);
        }
    }

    // Set IRQ
    // pwm_set_irq_enabled(pwm->slice, true);
    // irq_set_exclusive_handler(PWM_IRQ_WRAP, on_pwm_wrap);
    // irq_set_enabled(PWM_IRQ_WRAP, true);
}

/*
 * Register GPIO
 */
static void *rp2040_pwm_register(fuse_t *fuse, void *data)
{
    assert(fuse);

    // Creat a gpio object
    struct pwm_instance *self = fuse_malloc(fuse, sizeof(struct pwm_instance));
    if (self == NULL)
    {
        fuse_debugf(fuse, "rp2040_pwm_register: failed to allocate memory for driver\n");
        return NULL;
    }

    // Set fuse object
    self->fuse = fuse;
    _pwm = self;

    // Call rp2040_pwm_init
    if (!fuse_register_action(fuse, EV_GPIO_PWM_INIT, self, rp2040_pwm_init))
    {
        fuse_debugf(fuse, "rp2040_pwm_register: failed to register EV_GPIO_PWM_INIT action\n");
        return NULL;
    }

    // Return success
    return self;
}

///////////////////////////////////////////////////////////////////////////////

fuse_driver_params_t rp2040_pwm = {
    .name = "rp2040_pwm",
    .init = rp2040_pwm_register,
    .events = {
        {.event = EV_PWM_INIT, .name = "EV_PWM_INIT", .finalizer = rp2040_pwm_finalizer},
        {.event = 0, .name = NULL}}};
