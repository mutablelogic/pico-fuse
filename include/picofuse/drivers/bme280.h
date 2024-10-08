/** @file bme280.h
 *  @brief Function prototypes for temp/pressure/humidity sensor BME280
 */
#ifndef PICOFUSE_BME280_H
#define PICOFUSE_BME280_H
#include <picofuse/spi.h>

#ifdef DEBUG
#define fuse_new_bme280(self, spi) \
    ((fuse_bme280_t *)fuse_new_bme280_ex((self), (spi), __FILE__, __LINE__))
#else
#define fuse_new_bme280(self, spi) \
    ((fuse_bme280_t *)fuse_new_bme280_ex((self), (spi), 0, 0))
#endif

/** @brief An opaque BME280 object
 */
typedef struct bme280_context fuse_bme280_t;

/** @brief A BME280 measurement 
 */
typedef struct {
    float temperature; ///< Temperature in degrees Celsius, or 0 if not measured
    float pressure;    ///< Pressure in Pascals, or 0 if not measured
    float humidity;    ///< Humidity in %RH, or 0 if not measured
} fuse_bme280_measurement_t;

/** @brief Create a new BME280 driver with SPI interface
 *
 * @param self The fuse application
 * @param spi The SPI interface
 * @return The BME280, or NULL if the BME280 could not be initialized
 */
fuse_bme280_t *fuse_new_bme280_ex(fuse_t *self, fuse_spi_t* spi, const char *file, const int line);

/** @brief Measure temperature, pressure and humidity once
 * 
 * Measure the temperature, pressure and humidity, and return the measurement, or NULL if the 
 * measurement failed.
 * 
 * @param self The fuse application
 * @param bme280 The BME280 instance
 * @return The measurement, or NULL if the measurement failed
 */
fuse_bme280_measurement_t* fuse_bme280_measure(fuse_t *self, fuse_bme280_t *bme280);

/** @brief Measure temperature, pressure and humidity periodically
 * 
 * Measure the temperature, pressure and humidity, and emit an event with the measurements.
 * If periodic_ms is non-zero, then the measurement event will be emitted periodically. If zero,
 * a single event will be emitted after the measurement is completed.
 * 
 * @param self The fuse application
 * @param bme280 The BME280 instance
 * @param periodic_ms The periodic interval, in milliseconds, to emit a measurement event
 * @return True if the measurement was successful
 */
bool fuse_bme280_read(fuse_t *self, fuse_bme280_t *bme280, uint32_t periodic_ms);

#endif
