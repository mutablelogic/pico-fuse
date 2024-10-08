/** @file uc8151.h
 *  @brief Function prototypes for e-ink driver UC8151
 */
#ifndef PICOFUSE_UC8151_H
#define PICOFUSE_UC8151_H
#include <picofuse/spi.h>

#ifdef DEBUG
#define fuse_new_uc8151(self, spi) \
    ((fuse_bme280_t *)fuse_new_uc8151_ex((self), (spi), __FILE__, __LINE__))
#else
#define fuse_new_uc8151(self, spi) \
    ((fuse_bme280_t *)fuse_new_uc8151_ex((self), (spi), 0, 0))
#endif

/** @brief An opaque UC8151 object
 */
typedef struct uc8151_context fuse_uc8151_t;

#endif
