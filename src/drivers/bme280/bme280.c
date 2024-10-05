#include <picofuse/picofuse.h>
#include "bme280.h"
#include "printf.h"

///////////////////////////////////////////////////////////////////////////////
// DECLARATIONS

static bool fuse_bme280_init(fuse_t *self, fuse_value_t *value, const void *user_data);
static void fuse_bme280_destroy(fuse_t *self, fuse_value_t *value);
static size_t fuse_bme280_qstr(fuse_t *self, char *buf, size_t sz, size_t i, fuse_value_t *v);

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
        .qstr = fuse_bme280_qstr         
    };
    fuse_register_value_type(self, FUSE_MAGIC_BME280, fuse_bme280_type);
}

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

/** @brief Create a new BME280 driver with SPI interface
 */
fuse_bme280_t *fuse_new_bme280_ex(fuse_t *self, fuse_spi_t* spi, const char *file, const int line)
{
    assert(self);
    assert(spi);

    // Register type
    if (!fuse_is_registered_value(self, FUSE_MAGIC_BME280)) {
        fuse_register_bme280(self);
    }

    // Create a new BME280 context
    fuse_bme280_t *ctx = (fuse_bme280_t *)fuse_new_value_ex(self, FUSE_MAGIC_BME280, spi, file, line);
    if (ctx == NULL)
    {
        return NULL;
    }

    // Return the BME280 context
    return ctx;
}


/** @brief Read the temperature, pressure and humidity from the BME280
 */
void fuse_bme280_read(fuse_t* self, fuse_bme280_t* bme280)
{
    assert(self);
    assert(bme280);

    // Read the temperature, pressure and humidity
    int32_t temperature, pressure, humidity;
    bme280_read_raw(self, bme280, &temperature, &pressure, &humidity);
    fuse_debugf(self, "fuse_bme280_read: T=%X P=%X H=%X\n", temperature, pressure, humidity);

    int32_t fine; 
    int32_t tempC = bme280_compensate_temperature(bme280, temperature, &fine);
    fuse_debugf(self, "fuse_bme280_read: T=%d\n", tempC);
    if (pressure != 0x80000) {
        uint32_t pressureP = bme280_compensate_pressure(bme280, pressure, fine);
        fuse_debugf(self, "fuse_bme280_read: P=%d\n",pressureP);
    }
    if (humidity != 0x8000) {
        uint32_t humidityH = bme280_compensate_humidity(bme280, humidity, fine);
        fuse_debugf(self, "fuse_bme280_read: H=%d\n",humidityH / 1024);
    }
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
    fuse_bme280_t *ctx = (fuse_bme280_t *)value;
    ctx->spi = (fuse_spi_t* )fuse_retain(self, (fuse_value_t* )user_data);

    // Read the chip id
    ctx->id = bme280_read_chip_id(self, ctx);
    if (ctx->id != BME280_CHIP_ID)
    {
        fuse_debugf(self, "fuse_bme280_init: invalid chip ID 0x%X\n", ctx->id);
    }

    // Read the compensation parameters
    bme280_read_compensation_parameters(self, ctx);

    // No oversampling
    bme280_write_osrs(self, ctx, 1, 1, 1);

    // Return success
    return true;
}

/** @brief Release the BME280 driver
 */
static void fuse_bme280_destroy(fuse_t *self, fuse_value_t *value)
{
    assert(self);
    assert(value);

    // Release the SPI
    fuse_release(self, ((fuse_bme280_t *)value)->spi);
}


/** @brief Append a JSON representation of the BME driver
 */
static size_t fuse_bme280_qstr(fuse_t *self, char *buf, size_t sz, size_t i, fuse_value_t *v) {
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
    i = vtostr_internal(self, buf, sz, i, (fuse_value_t* )bme280->spi, true);
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
    i = utostr_internal(buf, sz, i, bme280_read_mode(self,bme280), 0);

    // Add suffix
    i = chtostr_internal(buf, sz, i, '}');

    // Return new index
    return i;
}
