/** @file adc.h
 *  @brief Function prototypes which control the Analog-to-Digital converter
 */
#ifndef PICOFUSE_ADC_H
#define PICOFUSE_ADC_H

#include <stdint.h>

#ifdef DEBUG
#define fuse_new_adc(self, channels, samples, rate) \
    ((fuse_adc_t *)fuse_new_adc_ex((self), (channels), (samples), (rate), __FILE__, __LINE__))
#else
#define fuse_new_adc(self, channels, samples, rate) \
    ((fuse_adc_t *)fuse_new_adc_ex((self), (channels), (samples), (rate), 0, 0))
#endif

#define FUSE_ADC0 0x001 ///< GPIO26, GPIO40
#define FUSE_ADC1 0x002 ///< GPIO27, GPIO41
#define FUSE_ADC2 0x004 ///< GPIO28, GPIO42
#define FUSE_ADC3 0x008 ///< GPIO29 (VSYS on Pico and Pico2), GPIO43 
#define FUSE_ADC4 0x010 ///< N/A, GPIO44
#define FUSE_ADC5 0x020 ///< N/A, GPIO45
#define FUSE_ADC6 0x040 ///< N/A, GPIO46
#define FUSE_ADC7 0x080 ///< N/A, GPIO47

/** @brief Create a new ADC instance
 *
 *  Set up an ADC instance for reading 12-bit samples from selected ADC channels in a
 *  round-robin. An event FUSE_EVENT_ADC is fired and can be handled for each sample buffer
 *  created. By default, the ADC sampling is not started and must be started with fuse_adc_run().
 * 
 * @param self The fuse instance
 * @param channels A bit-wise mask of ADC channels to sample
 * @param samples The number of samples to buffer before firing an event. This must be a multiple of the
 *               number of channels selected.
 * @param rate The approximate rate at which to sample the ADC channels, in Hz. The rate can be zero (maximum sampling rate)
 *           or between X and Y Hz
 * @param file The file name where the function is called
 * @param line The line number where the function is called
 * @return The ADC instance, or NULL if the ADC could not be created, or was already created, since only one instance
 *        can be created.
 */
fuse_adc_t* fuse_new_adc_ex(fuse_t* self, uint8_t channels, size_t samples, uint16_t rate, const char *file, const int line);

/* @brief Start sampling the ADC
 * 
 * The ADC is started and samples are taken at the rate specified when the ADC was created.
 * The samples are buffered and an event FUSE_EVENT_ADC is fired when the buffer is full.
 * The user_data to the event callback is a pointer to the buffer of samples, which are 16-bit values
 * with 12-bits of data, interleaved between the selected channels.
 * 
 * @param adc The ADC instance
 */
void fuse_adc_run(fuse_adc_t* adc);

/* @brief Stop sampling the ADC
 * 
 * The ADC sampling is stopped.
 * 
 * @param adc The ADC instance
 */
void fuse_adc_stop(fuse_adc_t* adc);

/** @brief Return the number of ADC channels
 *
 *  Return the number of ADC channels available on the device (not including
 *  the temperature sensor). For RP2040 and QFN-60 version of RP2350, the
 *  number of ADC channels is 4. For QFN-80 version of RP2350, this is 8.
 * 
 *  @return Returns the number of ADC channels, which are zero-indexed
 */
uint8_t fuse_adc_count();

#endif
