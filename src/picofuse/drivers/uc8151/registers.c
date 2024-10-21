#include "uc8151.h"

#define UC8151_REG_REV 0x70 // Revision
#define UC8151_REG_DRF 0x12 // Display refresh
#define UC8151_REG_POF 0x02 // Power off
#define UC8151_REG_PON 0x04 // Power on

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

/** @brief Read LUT_REV from the UC8151
 * 
 * @param self The fuse instance
 * @param ctx The UC8151 context
 * @return revision or 0 on error
 */
uint8_t fuse_uc8151_read_revision(fuse_t *self, fuse_uc8151_t *ctx) {
    assert(self);
    assert(ctx);

    uint8_t rev;
    if(!fuse_uc8151_read(self, ctx, UC8151_REG_REV, &rev, sizeof(uint8_t))) {
        fuse_debugf(self, "fuse_uc8151_read_revision: LUT_REV failed");
        return 0;
    }

    // Return revision
    return rev;
}

/** @brief Refresh the display (DRF)
 * 
 * @param self The fuse instance
 * @param ctx The UC8151 context
 * @return true if successful
 */
bool fuse_uc8151_refresh(fuse_t *self, fuse_uc8151_t *ctx) {
    assert(self);
    assert(ctx);

    if(!fuse_uc8151_read(self, ctx, UC8151_REG_DRF, NULL, 0)) {
        fuse_debugf(self, "fuse_uc8151_refresh: DRF failed");
        return 0;
    }

    // Return busy wait
    return fuse_uc8151_busywait(self, ctx, 1000);
}

/** @brief Power on/off (PON/POF)
 * 
 * @param self The fuse instance
 * @param ctx The UC8151 context
 * @param on Power on (true) or off (false)
 * @return true if successful
 */
bool fuse_uc8151_power(fuse_t *self, fuse_uc8151_t *ctx, bool on) {
    assert(self);
    assert(ctx);

    if(!fuse_uc8151_read(self, ctx, on ? UC8151_REG_PON : UC8151_REG_POF, NULL, 0)) {
        fuse_debugf(self, "fuse_uc8151_power: PON/POF failed");
        return 0;
    }

    // Return busy wait
    return fuse_uc8151_busywait(self, ctx, 1000);
}
