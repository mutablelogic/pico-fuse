#include "uc8151.h"

///////////////////////////////////////////////////////////////////////////////
// PRIVATE METHODS

/** @brief Return the busy state of the UC8151
 * 
 * @param self The fuse instance
 * @param ctx The UC8151 context
 * @return the value of the busy pin, or false if busy pin is not implemented
 */
static inline bool fuse_uc8151_busy(fuse_t *self, fuse_uc8151_t *ctx) {
    assert(self);
    assert(ctx);

    if (ctx->busy) {
        return fuse_gpio_get(ctx->busy);
    } else {
        return false;
    }
}


///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

/** @brief Reset the UC8151 and wait for it to become ready
 * 
 * @param self The fuse instance
 * @param ctx The UC8151 context
 * @return true if successful, false if reset is not implemented or there is a busy timeout
 */
bool fuse_uc8151_reset(fuse_t *self, fuse_uc8151_t *ctx) {
    assert(self);
    assert(ctx);

    if (ctx->reset) {
        fuse_gpio_set(ctx->reset, false);
        sleep_ms(10);
        fuse_gpio_set(ctx->reset, true);
        return fuse_uc8151_busywait(self, ctx, 10);
    } else {
        return false;
    }
}

/** @brief Wait for the UC8151 busy pin to become ready
 * 
 * @param self The fuse instance
 * @param ctx The UC8151 context
 * @param timeout The expected busy timeout in milliseconds
 * @return true if the device is ready, false if busy is not implemented or there is a busy timeout
 */
bool fuse_uc8151_busywait(fuse_t *self, fuse_uc8151_t *ctx, uint16_t timeout) {
    assert(self);
    assert(ctx);

    if (!ctx->busy) {
        return false;
    }

    do {
        if (!fuse_uc8151_busy(self, ctx)) {
            return true;
        }
        sleep_ms(1);
    } while(timeout-- > 0);

    // Return failure
    fuse_debugf(self, "fuse_uc8151_busywait: busy timeout\n");
    return false;
}
