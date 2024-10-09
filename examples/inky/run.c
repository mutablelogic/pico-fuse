#include <picofuse/picofuse.h>

/* @brief Callback when there is a measurement from the BME280
 */
void bme280_callback(fuse_t *self, fuse_event_t *evt, void *user_data)
{
    assert(self);
    assert(evt);
    assert(user_data);

    // Print the measurement
    fuse_bme280_measurement_t *meas = (fuse_bme280_measurement_t *)user_data;
    fuse_printf(self, "Temp: %f\n", meas->temperature);
    if (meas->pressure)
    {
        fuse_printf(self, "Pressure: %f\n", meas->pressure);
    }
    if (meas->humidity)
    {
        fuse_printf(self, "Humidity: %f\n", meas->humidity);
    }
}

/* @brief Callback for printing out memory stats
 */
void stats_callback(fuse_t *self, fuse_event_t *evt, void *user_data)
{
    assert(self);
    assert(evt);

    // user_data should be NULL
    if (user_data)
    {
        return;
    }

    // Print the stats
    size_t cur, max;
    fuse_memstats(self, &cur, &max);

    // Print the stats
    fuse_printf(self, "Memory: %u bytes/%u bytes\n", cur, max);
}

int run(fuse_t *self)
{
    // Initialize picofuse
    picofuse_init(self);

    // Create an SPI interface
    fuse_spi_data_t config = {
        .rx = 16,
        .cs = 17,
        .ck = 18,
        .tx = 19,
        .baudrate = 5 * 1000 * 1000,
        .xfr_delayms = 10,
    };
    const uint8_t dc = 20;
    const uint8_t reset = 21;
    const uint8_t busy = 26;
    const uint8_t swa = 12;
    const uint8_t swb = 13;
    const uint16_t swc = 14;
    const uint16_t width = 296;
    const uint16_t height = 128;

    // Create a new UC8151
    fuse_uc8151_config_t uc8151_config = {
        .spi = fuse_new_spi(self, config),
        .dc = 20,
        .reset = 21,
        .busy = 26,
        .width = 296,
        .height = 128
    };
    fuse_uc8151_t *uc8151 = fuse_new_uc8151(self, uc8151_config);
    assert(uc8151);

    fuse_debugf(self, "uc8151 initialised: %v\n", uc8151);

    return 0;
}
