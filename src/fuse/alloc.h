/** @file alloc.h
 *  @brief Private function prototypes and structure definitions for allocators
 */
#ifndef FUSE_PRIVATE_ALLOC_H
#define FUSE_PRIVATE_ALLOC_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/** @brief Represents a memory block header
 */
struct fuse_allocator_header
{
    void *ptr;                          ///< A pointer to the memory block
    size_t size;                        ///< The size of the memory block, in bytes
    uint16_t magic;                     ///< A magic number
    uint16_t ref;                       ///< The reference count of the memory block
    struct fuse_allocator_header *prev; ///< The previous memory block header, or NULL if this is the first memory block header
    struct fuse_allocator_header *next; ///< The next memory block header, or NULL if this is the last memory block header
    void *head;                         ///< The previous value in a linked list
    void *tail;                         ///< The next value in a linked list
#ifdef DEBUG
    const char *file; ///< The file where the allocation was made
    int line;         ///< The line of the file where the allocation was made
#endif
};

/** @brief Represents an allocator implementation
 */
struct fuse_allocator
{
    void *(*malloc)(struct fuse_allocator *ctx, size_t size, uint16_t magic, const char *file, int line); ///< Memory allocator
    void (*free)(struct fuse_allocator *ctx, void *ptr);                                                  ///< Free function
    void (*destroy)(struct fuse_allocator *ctx);                                                          ///< Destroy function
    uint16_t (*magic)(struct fuse_allocator *ctx, void *ptr);                                             ///< Magic function
    size_t (*size)(struct fuse_allocator *ctx, void *ptr);                                                ///< Size function
    void (*retain)(struct fuse_allocator *ctx, void *ptr);                                                ///< Retain function
    bool (*release)(struct fuse_allocator *ctx, void *ptr);                                               ///< Release function
    void **(*headptr)(void *ptr);                                                                         ///< Pointer to the head pointer
    void **(*tailptr)(void *ptr);                                                                         ///< Pointer to the tail pointer

    struct fuse_allocator_header *head; ///< The head of the list of memory blocks
    struct fuse_allocator_header *tail; ///< The tail of the list of memory blocks
    size_t cur;                       ///< The total number of bytes allocated
    size_t max;                       ///< The max number of bytes allocated
};

/** @brief Allocate memory from the allocator
 *
 *  @param self The allocator object
 *  @param size The size of the memory block to allocate
 *  @param magic The magic number to use for the memory block
 *  @param file The file where the allocation was made
 *  @param line The line where the allocation was made
 *  @returns A pointer to the allocated memory block, or NULL if no memory could be allocated
 */
void *fuse_allocator_malloc(struct fuse_allocator *self, size_t size, uint16_t magic, const char *file, int line);

/** @brief Free a memory block in the memory pool
 *
 *  @param self The allocator object
 *  @param ptr A pointer to the memory block
 */
void fuse_allocator_free(struct fuse_allocator *self, void *ptr);

/** @brief Retrieve the magic number for a memory block
 *
 *  @param self The allocator object
 *  @param ptr A pointer to the memory block
 *  @returns The magic number for the memory block
 */
uint16_t fuse_allocator_magic(struct fuse_allocator *self, void *ptr);

/** @brief Retrieve the size for a memory block
 *
 *  @param self The allocator object
 *  @param ptr A pointer to the memory block
 *  @returns The size in bytes for the memory block
 */
size_t fuse_allocator_size(struct fuse_allocator *self, void *ptr);

/** @brief Retain a memory block by incrementing the reference counter
 *
 * @param self The allocator object
 * @param ptr A pointer to the memory block
 */
void fuse_allocator_retain(struct fuse_allocator *self, void *ptr);

/** @brief Release a memory block by decrementing the reference counter
 *
 * @param self The allocator object
 * @param ptr A pointer to the memory block
 * @returns True if the memory block was freed, false otherwise
 */
bool fuse_allocator_release(struct fuse_allocator *self, void *ptr);

/** @brief Release all memory in the pool and destroy the allocator
 *
 *  @param self The allocator object
 */
void fuse_allocator_destroy(struct fuse_allocator *self);

/** @brief Retrieve the head pointer for a memory block
 *
 * The head pointer is the pointer to the previous value in a linked list.
 *
 * @param self The allocator object
 * @param ptr A pointer to the memory block
 * @returns A pointer to the head pointer
 */
void **fuse_allocator_headptr(struct fuse_allocator *self, void *ptr);

/** @brief Retrieve the tail pointer for a memory block
 *
 * The tail pointer is the pointer to the next value in a linked list.
 *
 * @param self The allocator object
 * @param ptr A pointer to the memory block
 * @returns A pointer to the tail pointer
 */
void **fuse_allocator_tailptr(struct fuse_allocator *self, void *ptr);

#endif
