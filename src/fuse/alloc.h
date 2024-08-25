/** @file alloc.h
 *  @brief Private function prototypes and structure definitions for allocators
 */
#ifndef FUSE_PRIVATE_ALLOC_H
#define FUSE_PRIVATE_ALLOC_H

#include <stddef.h>
#include <stdbool.h>

/** @brief Represents a memory block header
 */
struct fuse_allocator_header
{
    void *ptr;                          ///< A pointer to the memory block
    size_t size;                        ///< The size of the memory block, in bytes
    uint16_t magic;                     ///< A magic number
    uint16_t ref;                       ///< The reference count of the memory block
    bool used;                          ///< Whether the memory block is used
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
};

/** @brief Retrieve the head pointer for a memory block
 *
 * @param self The allocator object
 * @param ptr A pointer to the memory block
 * @returns A pointer to the head pointer
 */
void **fuse_allocator_headptr(fuse_allocator_t *self, void *ptr);

/** @brief Retrieve the tail pointer for a memory block
 *
 * @param self The allocator object
 * @param ptr A pointer to the memory block
 * @returns A pointer to the tail pointer
 */
void **fuse_allocator_tailptr(fuse_allocator_t *self, void *ptr);

// Built-in methods
void *fuse_allocator_builtin_malloc(struct fuse_allocator *ctx, size_t size, uint16_t magic, const char *file, int line);
void fuse_allocator_builtin_free(struct fuse_allocator *ctx, void *ptr);
void fuse_allocator_builtin_destroy(struct fuse_allocator *ctx);
uint16_t fuse_allocator_builtin_magic(struct fuse_allocator *ctx, void *ptr);
size_t fuse_allocator_builtin_size(struct fuse_allocator *ctx, void *ptr);
void fuse_allocator_builtin_retain(struct fuse_allocator *ctx, void *ptr);
bool fuse_allocator_builtin_release(struct fuse_allocator *ctx, void *ptr);
void **fuse_allocator_builtin_headptr(void *ptr);
void **fuse_allocator_builtin_tailptr(void *ptr);

#endif
