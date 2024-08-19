/** @file picofuse.h
 *  @brief Fuse application function prototypes specific to Pico
 *
 *  This file contains the function prototypes for Pico-specific devices
 */
#ifndef PICOFUSE_H
#define PICOFUSE_H

#include <fuse/fuse.h>
#include <picofuse/adc.h>
#include <picofuse/gpio.h>
#include <picofuse/led.h>
#include <picofuse/main.h>
#include <picofuse/mqtt.h>
#include <picofuse/stdio.h>
#include <picofuse/wifi.h>

extern fuse_device_t wifi; ///< WiFi device
extern fuse_device_t mqtt; ///< MQTT device

#endif
