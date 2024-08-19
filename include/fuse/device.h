/** @file device.h
 *  @brief Fuse device registration
 */
#ifndef FUSE_DEVICE_H
#define FUSE_DEVICE_H

///////////////////////////////////////////////////////////////////////////////
// TYPES

/** @brief Fuse device
 */
typedef struct
{
    const char *name;
    void *(*init)(fuse_t *fuse, void *userdata);
    void (*destroy)(fuse_t *fuse, void *context);
} fuse_device_t;

/** @brief Fuse context
 */
typedef void *fuse_context_t;

///////////////////////////////////////////////////////////////////////////////
// METHODS

/** @brief Register a fuse device
 *
 *  @param fuse The fuse application
 *  @param device A pointer to the device to register
 *  @param user_data Additional arguments that the device needs for registration
 *  @returns A device context, or NULL if registration failed
 */
fuse_context_t *fuse_register(fuse_t *fuse, fuse_device_t *device, void *user_data);

#endif
