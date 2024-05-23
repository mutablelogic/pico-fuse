/** @file tensor.h
 *  @brief Private function prototypes and structure definitions for tensors
 */
#ifndef CTENSOR_PRIVATE_TENSOR_H
#define CTENSOR_PRIVATE_TENSOR_H

#include <stdlib.h>

/** @brief The maximum number of dimensions a tensor can have
 */
#define TENSOR_MAX_DIMS 16

/* @brief Represents an instance of a tensor
 */
struct tensor
{
    uint16_t magic;
    uint8_t dimensions;
    uint32_t shape[TENSOR_MAX_DIMS + 1];
    void *data;
};

#endif
