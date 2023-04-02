#include <pico/stdlib.h>
#include <hardware/adc.h>
#include "adc.h"

void mq_adc_init(int temp_sensor)
{
    adc_init();
    if (temp_sensor)
    {
        adc_set_temp_sensor_enabled(true);
    }
}

uint16_t mq_adc_read(int sensor)
{
    adc_select_input(sensor);
    sleep_ms(1);
    return adc_read();
}

float mq_adc_read_temp()
{
    float v = (float)mq_adc_read(4) * (3.3f / (1 << 12));
    return 27.0f - (v - 0.706f) / 0.001721f;
}
