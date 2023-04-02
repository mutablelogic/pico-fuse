
#ifndef ADC_H
#define ADC_H

// Initialize the ADC and optionally the temperature sensor
extern void mq_adc_init(int temp_sensor);
extern float mq_adc_read_temp();

#endif
