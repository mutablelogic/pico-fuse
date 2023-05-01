#ifndef RP2040_SPI_H
#define RP2040_SPI_H

#include <fuse/fuse.h>
#include <stdint.h>

/*
 * Events
 */
#define EV_SPI_INIT (fuse_event_t)(0x0401)

/*
 * SPI configuration
 */
typedef struct
{
    int instance;      // SPI instance (0 or 1)
    uint32_t baudrate; // SPI baud rate
    bool master;       // Master mode if true, slave mode if false
    bool cpha;         // Clock phase
    bool cpol;         // Clock polarity
    bool lsb_first;    // LSB first if true, MSB first if false
} pwm_t;

/*
 * Define the SPI driver
 */
extern fuse_driver_params_t rp2040_spi;

#endif
