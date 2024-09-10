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
    uint8_t ck; ///< Clock GPIO
    uint8_t tx; ///< Transmit GPIO
    uint8_t rx; ///< Receive GPIO
    uint32_t baudrate; ///< Requested baudrate
    uint16_t xfr_delayms; ///< Pause after a write or read in milliseconds
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
 * Write data to an SPI slave device. If the blocking value is set to true, then the function will wait for the 
 * data to be written. Otherwise, DMA transfer is used and an event FUSE_EVENT_SPI_TX is generated when the 
 * transfer is complete.
 *
 * @param self The fuse application
 * @param spi The spi instance
 * @param data The data to write
 * @param sz The number of bytes to write
 * @param blocking True if the write should be blocking
 * @return True if successful
 */
bool fuse_spi_write(fuse_t *self, fuse_spi_t *spi, void* data, size_t sz, bool blocking);

/** @brief Read a data block
 *
 * Read data from an SPI slave device. If the blocking value is set to true, then the function will wait for the 
 * data to be read. Otherwise, DMA transfer is used and an event FUSE_EVENT_SPI_RX is generated when the 
 * transfer is complete.
 *
 * @param spi The spi instance
 * @param data The data to read
 * @param sz The number of bytes to read
 * @return True if successful
 */
bool fuse_spi_read(fuse_t *self, fuse_spi_t *spi, void* data, size_t sz, bool blocking);

/** @brief Transfer data to and from an SPI slave device
 *
 * Write data to an SPI slave device, pause, and then read. If the blocking value is set to true,
 * then the function will wait for the data to be read. Otherwise, DMA transfer is used and an event 
 * FUSE_EVENT_SPI_RX is generated when the read transfer is complete.
 *
 * @param self The fuse application
 * @param spi The spi instance
 * @param data The data to read/write
 * @param wsz The number of bytes to write
 * @param rsz The number of bytes to read
 * @param blocking True if the write should be blocking
 * @return True if successful
 */
bool fuse_spi_xfr(fuse_t *self, fuse_spi_t *spi, void* data, size_t wsz, size_t rsz, bool blocking);

#endif
