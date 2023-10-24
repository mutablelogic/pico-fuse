/** @file led.h
 *  @brief Routines to switch the on-board LED on and off
 */
#ifndef PICOFUSE_LED_H
#define PICOFUSE_LED_H

/** @brief Set the on-board LED value
 * 
 *  @param value The value to set the LED to
 */
void fuse_led_set(bool value);

/** @brief Initialise a GPIO for the on-board LED
 */
void fuse_led_new();

/** @brief  Resets a GPIO back for the on-board LED
 */
void fuse_led_destroy();

#endif
