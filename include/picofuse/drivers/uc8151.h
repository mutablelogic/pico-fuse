/** @file uc8151.h
 *  @brief Function prototypes for e-ink driver UC8151
 */
#ifndef PICOFUSE_UC8151_H
#define PICOFUSE_UC8151_H

#include <stdint.h>
#include <stdbool.h>
#include <picofuse/spi.h>

#ifdef DEBUG
#define fuse_new_uc8151(self, config) \
    ((fuse_uc8151_t *)fuse_new_uc8151_ex((self), (config), __FILE__, __LINE__))
#else
#define fuse_new_uc8151(self, config) \
    ((fuse_uc8151_t *)fuse_new_uc8151_ex((self), (config), 0, 0))
#endif

/** @brief An opaque UC8151 object
 */
typedef struct uc8151_context fuse_uc8151_t;


/** @brief Update speed for the e-ink display
 */
typedef enum
{
    FUSE_UC8151_DEFAULT = 0,
    FUSE_UC8151_MEDIUM, 
    FUSE_UC8151_FAST,
    FUSE_UC8151_TURBO
} fuse_uc8151_update_t;

/** @brief Data used to initialize the UC8151 instance
 */
typedef struct 
{
    fuse_spi_t* spi; ///< The SPI instance    
    uint8_t dc; ///< Data/Command GPIO.
    uint8_t reset; ///< Reset GPIO (active low). Can be zero if unused.
    uint8_t busy; ///< Busy GPIO (active low). Can be zero if unused.
    fuse_uc8151_update_t update; ///< The update speed. Can be zero for default.
    uint16_t width; ///< The width of the display in pixels. Required.
    uint16_t height; ///< The height of the display in pixels. Required.
} fuse_uc8151_config_t;

/** @brief Create a new UC8151 instance
 *
 * @param self The fuse application
 * @param config The data used to initialize the UC8151 instance
 * @return The UC8151 instance, or NULL if it could not be initialized
 */
fuse_uc8151_t *fuse_new_uc8151_ex(fuse_t *self,fuse_uc8151_config_t config, const char *file, const int line);

#endif
