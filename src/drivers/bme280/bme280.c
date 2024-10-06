#include <picofuse/picofuse.h>
#include "bme280.h"
#include "printf.h"

///////////////////////////////////////////////////////////////////////////////
// DECLARATIONS

static bool fuse_bme280_init(fuse_t *self, fuse_value_t *value, const void *user_data);
static void fuse_bme280_destroy(fuse_t *self, fuse_value_t *value);
static size_t fuse_bme280_qstr(fuse_t *self, char *buf, size_t sz, size_t i, fuse_value_t *v);
static void fuse_bme280_timer_callback(fuse_t *self, fuse_event_t *evt, void *user_data);

///////////////////////////////////////////////////////////////////////////////
// LIFECYCLE

/** @brief Register value type for BME280
 */
void fuse_register_bme280(fuse_t *self)
{
    assert(self);

    // Register BME280
    fuse_value_desc_t fuse_bme280_type = {
        .size = sizeof(struct bme280_context),
        .name = "BME280",
        .init = fuse_bme280_init,
        .destroy = fuse_bme280_destroy,
        .cstr = fuse_bme280_qstr,
        .qstr = fuse_bme280_qstr};
    fuse_register_value_type(self, FUSE_MAGIC_BME280, fuse_bme280_type);
}

/** @brief Create a new BME280 driver with SPI interface
 */
fuse_bme280_t *fuse_new_bme280_ex(fuse_t *self, fuse_spi_t *spi, const char *file, const int line)
{
    assert(self);
    assert(spi);

    // Register type
    if (!fuse_is_registered_value(self, FUSE_MAGIC_BME280))
    {
        fuse_register_bme280(self);
    }

    // Return a new BME280 context
    return (fuse_bme280_t *)fuse_new_value_ex(self, FUSE_MAGIC_BME280, spi, file, line);
}

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

/** @brief Measure temperature, humidity and pressure data once
 *
 * Measure the temperature, pressure and humidity, and put the results in the
 * BME280 instance. Returns a pointer to the measurement if successful.
 *
 * @param self The fuse application
 * @param bme280 The BME280 instance
 * @return The measurement if successful, otherwise NULL
 */
fuse_bme280_measurement_t* fuse_bme280_measure(fuse_t *self, fuse_bme280_t *bme280)
{
    assert(self);
    assert(bme280);

    // Read the raw temperature, pressure and humidity values
    int32_t temperature, pressure, humidity;
    fuse_debugf(self, "fuse_bme280_measure: %v\n", bme280);
    if (bme280_read_raw(self, bme280, &temperature, &pressure, &humidity) == false)
    {
        return NULL;
    }

    // Reset measurement data
    bme280->meas.temperature = 0;
    bme280->meas.pressure = 0;
    bme280->meas.humidity = 0;

    // If temperature not read, then return failure
    if (temperature == 0x80000)
    {
        fuse_debugf(self, "fuse_bme280_measure: temperature not read\n");
        return NULL;
    }

    // Compensate the temperature, pressure and humidity
    int32_t fine;
    int32_t tempC = bme280_compensate_temperature(bme280, temperature, &fine);
    bme280->meas.temperature = tempC / 100.0;
    if (pressure != 0x80000)
    {
        uint32_t pressureP = bme280_compensate_pressure(bme280, pressure, fine);
        bme280->meas.pressure = pressureP;
    }
    if (humidity != 0x8000)
    {
        uint32_t humidityH = bme280_compensate_humidity(bme280, humidity, fine);
        bme280->meas.humidity = humidityH / 1024.0;
    }

    // Return success
    return &(bme280->meas);
}

/** @brief Read the BME280 data
 *
 * Measure the temperature, pressure and humidity, and emit an event with the measurements
 *
 * @param self The fuse application
 * @param bme280 The BME280 instance
 * @param periodic_ms The periodic interval, in milliseconds, to emit a measurement event
 * @return True if the measurement was successful
 */
bool fuse_bme280_read(fuse_t *self, fuse_bme280_t *bme280, uint32_t periodic_ms)
{
    assert(self);
    assert(bme280);

    // If there is a timer, then cancel it
    if (bme280->timer != NULL)
    {
        fuse_debugf(self, "fuse_bme280_read: cancelling existing timer\n");
        fuse_timer_cancel(self, bme280->timer);
        bme280->timer = NULL;
    }

    // Create a new timer
    bme280->timer = fuse_timer_schedule(self, periodic_ms, periodic_ms > 0 ? true : false, bme280);
    if (bme280->timer == NULL)
    {
        fuse_debugf(self, "fuse_bme280_read: could not schedule a timer\n");
        return false;
    }

    // Return success
    return true;
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE METHODS

/** @brief Initialize a BME280 driver
 */
static bool fuse_bme280_init(fuse_t *self, fuse_value_t *value, const void *user_data)
{
    assert(self);
    assert(value);
    assert(user_data);

    // Retain the SPI interface
    // Read the chip id
    fuse_bme280_t *ctx = (fuse_bme280_t *)value;
    ctx->spi = (fuse_spi_t *)fuse_retain(self, (fuse_value_t *)user_data);
    ctx->timer = NULL;
    ctx->id = bme280_read_chip_id(self, ctx);
    if (ctx->id != BME280_CHIP_ID)
    {
        fuse_debugf(self, "fuse_bme280_init: invalid chip ID 0x%X\n", ctx->id);
        return false;
    }

    // Reset the BME280
    bme280_reset(self, ctx);

    // Wait for status measuring and updating to complete
    bool measuring, updating;
    uint8_t tries = 0;
    do
    {
        sleep_ms(2);
        if (++tries > 3)
        {
            break;
        }
        bme280_read_status(self, ctx, &measuring, &updating);
    } while (measuring || updating);

    if (tries > 3)
    {
        fuse_debugf(self, "fuse_bme280_init: failed to reset\n");
        return false;
    }

    // Set timer handler - on core 0
    fuse_register_callback(self, FUSE_EVENT_TIMER, 0, fuse_bme280_timer_callback);

    // Read the compensation parameters
    bme280_read_compensation_parameters(self, ctx);

    // Oversampling T=2, P=16, H=1
    bme280_write_osrs(self, ctx, 2, 16, 1);

    // Return success
    return true;
}

/** @brief Release the BME280 driver
 */
static void fuse_bme280_destroy(fuse_t *self, fuse_value_t *value)
{
    assert(self);
    assert(value);

    // Get the BME280 context
    fuse_bme280_t *ctx = (fuse_bme280_t *)value;

    // If there is a timer, then cancel it
    if (ctx->timer != NULL)
    {
        fuse_timer_cancel(self, ctx->timer);
    }

    // Release SPI
    fuse_release(self, ctx->spi);
}

/** @brief Callback when there is timer to do a measurement
 */
static void fuse_bme280_timer_callback(fuse_t *self, fuse_event_t *evt, void *user_data)
{
    assert(self);
    assert(evt);

    // Get the BME280 context
    if (fuse_value_type(self, user_data) != FUSE_MAGIC_BME280)
    {
        return;
    }

    // Get the BME280 context
    fuse_bme280_t *ctx = (fuse_bme280_t *)user_data;

    // Measure the temperature, pressure and humidity
    fuse_bme280_measurement_t* measurement = fuse_bme280_measure(self, ctx);
    if (measurement == NULL)
    {
        fuse_debugf(self, "fuse_bme280_timer_callback: failed to measure\n");
        return;
    }

    // Create the event with the measurement as the user_data
    assert(fuse_new_event(self, (fuse_value_t *)ctx, FUSE_EVENT_BME280, measurement));
}

/** @brief Append a JSON representation of the BME driver
 */
static size_t fuse_bme280_qstr(fuse_t *self, char *buf, size_t sz, size_t i, fuse_value_t *v)
{
    assert(self);
    assert(buf == NULL || sz > 0);
    assert(v);

    // Add prefix
    i = chtostr_internal(buf, sz, i, '{');

    // Add chip-id number
    fuse_bme280_t *bme280 = (fuse_bme280_t *)v;
    i = qstrtostr_internal(buf, sz, i, "id");
    i = chtostr_internal(buf, sz, i, ':');
    i = utostr_internal(buf, sz, i, bme280->id, 0);
    i = chtostr_internal(buf, sz, i, ',');

    // Add spi
    i = qstrtostr_internal(buf, sz, i, "spi");
    i = chtostr_internal(buf, sz, i, ':');
    i = vtostr_internal(self, buf, sz, i, (fuse_value_t *)bme280->spi, true);
    i = chtostr_internal(buf, sz, i, ',');

#ifdef DEBUG
    // Output compensation parameters - for debug only
    i = qstrtostr_internal(buf, sz, i, "T1");
    i = chtostr_internal(buf, sz, i, ':');
    i = utostr_internal(buf, sz, i, bme280->T1, 0);
    i = chtostr_internal(buf, sz, i, ',');

    i = qstrtostr_internal(buf, sz, i, "T2");
    i = chtostr_internal(buf, sz, i, ':');
    i = itostr_internal(buf, sz, i, bme280->T2, 0);
    i = chtostr_internal(buf, sz, i, ',');

    i = qstrtostr_internal(buf, sz, i, "T3");
    i = chtostr_internal(buf, sz, i, ':');
    i = itostr_internal(buf, sz, i, bme280->T3, 0);
    i = chtostr_internal(buf, sz, i, ',');

    i = qstrtostr_internal(buf, sz, i, "P1");
    i = chtostr_internal(buf, sz, i, ':');
    i = utostr_internal(buf, sz, i, bme280->P1, 0);
    i = chtostr_internal(buf, sz, i, ',');

    i = qstrtostr_internal(buf, sz, i, "P2");
    i = chtostr_internal(buf, sz, i, ':');
    i = itostr_internal(buf, sz, i, bme280->P2, 0);
    i = chtostr_internal(buf, sz, i, ',');

    i = qstrtostr_internal(buf, sz, i, "P3");
    i = chtostr_internal(buf, sz, i, ':');
    i = itostr_internal(buf, sz, i, bme280->P3, 0);
    i = chtostr_internal(buf, sz, i, ',');

    i = qstrtostr_internal(buf, sz, i, "P4");
    i = chtostr_internal(buf, sz, i, ':');
    i = itostr_internal(buf, sz, i, bme280->P4, 0);
    i = chtostr_internal(buf, sz, i, ',');

    i = qstrtostr_internal(buf, sz, i, "P5");
    i = chtostr_internal(buf, sz, i, ':');
    i = itostr_internal(buf, sz, i, bme280->P5, 0);
    i = chtostr_internal(buf, sz, i, ',');

    i = qstrtostr_internal(buf, sz, i, "P6");
    i = chtostr_internal(buf, sz, i, ':');
    i = itostr_internal(buf, sz, i, bme280->P6, 0);
    i = chtostr_internal(buf, sz, i, ',');

    i = qstrtostr_internal(buf, sz, i, "P7");
    i = chtostr_internal(buf, sz, i, ':');
    i = itostr_internal(buf, sz, i, bme280->P7, 0);
    i = chtostr_internal(buf, sz, i, ',');

    i = qstrtostr_internal(buf, sz, i, "P8");
    i = chtostr_internal(buf, sz, i, ':');
    i = itostr_internal(buf, sz, i, bme280->P8, 0);
    i = chtostr_internal(buf, sz, i, ',');

    i = qstrtostr_internal(buf, sz, i, "P9");
    i = chtostr_internal(buf, sz, i, ':');
    i = itostr_internal(buf, sz, i, bme280->P9, 0);
    i = chtostr_internal(buf, sz, i, ',');

    i = qstrtostr_internal(buf, sz, i, "H1");
    i = chtostr_internal(buf, sz, i, ':');
    i = utostr_internal(buf, sz, i, bme280->H1, 0);
    i = chtostr_internal(buf, sz, i, ',');

    i = qstrtostr_internal(buf, sz, i, "H2");
    i = chtostr_internal(buf, sz, i, ':');
    i = itostr_internal(buf, sz, i, bme280->H2, 0);
    i = chtostr_internal(buf, sz, i, ',');

    i = qstrtostr_internal(buf, sz, i, "H3");
    i = chtostr_internal(buf, sz, i, ':');
    i = utostr_internal(buf, sz, i, bme280->H3, 0);
    i = chtostr_internal(buf, sz, i, ',');

    i = qstrtostr_internal(buf, sz, i, "H4");
    i = chtostr_internal(buf, sz, i, ':');
    i = itostr_internal(buf, sz, i, bme280->H4, 0);
    i = chtostr_internal(buf, sz, i, ',');

    i = qstrtostr_internal(buf, sz, i, "H5");
    i = chtostr_internal(buf, sz, i, ':');
    i = itostr_internal(buf, sz, i, bme280->H5, 0);
    i = chtostr_internal(buf, sz, i, ',');

    i = qstrtostr_internal(buf, sz, i, "H6");
    i = chtostr_internal(buf, sz, i, ':');
    i = itostr_internal(buf, sz, i, bme280->H6, 0);
    i = chtostr_internal(buf, sz, i, ',');
#endif

    // Output mode
    i = qstrtostr_internal(buf, sz, i, "mode");
    i = chtostr_internal(buf, sz, i, ':');
    i = utostr_internal(buf, sz, i, bme280_read_mode(self, bme280), 0);

    // Add suffix
    i = chtostr_internal(buf, sz, i, '}');

    // Return new index
    return i;
}
