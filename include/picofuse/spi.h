/** @file spi.h
 *  @brief Function prototypes for the SPI interface
 */
#ifndef PICOFUSE_SPI_H
#define PICOFUSE_SPI_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#ifdef DEBUG
#define fuse_new_spi(self, data) \
    ((fuse_spi_t *)fuse_new_spi_ex((self), (data), __FILE__, __LINE__))
#else
#define fuse_new_spi(self, data) \
    ((fuse_spi_t *)fuse_new_spi_ex((self), (data), 0, 0))
#endif

/** @brief An opaque SPI object
 */
typedef struct spi_context fuse_spi_t;

/** @brief SPI data used to initialize the SPI interface
 */
typedef struct 
{
    uint8_t cs; ///< Chip select GPIO. Set to zero for no CS pin. Assumes active low.
    uint8_t sck; ///< Clock GPIO
    uint8_t tx; ///< Transmit GPIO
    uint8_t rx; ///< Receive GPIO
    uint32_t baudrate; ///< Requested baudrate
} fuse_spi_data_t;

/** @brief Initialize an SPI interface
 *
 * @param self The fuse application
 * @param data The SPI data used to initialize the SPI interface
 * @return The SPI interface, or NULL if the SPI interface could not be initialized
 */
fuse_spi_t *fuse_new_spi_ex(fuse_t *self,fuse_spi_data_t data, const char *file, const int line);

/** @brief Write a data block
 *
 * Write and wait for the SPI interface to complete the transfer.
 *
 * @param spi The spi instance
 * @param data The data to write
 * @param sz The number of bytes to write
 * @return True if successful
 */
bool fuse_spi_write(fuse_spi_t *spi, void* data, size_t sz);

/** @brief Read a data block
 *
 * Wait for the SPI interface to read data.
 *
 * @param spi The spi instance
 * @param data The data to read
 * @param sz The number of bytes to read
 * @return True if successful
 */
bool fuse_spi_read(fuse_spi_t *spi, void* data, size_t sz);

#endif
