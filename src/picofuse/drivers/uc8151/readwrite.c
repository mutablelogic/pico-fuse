#include "uc8151.h"

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

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
bool fuse_uc8151_read(fuse_t *self, fuse_uc8151_t *ctx, uint8_t reg, uint8_t *data, size_t sz) {
    assert(self);
    assert(ctx);
    assert(data || sz == 0);
    assert(ctx->spi);
    assert(ctx->dc);

    // Write register
    if(reg != 0xFF) {
        fuse_gpio_set(ctx->dc, false);
        if(!fuse_spi_xfr(self, ctx->spi, &reg, sizeof(uint8_t), 0, true)) {
            fuse_debugf(self, "fuse_uc8151_read: failed to write register");
            return false;
        }
    }

    // Read data
    if (sz > 0) {
        fuse_gpio_set(ctx->dc, true);
        if(!fuse_spi_xfr(self, ctx->spi, data, 0, sz, true)) {
            fuse_debugf(self, "fuse_uc8151_read: failed to read data");
            return false;
        }
    }

    // Return true
    return true;
}

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
bool fuse_uc8151_write(fuse_t *self, fuse_uc8151_t *ctx, uint8_t reg, uint8_t *data, size_t sz) {
    assert(self);
    assert(ctx);
    assert(data);
    assert(sz > 0);

    return false;
}
