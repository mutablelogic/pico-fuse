#include <pico/stdlib.h>
#include <fuse/fuse.h>
#include <rp2040/spi.h>
#include <hardware/spi.h>

///////////////////////////////////////////////////////////////////////////////

typedef struct
{
    spi_inst_t* instance;
} spi_t;

///////////////////////////////////////////////////////////////////////////////
// PRIVATE METHODS

/*
 * Initialize an SPI device
 */
static void rp2040_spi_init(fuse_t *fuse, fuse_driver_t *driver, fuse_event_t event, void *data)
{
    assert(fuse);
    assert(driver);
    assert(data);
    assert(event == EV_SPI_INIT);
    fuse_debugf(fuse, "rp2040_spi_init\n");
}

/*
 * Register SPI driver
 */
void *rp2040_spi_register(fuse_t *fuse, void *data)
{
    spi_t *self = fuse_malloc(fuse, sizeof(spi_t));
    if (self == NULL)
    {
        fuse_debugf(fuse, "rp2040_spi_register: failed to allocate memory for driver\n");
        return NULL;
    }

    // Register actions
    if (!fuse_register_action(fuse, EV_SPI_INIT, self, rp2040_spi_init))
    {
        return NULL;
    }

    // Return success
    return self;
}

///////////////////////////////////////////////////////////////////////////////

fuse_driver_params_t rp2040_spi = {
    .name = "rp2040_spi",
    .init = rp2040_spi_register,
    .events = {
        {.event = EV_SPI_INIT, .name = "EV_SPI_INIT"},
        {}}};
