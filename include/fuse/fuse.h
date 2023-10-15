#ifndef FUSE_H
#define FUSE_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/*
 * Represents a fuse application
 */
typedef struct fuse_instance fuse_t;

#include <fuse/debug.h>
#include <fuse/flags.h>
#include <fuse/pool.h>
#include <fuse/sleep.h>

/*
 * Create a new fuse application
 *
 * @returns A new fuse application, or NULL if memory could not be allocated
 */
fuse_t *fuse_init(fuse_flag_t flags);

/*
 * Deallocate a fuse application
 *
 * @param fuse The fuse application to deallocate
 *
 */
void fuse_destroy(fuse_t *fuse);

#endif
