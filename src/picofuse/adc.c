#include <fuse/fuse.h>
#include <picofuse/picofuse.h>
#include <hardware/platform_defs.h>
#include <hardware/adc.h>

#define ADC_MASK (0xFFF)
#define ADC_VREF (3.3f)

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

uint8_t fuse_adc_new(uint8_t pin)
{
    assert(pin < fuse_gpio_count());

    // Initialize the ADC
    static bool adc = false;
    if (!adc)
    {
        adc_init();
        adc = true;
    }

    uint8_t channel = fuse_adc_channel(pin);
    if (channel == FUSE_ADC_INVALID)
    {
        return channel;
    }

    // Initialize the pin
    adc_gpio_init(pin);

    // If the ADC is associated with the temperature sensor, enable it
    if (fuse_adc_channel(pin) == fuse_adc_channel_temp())
    {
        adc_set_temp_sensor_enabled(true);
    }

    // Return the channel associated with the pin
    return channel;
}

void fuse_adc_destroy(uint8_t pin)
{
    assert(pin < fuse_gpio_count());
    assert(fuse_adc_channel(pin) != FUSE_ADC_INVALID);

    // Disable the temperature sensor if it is enabled
    if (fuse_adc_channel(pin) == fuse_adc_channel_temp())
    {
        adc_set_temp_sensor_enabled(false);
    }

    // Reset the GPIO pin
    fuse_gpio_destroy(pin);
}

inline uint8_t fuse_adc_count()
{
    return NUM_ADC_CHANNELS;
}

inline uint8_t fuse_adc_channel(uint8_t pin)
{
    assert(pin < fuse_gpio_count());
    if (pin < 26 || pin > 30)
    {
        return FUSE_ADC_INVALID;
    }
    return pin - 26;
}

inline uint8_t fuse_adc_pin(uint8_t channel)
{
    assert(channel < fuse_adc_count());
    return channel + 26;
}

inline uint8_t fuse_adc_channel_temp()
{
    // The temperature channel is the last channel
    return NUM_ADC_CHANNELS - 1;
}

uint32_t fuse_adc_get(uint8_t channel, uint32_t *mask)
{
    assert(channel < fuse_adc_count());
    adc_select_input(channel);
    if (mask != NULL)
    {
        *mask = ADC_MASK;
    }
    return adc_read() & ADC_MASK;
}

float fuse_adc_get_voltage(uint8_t channel)
{
    assert(channel < fuse_adc_count());
    uint32_t value = fuse_adc_get(channel,NULL);
    return (float)value *  ADC_VREF / (float)ADC_MASK;
}

float fuse_adc_get_temp(uint8_t channel)
{
    float value = fuse_adc_get_voltage(channel);
    return 27. - (value - 0.706f) / 0.001721f;
}
