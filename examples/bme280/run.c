#include <picofuse/picofuse.h>

#define GPIO_SPI_SCK 2 // SCK = GPIO2, Physical pin 4
#define GPIO_SPI_TX  3 // TX  = GPIO3, Physical pin 5
#define GPIO_SPI_RX  4 // RX  = GPIO4, Physical pin 6
#define GPIO_SPI_CS  5 // CS  = GPIO5, Physical pin 7

#define READ_BIT 0x80

static void read_register(fuse_t* self, fuse_spi_t* spi, uint8_t reg, uint8_t *buf, size_t len) {
    assert(spi);
    assert(buf);
    assert(len);

    buf[0] = reg | READ_BIT;
    assert(fuse_spi_xfr(self, spi, buf, sizeof(uint8_t), len, true));
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
        .xfr_delayms = 10,
    };
    fuse_spi_t* spi = (fuse_spi_t* )fuse_retain(self, fuse_new_spi(self, config));
    assert(spi);

    // Print SPI instance
    fuse_printf(self, "SPI initialized: %v\n",spi);

    // See if SPI is working - interrograte the device for its I2C ID number, should be 0x60
    uint8_t id;
    read_register(self, spi, 0xD0, &id, sizeof(uint8_t));
    fuse_printf(self, "Chip ID is 0x%x\n", id);

    return 0;
}
