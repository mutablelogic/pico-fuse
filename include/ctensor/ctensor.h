/** @file ctensor.h
 *  @brief Tensor application prototypes
 *
 *  This file contains the function prototypes for creating and destroying
 *  tensors and tensor graphs
 */
#ifndef CTENSOR_H
#define CTENSOR_H

#include <stdlib.h>
#include <fuse/fuse.h>

/** @brief The representation of a tensor
 */
typedef struct tensor tensor_t;


#ifdef DEBUG
#define ctensor_new_f32(fuse, shape, elems) \
    (ctensor_new((fuse), FUSE_MAGIC_F32, (shape), (elems),  __FILE__, __LINE__))
#else
#define ctensor_new_f32(fuse, shape, elems) \
    (ctensor_new((fuse), FUSE_MAGIC_F32, (shape), (elems),  0, 0))
#endif

/** @brief Create a new tensor
 *
 *  @param fuse The fuse application to allocate memory from
 *  @param magic The data type of the tensor, which should be one of the FUSE_MAGIC_* constants
 *  @param shape A pointer to an array of dimensions for the tensor, with zero as the last element.
 *   If the tensor is a scalar, set the shape to null
 *  @param elems A pointer to data elements to initialise the tensor. Set to NULL if the
 *    tensor should be zero-initialised
 *  @returns A new tensor, or NULL if the tensor could not be allocated
 */
tensor_t *ctensor_new(fuse_t *fuse, uint16_t magic, uint32_t* shape, void *elems, const char *file, int line);

/** @brief Deallocate a tensor
 *
 *  This method releases all resources associated with a tensor
 *
 *  @param value The fuse application to deallocate
 *  @returns The exit code of the application. Returns zero on success.
 */
int ctensor_destroy(tensor_t *value);

#endif
