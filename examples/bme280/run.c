#include <picofuse/picofuse.h>

#define GPIO_SPI_SCK 2 // SCK = GPIO2, Physical pin 4
#define GPIO_SPI_TX  3 // TX  = GPIO3, Physical pin 5
#define GPIO_SPI_RX  4 // RX  = GPIO4, Physical pin 6
#define GPIO_SPI_CS  5 // CS  = GPIO5, Physical pin 7


#define READ_BIT 0x80

static void read_registers(fuse_spi_t* spi, uint8_t reg, uint8_t *buf, size_t len) {
    reg |= READ_BIT;
    
    cs_select();
    spi_write_blocking(spi_default, &reg, 1);
    sleep_ms(10);
    spi_read_blocking(spi_default, 0, buf, len);
    cs_deselect();
    sleep_ms(10);
}

int run(fuse_t *self)
{
    // Initialize picofuse
    picofuse_init(self);

    // Bind SPI
     // This example will use SPI0 at 0.5MHz.    
    fuse_spi_data_t config = {
        .cs = GPIO_SPI_CS,
        .ck = GPIO_SPI_SCK,
        .tx = GPIO_SPI_TX,
        .rx = GPIO_SPI_RX,
        .baudrate = 500 * 1000,
    };
    fuse_spi_t* spi = (fuse_spi_t* )fuse_retain(self, fuse_new_spi(self, config));
    assert(spi);

    // Print SPI instance
    fuse_printf(self, "SPI initialized: %v\n",spi);

    return 0;
}
