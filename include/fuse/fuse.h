/** @file fuse.h
 *  @brief Fuse application function prototypes
 *
 *  This file contains the function prototypes for creating and destroying
 *  a fuse application. Commonly you would create the application in main,
 *  and then destroy it at the end of main, returning the exit code.
 */
#ifndef FUSE_H
#define FUSE_H

/** @brief The representation of a fuse application
 */
typedef struct fuse_application fuse_t;

#include "assert.h"
#include "debug.h"
#include "device.h"
#include "list.h"
#include "magic.h"
#include "map.h"
#include "mutex.h"
#include "printf.h"
#include "random.h"
#include "sleep.h"
#include "timer.h"
#include "value.h"

/** @brief Create a new fuse application
 *
 *  @param flags The flags to use for the fuse application, to modify behaviour
 *  @returns A new fuse application, or NULL if memory could not be allocated
 */
fuse_t *fuse_new();

/** @brief Deallocate a fuse application
 *
 *  This method releases all resources associated with a fuse application and returns
 *  an exit code. A zero exit code indicates success, and a non-zero exit code indicates
 *  failure.
 *
 *  @param fuse The fuse application to deallocate
 *  @returns The exit code of the application. Returns zero on success.
 */
int fuse_destroy(fuse_t *fuse);

/** @brief Allocate memory
 *
 *  This method allocates memory from the fuse memory pool. It will return NULL
 *  if the memory could not be allocated.
 *
 *  @param self The fuse application
 *  @param magic The magic number to use for the memory block
 *  @param user_data User data to pass to the initialisation function
 *  @param file The file from which the memory is allocated
 *  @param line The line in the file from which the memory is allocated
 *  @returns Pointer to the allocated memory block, or NULL if the memory could not be allocated
 */
void *fuse_alloc_ex(fuse_t *self, const uint16_t magic, const void *user_data, const char *file, const int line);

#ifdef DEBUG
#define fuse_alloc(self, magic, user_data) \
    (fuse_alloc_ex((self), (magic), (user_data), __FILE__, __LINE__))
#else
#define fuse_alloc(self, size, magic) \
    (fuse_alloc_ex((self), (magic), (user_data), 0, 0))
#endif

/** @brief Free allocated memory
 *
 *  This method frees allocated memory from the fuse memory pool
 *
 *  @param self The fuse application
 *  @param ptr A pointer to the allocated memory
 */
void fuse_free(fuse_t *self, void *ptr);

/** @brief Run the fuse application until an exit code is set
 *
 *  This method starts the event loop, which continues until the exit code is set.
 *
 *  @param self The fuse application
 *  @param callback The callback function which is called to initialise the application. If
 *                  the callback returns a non-zero value, then the event loop is terminated and
 *                  the exit code is set to this value.
 */
void fuse_run(fuse_t *self, int (*callback)(fuse_t *));

/** @brief Set the fuse exit code
 *
 *  This method should be called after the run loop has been started, in order to
 *  set the exit code for the application.
 *
 *  @param self The fuse application
 *  @param exit_code The exit code to set. Set the exit code to signal successful completion of the application
 */
void fuse_exit(fuse_t *self, const int exit_code);

/** @brief Drain the memory allocation pool
 *
 * This method empties auto-releaaed values (with a zero retain count).
 *
 * @param self The fuse instance
 * @param sz The maximum number of values to release, or 0 for all
 * @return The number of values that were released
 */
size_t fuse_drain(fuse_t *self, size_t cap);

#endif
