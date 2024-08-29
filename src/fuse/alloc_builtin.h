/** @file alloc_builtin.h
 *  @brief Private function prototypes and structure definitions for allocators
 */
#ifndef FUSE_PRIVATE_ALLOC_BUILTIN_H
#define FUSE_PRIVATE_ALLOC_BUILTIN_H

/* @brief Create a new allocator 
 * 
 * Create a new allocator which uses the system malloc and free functions to create
 * and destroy memory blocks.
 * 
 * @returns A pointer to the new allocator
 */
struct fuse_allocator *fuse_allocator_builtin_new();

#endif
