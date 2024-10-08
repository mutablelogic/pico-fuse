#include <picofuse/picofuse.h>
#include "uc8151.h"
#include "printf.h"

///////////////////////////////////////////////////////////////////////////////
// DECLARATIONS

static void fuse_uc8151_register(fuse_t *self);
static bool fuse_uc8151_init(fuse_t *self, fuse_value_t *value, const void *user_data);
static void fuse_uc8151_destroy(fuse_t *self, fuse_value_t *value);
static size_t fuse_uc8151_str(fuse_t *self, char *buf, size_t sz, size_t i, fuse_value_t *v, bool json);

///////////////////////////////////////////////////////////////////////////////
// LIFECYCLE

/** @brief Register value type for UC8151
 */
static void fuse_uc8151_register(fuse_t *self)
{
    assert(self);

    // Register UC8151
    fuse_value_desc_t fuse_uc8151_type = {
        .size = sizeof(struct uc8151_context),
        .name = "UC8151",
        .init = fuse_uc8151_init,
        .destroy = fuse_uc8151_destroy,
        .str = fuse_uc8151_str
    };
    fuse_register_value_type(self, FUSE_MAGIC_UC8151, fuse_uc8151_type);
}

/** @brief Create a new UC8151 driver with SPI interface
 */
fuse_uc8151_t *fuse_new_uc8151_ex(fuse_t *self, fuse_spi_t *spi, const char *file, const int line)
{
    assert(self);
    assert(spi);

    // Register type
    if (!fuse_is_registered_value(self, FUSE_MAGIC_UC8151))
    {
        fuse_uc8151_register(self);
    }

    // Return a new UC8151 context
    return (fuse_uc8151_t *)fuse_new_value_ex(self, FUSE_MAGIC_UC8151, spi, file, line);
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE METHODS

/** @brief Initialize a new UC8151 instance
 */
static bool fuse_uc8151_init(fuse_t *self, fuse_value_t *value, const void *user_data)
{
    assert(self);
    assert(value);
    assert(user_data);

    // TODO
    return false;
}

/** @brief Release the UC8151 instance
 */
static void fuse_uc8151_destroy(fuse_t *self, fuse_value_t *value)
{
    assert(self);
    assert(value);

    // TODO
}

/** @brief Append a JSON representation of the UC8151 instance
 */
static size_t fuse_uc8151_str(fuse_t *self, char *buf, size_t sz, size_t i, fuse_value_t *v, bool json)
{
    assert(self);
    assert(buf == NULL || sz > 0);
    assert(v);

    // Add prefix
    i = chtostr_internal(buf, sz, i, '{');

    // TODO

    // Add suffix
    i = chtostr_internal(buf, sz, i, '}');

    // Return the index
    return i;
}

