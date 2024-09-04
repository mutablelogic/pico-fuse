/** @file led.h
 *  @brief Function prototypes to control the on-board LED
 */
#ifndef PICOFUSE_LED_H
#define PICOFUSE_LED_H

#include <stdbool.h>

#ifdef DEBUG
#define fuse_new_led(self) \
    ((fuse_led_t *)fuse_new_led_ex((self), __FILE__, __LINE__))
#else
#define fuse_new_led(self) \
    ((fuse_led_t *)fuse_new_led_ex((self), 0, 0))
#endif

/** @brief An opaque led object
 */
typedef struct led_context fuse_led_t;

/** @brief Initialize the on-board LED
 *
 * @param self The fuse application
 * @return The LED, or NULL if the LED could not be initialized
 */
fuse_led_t *fuse_new_led_ex(fuse_t *self, const char *file, const int line);

/** @brief Set the LED binary value
 *
 *  @param led The LED
 *  @param value The binary value of the LED
 */
void fuse_led_set(fuse_led_t *pin, bool value);

/** @brief Get the LED binary value
 *
 *  @param led The LED
 *  @return The binary value of the LED
 */
bool fuse_led_get(fuse_led_t *pin);

#endif
