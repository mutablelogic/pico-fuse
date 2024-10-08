#include <picofuse/picofuse.h>
#include <hardware/spi.h>
#include "fuse.h"
#include "gpio.h"
#include "printf.h"
#include "spi.h"

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
static size_t fuse_spi_str(fuse_t *self, char *buf, size_t sz, size_t i, fuse_value_t *v, bool json);

/** @brief Return SPI instance from GPIO pins, or NULL if invalid pin combination
 */
static spi_inst_t * fuse_spi_gpio_to_instance(fuse_spi_data_t *data);

/** Drive CS
 * @param spi SPI context
 * @param select true to select (drive low), false to deselect (drive high)
 */
static void fuse_spi_cs(fuse_spi_t *spi, bool select);

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
        .str = fuse_spi_str,
    };
    fuse_register_value_type(self, FUSE_MAGIC_SPI, fuse_spi_type);
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE METHODS

/** @brief Initialize a SPI interface
 */
static bool fuse_spi_init(fuse_t *self, fuse_value_t *value, const void *user_data) {
    assert(self);
    assert(value);
    assert(user_data);

    // Check parameters
    fuse_spi_t *spi = (fuse_spi_t *)value;
    fuse_spi_data_t *data = (fuse_spi_data_t *)user_data;
    assert(data->cs < fuse_gpio_count());
    assert(data->ck < fuse_gpio_count());
    assert(data->tx < fuse_gpio_count());
    assert(data->rx < fuse_gpio_count());

    // Check the GPIO combination, return the interface
    spi_inst_t* inst = fuse_spi_gpio_to_instance(data);
    if (!inst) {
        fuse_debugf(self, "fuse_spi_init: invalid GPIO combination\n");
        return false;
    } else {
        spi->inst = inst;
    }

    // Initialize CS
    if (data->cs) {
        spi->cs = fuse_new_gpio_ex(self, data->cs, FUSE_GPIO_OUT, 0, 0);
        if(!spi->cs) {
            fuse_debugf(self, "fuse_spi_init: cannot initialize CS (GPIO%d)\n", data->cs);
            return false;
        }
    }

    // Initialize CK
    spi->ck = fuse_new_gpio_ex(self, data->ck, FUSE_GPIO_SPI, 0, 0);
    if(!spi->ck) {
        fuse_debugf(self, "fuse_spi_init: cannot initialize SCK (GPIO%d)\n", data->ck);
        return false;
    }    

    // Initialize TX
    spi->tx = fuse_new_gpio_ex(self, data->tx, FUSE_GPIO_SPI, 0, 0);
    if(!spi->tx) {
        fuse_debugf(self, "fuse_spi_init: cannot initialize TX (GPIO%d)\n", data->tx);
        return false;
    }    

    // Initialize RX
    spi->rx = fuse_new_gpio_ex(self, data->rx, FUSE_GPIO_SPI, 0, 0);
    if(!spi->rx) {
        fuse_debugf(self, "fuse_spi_init: cannot initialize RX (GPIO%d)\n", data->rx);
        return false;
    }    

    // Initialise SPI
    uint32_t baudrate = spi_init(spi->inst, data->baudrate);
    if (baudrate == 0) {
        fuse_debugf(self, "fuse_spi_init: spi_init failed with baudrate %d\n",data->baudrate);
        return false;
    } else if (baudrate != data->baudrate) {
        fuse_debugf(self, "fuse_spi_init: spi_init set different baudrate (expected %d, actual %d)\n", data->baudrate, baudrate);
    }

    // Retain the GPIO pins
    fuse_retain(self, spi->cs);
    fuse_retain(self, spi->ck);
    fuse_retain(self, spi->tx);
    fuse_retain(self, spi->rx);

    // Chip select is active-low, so we'll initialise it to a driven-high state
    if(spi->cs) {
        fuse_gpio_set(spi->cs, true);
    }

    // Set pause
    spi->xfr_delayms = data->xfr_delayms;

    // Return success
    return true;
}

/** @brief Destroy the SPI interface
 */
static void fuse_spi_destroy(fuse_t *self, fuse_value_t *value) {
    assert(self);
    assert(value);

    // Disable SPI
    fuse_spi_t *spi = (fuse_spi_t *)value;
    assert(spi->inst);
    spi_deinit(spi->inst);

    // Release GPIO pins
    fuse_release(self, spi->cs);
    fuse_release(self, spi->ck);
    fuse_release(self, spi->tx);
    fuse_release(self, spi->rx);
}

/** @brief Append a JSON representation of an SPI interface
 */
static size_t fuse_spi_str(fuse_t *self, char *buf, size_t sz, size_t i, fuse_value_t *v, bool json) {
    assert(self);
    assert(buf == NULL || sz > 0);
    assert(v);
    
    // Add prefix
    i = chtostr_internal(buf, sz, i, '{');

    // Add instance number
    fuse_spi_t *spi = (fuse_spi_t *)v;
    i = qstrtostr_internal(buf, sz, i, "instance");
    i = chtostr_internal(buf, sz, i, ':');
    i = utostr_internal(buf, sz, i, spi_get_index(spi->inst), 0);
    i = chtostr_internal(buf, sz, i, ',');

    // Add CS
    if (spi->cs) {
        i = qstrtostr_internal(buf, sz, i, "cs");
        i = chtostr_internal(buf, sz, i, ':');
        i = vtostr_internal(self, buf, sz, i, (fuse_value_t *)spi->cs, true);
        i = chtostr_internal(buf, sz, i, ',');
    }

    // Add CK
    if (spi->ck) {
        i = qstrtostr_internal(buf, sz, i, "ck");
        i = chtostr_internal(buf, sz, i, ':');
        i = vtostr_internal(self, buf, sz, i, (fuse_value_t *)spi->ck, true);
        i = chtostr_internal(buf, sz, i, ',');
    }

    // Add TX
    if (spi->tx) {
        i = qstrtostr_internal(buf, sz, i, "tx");
        i = chtostr_internal(buf, sz, i, ':');
        i = vtostr_internal(self, buf, sz, i, (fuse_value_t *)spi->tx, true);
        i = chtostr_internal(buf, sz, i, ',');
    }

    // Add RX
    if (spi->rx) {
        i = qstrtostr_internal(buf, sz, i, "rx");
        i = chtostr_internal(buf, sz, i, ':');
        i = vtostr_internal(self, buf, sz, i, (fuse_value_t *)spi->rx, true);
        i = chtostr_internal(buf, sz, i, ',');
    }

    // Add xfr_delayms
    if (spi->xfr_delayms) {
        i = qstrtostr_internal(buf, sz, i, "xfr_delayms");
        i = chtostr_internal(buf, sz, i, ':');
        i = utostr_internal(buf, sz, i, (uint64_t)spi->xfr_delayms, 0);
        i = chtostr_internal(buf, sz, i, ',');
    }

    // Add Baudrate
    i = qstrtostr_internal(buf, sz, i, "baudrate");
    i = chtostr_internal(buf, sz, i, ':');
    i = utostr_internal(buf, sz, i, (uint64_t)spi_get_baudrate(spi->inst), 0);

    // Add suffix
    i = chtostr_internal(buf, sz, i, '}');

    return i;
}

/** @brief Return SPI instance from GPIO pins, or NULL if invalid pin combination
 */
static spi_inst_t * fuse_spi_gpio_to_instance(fuse_spi_data_t *data) {
    // SPI instance number
    int8_t n = -1;

    // RX pin  - SPI0[  0,  4, 16, 20 ] SPI1[  8, 12, 28 ]
    // CS pin  - SPI0[  1,  5, 17, 21 ] SPI1[  9, 13     ]
    // CK pin  - SPI0[  2,  6, 18     ] SPI1[ 10, 14, 26 ]
    // TX pin  - SPI0[  3,  7, 19     ] SPI1[ 11, 15, 27 ]

    // RX pin
    switch (data->rx) {
        case 0:
        case 4:
        case 16:
        case 20:      
            n = 0;
            break;
        case 8:
        case 12:
        case 28:
            n = 1;
            break;
        default:
            return NULL;
    }    

    // CS pin
    switch (data->cs) {
        case 0:
            break;
        case 1:
        case 5:
        case 17:
        case 21:
            if (n == 1)
                return NULL;
            n = 0;
            break;
        case 9:
        case 13:
            if (n == 0)
                return NULL;
            n = 1;
            break;
        default:
            return NULL;
    }

    // SCK pin
    switch (data->ck) {
        case 2:
        case 6:
        case 18:
            if (n == 1)
                return NULL;
            n = 0;
            break;
        case 10:
        case 14:
        case 26:
            if (n == 0)
                return NULL;
            n = 1;
            break;
        default:
            return NULL;
    }

    // TX pin
    switch (data->tx) {
        case 3:
        case 7:
        case 19:
            if (n == 1)
                return NULL;
            n = 0;
            break;
        case 11:
        case 15:
        case 27:
            if (n == 0)
                return NULL;
            n = 1;
            break;
        default:
            return NULL;
    }   

    // Return correct instance 
    switch (n) {
        case 0:
            return spi0;
        case 1:
            return spi1;
        default:
            return NULL;
    }
}

static inline void fuse_spi_cs(fuse_spi_t *spi, bool select) {
    assert(spi);
    if (spi->cs) {
        asm volatile("nop \n nop \n nop");
        fuse_gpio_set(spi->cs, !select);
        asm volatile("nop \n nop \n nop");
    }
}

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

/** @brief Initialize an SPI interface
 */
fuse_spi_t *fuse_new_spi_ex(fuse_t *self, fuse_spi_data_t data, const char *file, const int line) {
    assert(self);
    return (fuse_spi_t *)fuse_new_value_ex(self, FUSE_MAGIC_SPI, &data, file, line);
}

/** @brief Write a data block
 */
bool fuse_spi_write(fuse_t *self, fuse_spi_t *spi, void* data, size_t sz, bool blocking) {
     return fuse_spi_xfr(self, spi, data, 0, sz, blocking);
}

/** @brief Read a data block
 */
bool fuse_spi_read(fuse_t *self, fuse_spi_t *spi, void* data, size_t sz, bool blocking) {
    return fuse_spi_xfr(self, spi, data, sz, 0, blocking);
}

/** @brief Transfer data to and from an SPI slave device
 */
bool fuse_spi_xfr(fuse_t *self, fuse_spi_t *spi, void* data, size_t wsz, size_t rsz, bool blocking) {
    assert(self);
    assert(spi);
    assert(data);
    assert(wsz > 0 || rsz > 0);

    // Blocking
    // TODO
    if (!blocking) {
        fuse_debugf(self,"fuse_spi_tx: non-blocking transfers not yet supported\n");
        return false;
    }

    // Activate CS
    fuse_spi_cs(spi, true);

    // Write
    bool result = true;
    if (wsz > 0) {
        size_t written = spi_write_blocking(spi->inst,data,wsz);
        // Debugging
        if (written != wsz) {
            result = false;
#ifdef DEBUG
            fuse_debugf(self,"fuse_spi_tx: written %u bytes, expected %u\n",written,wsz);
#endif        
        }
    }

    // Pause
    if (wsz && spi->xfr_delayms) {
        sleep_ms(spi->xfr_delayms);
    }

    // Read
    if (rsz > 0) {
        size_t read = spi_read_blocking(spi->inst,0,data,rsz);

        // Debugging
        if (read != rsz) {
            result = false;
#ifdef DEBUG
            fuse_debugf(self,"fuse_spi_tx: read %u bytes, expected %u\n",read,rsz);
#endif        
        }
    }    

    // Deactivate CS
    fuse_spi_cs(spi, false);

    // Sleep
    if (rsz && spi->xfr_delayms) {
        sleep_ms(spi->xfr_delayms);
    }

    // Return success
    return result;    
}