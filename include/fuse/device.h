/** @file device.h
 *  @brief Fuse device registration
 */
#ifndef FUSE_DEVICE_H
#define FUSE_DEVICE_H

///////////////////////////////////////////////////////////////////////////////
// TYPES

/** @brief Fuse device context
 */
typedef void *fuse_context_t;

/** @brief Fuse device
 */
typedef struct
{
    const char *name;
    fuse_context_t *(*init)(fuse_t *fuse, void *userdata);
    void (*destroy)(fuse_t *fuse, fuse_context_t *context);
} fuse_device_t;

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
