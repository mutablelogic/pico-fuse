#include <picofuse/picofuse.h>
#include <hardware/spi.h>
#include "spi.h"
#include "fuse.h"
#include "printf.h"

///////////////////////////////////////////////////////////////////////////////
// DECLARATIONS

/** @brief Initialize a SPI interface
 */
static bool fuse_spi_init(fuse_t *self, fuse_value_t *value, const void *user_data);

/** @brief Destroy the SPI interface
 */
static void fuse_spi_destroy(fuse_t *self, fuse_value_t *value);

/** @brief Append a string representation of an SPI interface
 */
static size_t fuse_spi_cstr(fuse_t *self, char *buf, size_t sz, size_t i, fuse_value_t *v);

/** @brief Append a JSON representation of an SPI interface
 */
static size_t fuse_spi_qstr(fuse_t *self, char *buf, size_t sz, size_t i, fuse_value_t *v);

///////////////////////////////////////////////////////////////////////////////
// LIFECYCLE

/** @brief Register type for SPI
 */
void fuse_register_value_spi(fuse_t *self)
{
    assert(self);

    // Register SPI type
    fuse_value_desc_t fuse_spi_type = {
        .size = sizeof(struct spi_context),
        .name = "SPI",
        .init = fuse_spi_init,
        .destroy = fuse_spi_destroy,
        .cstr = fuse_spi_cstr,
        .qstr = fuse_spi_qstr 
    };
    fuse_register_value_type(self, FUSE_MAGIC_SPI, fuse_spi_type);
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE METHODS

/** @brief Initialize a SPI interface
 */
static bool fuse_spi_init(fuse_t *self, fuse_value_t *value, const void *user_data) {
    // TODO
}

/** @brief Destroy the SPI interface
 */
static void fuse_spi_destroy(fuse_t *self, fuse_value_t *value) {
    // TODO

}

/** @brief Append a string representation of an SPI interface
 */
static size_t fuse_spi_cstr(fuse_t *self, char *buf, size_t sz, size_t i, fuse_value_t *v) {
    // TODO

}

/** @brief Append a JSON representation of an SPI interface
 */
static size_t fuse_spi_qstr(fuse_t *self, char *buf, size_t sz, size_t i, fuse_value_t *v) {
    // TODO

}

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

/** @brief Initialize an SPI interface
 */
fuse_spi_t *fuse_new_spi_ex(fuse_t *self,fuse_spi_data_t data, const char *file, const int line) {
    // TODO
}

/** @brief Write a data block
 */
bool fuse_spi_write(fuse_spi_t *spi, void* data, size_t sz) {
    // TODO
}

/** @brief Read a data block
 */
bool fuse_spi_read(fuse_spi_t *spi, void* data, size_t sz) {
    // TODO
}
