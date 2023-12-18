/** @file adc.h
 *  @brief Function prototypes which control the Analog-to-Digital convertor
 */
#ifndef PICOFUSE_ADC_H
#define PICOFUSE_ADC_H

#include <stdint.h>

#define FUSE_ADC_INVALID (0xFF) ///< ADC channel is invalid

/** @brief Initialize the ADC on a specific GPIO pin
 *
 *  Enables the ADC channel on a pin. Enables the temperature ADC channel.
 *
 *  @param pin The GPIO pin number
 *  @return The ADC channel number, or FUSE_ADC_INVALID if the GPIO pin is not valid
 */
uint8_t fuse_adc_new(uint8_t pin);

/** @brief Reset the ADC on a specific GPIO pin
 *
 * Resets the function for the pin. Disables the temperature ADC channel.
 */
void fuse_adc_destroy(uint8_t pin);

/** @brief Return the number of ADC channels
 *
 *  @return Returns the number of ADC channels, which are zero-indexed
 */
uint8_t fuse_adc_count();

/** @brief Return the channel for the GPIO pin
 *
 *  @param pin The GPIO pin
 *  @return Returns the channel number, or FUSE_ADC_INVALID if the GPIO pin
 *          is not associated with a channel
 */
uint8_t fuse_adc_channel(uint8_t pin);

/** @brief Return the pin for the ADC channel
 *
 *  @param channel The ADC channel
 *  @return Returns the GPIO pin, or FUSE_ADC_INVALID if the GPIO pin
 *          is not associated with a channel
 */
uint8_t fuse_adc_pin(uint8_t channel);

/** @brief Return the channel for the temperature sensor
 *
 *  @return Returns the channel number for the temperature sensor
 *          or FUSE_ADC_INVALID if an error occurred
 */
uint8_t fuse_adc_channel_temp();

/** @brief Get the raw value of the ADC channel
 *
 *  Select the input, and perform a single conversion on the ADC
 *
 *  @param channel The ADC channel
 *  @param mask A pointer to the bitmask for the significant bits, or NULL
 *              if the bitmask should not be set
 *  @return The raw value of the ADC channel
 */
uint32_t fuse_adc_get(uint8_t channel, uint32_t *mask);

/** @brief Get the celcius value of the ADC channel
 *
 *  Select the input, and perform a single conversion on the ADC, and return the
 *  celcius temperature value 
 *
 *  @param channel The ADC channel
 *  @return The celcius temperature value of the ADC channel
 */
float fuse_adc_get_temp(uint8_t channel);

/** @brief Get the voltage value of the ADC channel
 *
 *  Select the input, and perform a single conversion on the ADC, and return the
 *  voltage value, assuming ADC_VREF == 3.3 V
 *
 *  @param channel The ADC channel
 *  @return The voltage value of the ADC channel
 */
float fuse_adc_get_voltage(uint8_t channel);

#endif
