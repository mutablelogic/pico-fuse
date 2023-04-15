
#ifndef FUSE_MAIN_H
#define FUSE_MAIN_H

#include <stddef.h>
#include <stdbool.h>

/*
 * Represents a fuse application
 */
typedef struct fuse_instance fuse_t;

/*
 * Represents a fuse driver
 */
typedef struct
{
    // The name of the driver
    const char *name;

    // The init function for the driver, which returns a pointer to
    // the driver's instance, or NULL if an error occurred
    void *(*init)(fuse_t *fuse, void *user_data);
} fuse_driver_params_t;

/*
 * Representation of an action which occurs when an event is fired
 */
typedef void (*fuse_action_t)(fuse_t *fuse, fuse_event_t event, void *data);

/*
 * Flags for the fuse application
 */
typedef enum
{
    // No flags
    FUSE_FLAG_NONE = 0,

    // Debug mode - prints out more verbose output
    FUSE_FLAG_DEBUG = 1 << 0,
} fuse_flag_t;

/*
 * Create a new fuse application
 *
 * Initializes a fuse application with the given size of memory which
 * will be used for the fuse application. The flags parameter can affect
 * the behaviour of the fuse application.
 *
 * @param size    The size of memory to use for the fuse application
 * @param flags   The flags to use for the fuse application
 * @return        A pointer to the fuse application, or NULL if an error occurred
 *                which is usually due to not enough memory being allocated.
 */
fuse_t *fuse_init(size_t size, fuse_flag_t flags);

/*
 * Free resources from a fuse application, and stop the application
 *
 * @param fuse             The fuse application
 * @return                 The return code from the application, which
 *                         will be non-zero
 */
int fuse_exit(fuse_t *fuse);

/*
 * Process events until the application is stopped
 *
 * @param fuse             The fuse application
 */
void fuse_main(fuse_t *fuse);

/*
 * Check for existence of fuse flags
 *
 * @param fuse             The fuse application
 * @param flag             The flags to test, which are OR'd together
 * @return                 Return true if all flags are set, otherwise false
 */
bool fuse_is(fuse_t *fuse, fuse_flag_t flag);

/*
 * Register a driver with the fuse application
 *
 * @param fuse             The fuse application
 * @param params           The parameters for the driver
 * @return                 Returns a pointer to the driver's instance, or NULL
 */
void *fuse_register_driver(fuse_t *fuse, const fuse_driver_params_t *params, void *user_data);

/*
 * Register an action for an event firing
 *
 * @param fuse             The fuse application
 * @param event            The event that has been fired
 * @param action           The action to perform when the event is fired
 * @return                 Returns false if an error occurred
 */
bool fuse_register_action(fuse_t *fuse, void *driver, fuse_event_t event, fuse_action_t action);

#endif
