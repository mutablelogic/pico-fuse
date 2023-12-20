/** @file fuse.h
 *  @brief Fuse application function prototypes
 *
 *  This file contains the function prototypes for creating and destroying
 *  a fuse application. Commonly you would create the application in main,
 *  and then destroy it at the end of main, returning the exit code.
 */
#ifndef FUSE_H
#define FUSE_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/** @brief The representation of a fuse application
 */
typedef struct fuse_instance fuse_t;

#include <fuse/debug.h>
#include <fuse/flags.h>
#include <fuse/list.h>
#include <fuse/map.h>
#include <fuse/pool.h>
#include <fuse/random.h>
#include <fuse/sleep.h>
#include <fuse/value.h>

/** @brief Create a new fuse application
 *
 *  @param flags The flags to use for the fuse application, to modify behaviour
 *  @returns A new fuse application, or NULL if memory could not be allocated
 */
fuse_t *fuse_new(fuse_flag_t flags);

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
 *  @param size Number of bytes to allocate
 *  @param file The file from which the memory is allocated
 *  @param line The line in the file from which the memory is allocated
 *  @returns Pointer to the allocated memory block, or NULL if the memory could not be allocated
 */
void *fuse_alloc_ex(fuse_t *self, size_t size, const char *file, int line);

#ifdef DEBUG
#define fuse_alloc(self, size) \
    (fuse_alloc_ex((self), (size), __FILE__, __LINE__))
#else
#define fuse_alloc(self, size) \
    (fuse_alloc_ex((self), (size), 0, 0))
#endif

/** @brief Free allocated memory
 *
 *  This method frees allocated memory from the fuse memory pool
 *
 *  @param self The fuse application
 *  @param ptr A pointer to the allocated memory
 */
void fuse_free(fuse_t *self, void *ptr);

#endif
