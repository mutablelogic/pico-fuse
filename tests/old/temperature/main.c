
#include <stdio.h>
#include <fuse/fuse.h>
#include <picofuse/picofuse.h>

int run(fuse_t *fuse)
{
    uint8_t temp_channel = fuse_adc_channel_temp();
    assert(temp_channel != FUSE_ADC_INVALID);

    // Initalize the temperature sensor
    fuse_gpio_new(fuse_adc_pin(temp_channel), FUSE_GPIO_ADC);

    // Read the raw value of the temperature sensor
    for (int i = 0; i < 10; i++)
    {
        printf("Temp sensor=%.2fC\n", fuse_adc_get_temp(temp_channel));
        sleep_ms(500);
    }

    return 0;
}

int main()
{
    return fuse_main(FUSE_FLAG_DEBUG, run);
}
