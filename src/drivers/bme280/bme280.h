/** @file bme280.h
 *  @brief Private function prototypes and structure definitions for BME280 sensor
 */
#ifndef FUSE_PRIVATE_BME280_H
#define FUSE_PRIVATE_BME280_H

#include <picofuse/picofuse.h>
#include <stdint.h>

/** @brief BME280 context
 */
struct bme280_context
{
    fuse_spi_t* spi;
    uint8_t id;

    // Temperature compensation parameters
    uint16_t T1;
    int16_t T2;
    int16_t T3;

    // Pressure compensation parameters
    uint16_t P1;
    int16_t P2;
    int16_t P3;
    int16_t P4;
    int16_t P5;
    int16_t P6;
    int16_t P7;
    int16_t P8;
    int16_t P9;

    // Humidity compensation parameters
    uint8_t H1;
    int16_t H2;
    uint8_t H3;
    int16_t H4;
    int16_t H5;
    int8_t H6;
};

// Expected chip id
#define BME280_CHIP_ID 0x60

/** @brief Read the BME280 chip id
 * 
 * @param self The fuse application
 * @param bme280 The BME280 instance
 * @return The chip id
 */
uint8_t bme280_read_chip_id(fuse_t* self, fuse_bme280_t* bme280);

/** @brief Reset the BME280
 * 
 * @param self The fuse application
 * @param bme280 The BME280 instance
 */
void bme280_reset(fuse_t* self, fuse_bme280_t* bme280);


/** @brief Read the BME280 device status
 * 
 * @param self The fuse application
 * @param bme280 The BME280 instance
 * @param measuring Pointer to the measuring status, or NULL
 * @param updating Pointer to the updating status, or NULL
 */
void bme280_read_status(fuse_t* self, fuse_bme280_t* bme280, bool* measuring, bool* updating);

/** @brief Initialise the BME280 compensation parameters
 * 
 * @param self The fuse application
 * @param bme280 The BME280 instance
 */
void bme280_read_compensation_parameters(fuse_t* self, fuse_bme280_t* bme280);

/** @brief Read the BME280 registers as raw values
 * 
 * Read raw values, and must be converted to the correct units using the compensation parameters
 * 
 * @param self The fuse application
 * @param bme280 The BME280 instance
 * @param temperature Pointer to the temperature value, or NULL
 * @param pressure Pointer to the pressure value, or NULL
 * @param humidity Pointer to the humidity value, or NULL
 */
void bme280_read_raw(fuse_t* self, fuse_bme280_t* bme280, int32_t *temperature,int32_t *pressure,int32_t *humidity);

/** @brief Compensate temperature value
 *
 * Provide a raw temperature value, and this function will return the compensated temperature value
 * 
 * @param bme280 The BME280 instance
 * @param temperature The raw temperature value
 * @param fine A pointer to the intermediate temperature coefficient, or NULL
 * @return The compensated temperature value
 */
int32_t bme280_compensate_temperature(fuse_bme280_t* bme280, int32_t temperature, int32_t* fine);

/** @brief Compensate pressure value
 *
 * Provided with a raw pressure value and intermediate temperature coefficient,  this function will 
 * return the compensated pressure value
 * 
 * @param bme280 The BME280 instance
 * @param pressure The raw pressure value
 * @param fine The intermediate temperature coefficient
 * @return The compensated pressure value
 */
uint32_t bme280_compensate_pressure(fuse_bme280_t* bme280, int32_t pressure, int32_t fine);

/** @brief Compensate humidity value
 *
 * Provided with a raw humidity value,  this function will return the compensated humidity value. The output humidity 
 * should be divided by 1024 to get the value in %RH
 * 
 * @param bme280 The BME280 instance
 * @param humidity The raw humidity value
 * @param fine The intermediate temperature coefficient
 * @return The compensated humidity value
 */
uint32_t bme280_compensate_humidity(fuse_bme280_t* bme280, int32_t humidity, int32_t fine);

/** @brief Set oversampling values
 * 
 * @param self The fuse application
 * @param bme280 The BME280 instance
 * @param osrs_t Temperature oversampling value (0, 1, 2, 4, 8 or 16)
 * @param osrs_p Pressure oversampling value (0, 1, 2, 4, 8 or 16)
 * @param osrs_h Humidity oversampling value (0, 1, 2, 4, 8 or 16)
 */
void bme280_write_osrs(fuse_t* self, fuse_bme280_t* bme280, uint8_t osrs_t, uint8_t osrs_p, uint8_t osrs_h);

/** @brief Read measurement mode
 * 
 * @param self The fuse application
 * @param bme280 The BME280 instance
 * @return The measurement mode (0 for sleep, 1 for forced, 2 for normal)
 */
uint8_t bme280_read_mode(fuse_t* self, fuse_bme280_t* bme280);

/** @brief Write measurement mode
 * 
 * @param self The fuse application
 * @param bme280 The BME280 instance
 * @param mode The measurement mode (0 for sleep, 1 for forced, 2 for normal)
 */
void bme280_write_mode(fuse_t* self, fuse_bme280_t* bme280, uint8_t mode);

#endif
