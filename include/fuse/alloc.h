
/** @file alloc.h
 *  @brief Function prototypes for memory allocation
 *
 *  This contains the prototypes for memory allocation strategies
 */
#ifndef FUSE_ALLOC_H
#define FUSE_ALLOC_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/** @brief Opaque type for an allocator
 */
typedef struct fuse_allocator fuse_allocator_t;

/** @brief Opaque type for an allocator header
 */
typedef struct fuse_allocator_header fuse_allocator_header_t;

/** @brief Callback function for walking the memory pool
 *
 *  @param ptr A pointer to the memory block
 *  @param size The size of the memory block, in bytes (not including any memory metadata)
 *  @param magic The magic number for the memory block
 *  @param file The file where the allocation was made
 *  @param line The line of the file where the allocation was made
 */
typedef void (*fuse_allocator_walk_callback_t)(fuse_allocator_header_t *hdr, void *user);

/** @brief Create a new allocator using the bulit-in malloc and free functions
 *
 *  @returns An allocator object
 */
fuse_allocator_t *fuse_allocator_builtin_new();

/** @brief Allocate memory from the allocator
 *
 *  @param self The allocator object
 *  @param size The size of the memory block to allocate
 *  @param magic The magic number to use for the memory block
 *  @param file The file where the allocation was made
 *  @param line The line where the allocation was made
 *  @returns A pointer to the allocated memory block, or NULL if no memory could be allocated
 */
void *fuse_allocator_malloc(fuse_allocator_t *self, size_t size, uint16_t magic, const char *file, int line);

/** @brief Free a memory block in the memory pool
 *
 *  @param self The allocator object
 *  @param ptr A pointer to the memory block
 */
void fuse_allocator_free(fuse_allocator_t *self, void *ptr);

/** @brief Retrieve the magic number for a memory block
 *
 *  @param self The allocator object
 *  @param ptr A pointer to the memory block
 *  @returns The magic number for the memory block
 */
uint16_t fuse_allocator_magic(fuse_allocator_t *self, void *ptr);

/** @brief Retrieve the size for a memory block
 *
 *  @param self The allocator object
 *  @param ptr A pointer to the memory block
 *  @returns The size in bytes for the memory block
 */
size_t fuse_allocator_size(fuse_allocator_t *self, void *ptr);

/** @brief Retain a memory block by incrementing the reference counter
 *
 * @param self The allocator object
 * @param ptr A pointer to the memory block
 */
void fuse_allocator_retain(fuse_allocator_t *self, void *ptr);

/** @brief Release a memory block by decrementing the reference counter
 *
 * @param self The allocator object
 * @param ptr A pointer to the memory block
 * @returns True if the memory block was freed, false otherwise
 */
bool fuse_allocator_release(fuse_allocator_t *self, void *ptr);

/** @brief Release all memory in the pool and destroy the allocator
 *
 *  @param self The allocator object
 */
void fuse_allocator_destroy(fuse_allocator_t *self);

/** @brief Walk the memory pool from the head and call a callback function for each used memory block
 *
 * @param self The allocator object
 * @param ctx The context to pass to the callback function, or NULL if there is no context yet
 * @param callback The callback function to call for each memory block
 * @param user The user data to pass to the callback function
 * @returns The context to use for the next call to this function, or NULL if there are no more memory blocks
 */
void *fuse_allocator_walk(fuse_allocator_t *self, void *ctx, fuse_allocator_walk_callback_t callback, void *user);

#endif
