/** @file bme280.h
 *  @brief Function prototypes for temp/pressure/humidity sensor BME280
 */
#ifndef PICOFUSE_BME280_H
#define PICOFUSE_BME280_H

#include "spi.h"

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

/** @brief Create a new BME280 driver with SPI interface
 *
 * @param self The fuse application
 * @param spi The SPI interface
 * @return The BME280, or NULL if the BME280 could not be initialized
 */
fuse_bme280_t *fuse_new_bme280_ex(fuse_t *self, fuse_spi_t* spi, const char *file, const int line);

#endif
