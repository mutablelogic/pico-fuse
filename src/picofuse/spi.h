/** @file spi.h
 *  @brief Private function prototypes and structure definitions for SPI interface.
 */
#ifndef FUSE_PRIVATE_SPI_H
#define FUSE_PRIVATE_SPI_H

#include <picofuse/picofuse.h>
#include <hardware/spi.h>

/** @brief SPI context
 */
struct spi_context
{
    fuse_gpio_t* cs; ///< Chip select GPIO
    fuse_gpio_t* ck; ///< Clock GPIO
    fuse_gpio_t* tx; ///< Transmit GPIO
    fuse_gpio_t* rx; ///< Receive GPIO
    spi_inst_t *inst; ///< SPI instance
        uint16_t xfr_delayms; ///< Pause after a write or read in milliseconds
};

/** @brief Register type for SPI
 */
void fuse_register_value_spi(fuse_t *self);

#endif
