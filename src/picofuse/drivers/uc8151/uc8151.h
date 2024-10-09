/** @file uc8151.h
 *  @brief Private function prototypes and structure definitions for UC8151 e-ink display driver
 */
#ifndef FUSE_PRIVATE_UC8151_H
#define FUSE_PRIVATE_UC8151_H

#include <picofuse/picofuse.h>
#include <stdint.h>

/** @brief UC8151 context
 */
struct uc8151_context
{
    fuse_spi_t *spi;                ///< SPI interface
    fuse_gpio_t *dc;                ///< Data/Command GPIO
    fuse_gpio_t *reset;             ///< Reset GPIO (can be NULL)
    fuse_gpio_t *busy;              ///< Busy GPIO (can be NULL)
};

/** @brief Read data from the UC8151
 * 
 * If reg is not 0xFF, then read data from a specific register. Otherwise, read data only.
 * On return, the data buffer will contain the data read.
 * 
 * @param self The fuse instance
 * @param ctx The UC8151 context
 * @param reg The command to send (or 0 for data)
 * @param data The data buffer
 * @param sz The length of the data buffer to read
 * @return true if successful
 */
bool fuse_uc8151_read(fuse_t *self, fuse_uc8151_t *ctx, uint8_t reg, uint8_t *data, size_t sz);

/** @brief Write data to the UC8151
 *
 * If reg is not 0xFF, then write data for a specific register. Otherwise, write data only.
 * 
 * @param self The fuse instance
 * @param ctx The UC8151 context
 * @param reg The register to write (or 0xFF for data only)
 * @param data The data buffer
 * @param sz The length of the data buffer to write
 * @return true if successful
 */
bool fuse_uc8151_write(fuse_t *self, fuse_uc8151_t *ctx, uint8_t reg, uint8_t *data, size_t sz);


/** @brief Reset the UC8151 and wait for it to become ready
 * 
 * @param self The fuse instance
 * @param ctx The UC8151 context
 * @return true if successful, false if reset is not implemented or there is a busy timeout
 */
bool fuse_uc8151_reset(fuse_t *self, fuse_uc8151_t *ctx);


/** @brief Wait for the UC8151 busy pin to indicate an action has been completed
 * 
 * @param self The fuse instance
 * @param ctx The UC8151 context
 * @param timeout The expected busy timeout in milliseconds. If zero then busy state is returned immediately.
 * @return true if an action has been completed, false if busy is not implemented or there is a timeout
 */
bool fuse_uc8151_busywait(fuse_t *self, fuse_uc8151_t *ctx, uint16_t timeout);

/** @brief Read LUT_REV from the UC8151
 * 
 * @param self The fuse instance
 * @param ctx The UC8151 context
 * @return revision or 0 on error
 */
uint8_t fuse_uc8151_read_revision(fuse_t *self, fuse_uc8151_t *ctx);

/** @brief Refresh the display (DRF)
 * 
 * @param self The fuse instance
 * @param ctx The UC8151 context
 * @return true if successful
 */
bool fuse_uc8151_refresh(fuse_t *self, fuse_uc8151_t *ctx);

#endif
