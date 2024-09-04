/** @file picofuse.h
 *  @brief Fuse application function prototypes specific to Pico
 *
 *  This file contains the function prototypes for Pico-specific devices
 */
#ifndef PICOFUSE_H
#define PICOFUSE_H

#include <fuse/fuse.h>
#include "gpio.h"
#include "led.h"

/* @brief Register picofuse types
 *
 * @param fuse The fuse application
 */
void picofuse_init(fuse_t *fuse);

#endif
