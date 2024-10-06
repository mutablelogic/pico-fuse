#include <picofuse/picofuse.h>

#define GPIO_SPI_SCK 2 // SCK = GPIO2, Physical pin 4
#define GPIO_SPI_TX  3 // TX  = GPIO3, Physical pin 5
#define GPIO_SPI_RX  4 // RX  = GPIO4, Physical pin 6
#define GPIO_SPI_CS  5 // CS  = GPIO5, Physical pin 7


/* @brief Callback when there is a measurement from the BME280
 */
void bme280_callback(fuse_t *self, fuse_event_t *evt, void* user_data)
{
    assert(self);
    assert(evt);
    assert(user_data);

    // Print the measurement
    fuse_bme280_measurement_t* meas = (fuse_bme280_measurement_t*)user_data;
    fuse_printf(self, "Temp: %f\n",meas->temperature);
    if (meas->pressure) {
        fuse_printf(self, "Pressure: %f\n",meas->pressure);
    }
    if (meas->humidity) {
        fuse_printf(self, "Humidity: %f\n",meas->humidity);
    }
}

int run(fuse_t *self)
{
    // Initialize picofuse
    picofuse_init(self);

    // Create an SPI interface
    fuse_spi_data_t config = {
        .cs = GPIO_SPI_CS,
        .ck = GPIO_SPI_SCK,
        .tx = GPIO_SPI_TX,
        .rx = GPIO_SPI_RX,
        .baudrate = 500 * 1000, // This example will use SPI0 at 0.5MHz   
        .xfr_delayms = 10,
    };

    // Create a new BME280
    fuse_bme280_t* bme280 = (fuse_bme280_t* )fuse_retain(self, fuse_new_bme280(self, fuse_new_spi(self, config)));
    assert(bme280);

    // Print driver details
    fuse_printf(self, "bme280 initialized: %v\n", bme280);

    // Register a callback on core 0 for the measured data
    assert(fuse_register_callback(self, FUSE_EVENT_BME280, 0, bme280_callback));

    // Read measurements once per minute
    fuse_bme280_read(self, bme280, 60 * 1000);

    return 0;
}
