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
};

#endif
