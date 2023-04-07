
#ifndef PICOFUSE_EV_H
#define PICOFUSE_EV_H

extern void picofuse_callback(picofuse_t *, picofuse_event_t, void *);
extern void picofuse_handle_init(picofuse_t *, picofuse_init_t *);
extern void picofuse_handle_quit(picofuse_t *, picofuse_init_t *);

// GPIO
extern void picofuse_handle_gpio_init(picofuse_t *, picofuse_gpio_t *);

// On-board LED
extern void picofuse_handle_led_init(picofuse_t *, picofuse_led_t *);
extern void picofuse_handle_led(picofuse_t *, picofuse_led_t *, bool);

// Scheduled callback. Returns false to cancel the callback, else
// return true to reschedule the callback.
typedef bool (*picofuse_schedule_callback_t)(picofuse_t *, void *);

extern int picofuse_timer_init(picofuse_t *);
extern void picofuse_timer_deinit(picofuse_t *);
extern void picofuse_handle_timer_init(picofuse_t *, picofuse_timer_t *);
extern void picofuse_handle_timer(picofuse_t *, picofuse_timer_t *);
extern bool picofuse_schedule_ms(picofuse_t *, uint32_t, picofuse_schedule_callback_t, void *);

// WiFi
extern void picofuse_wifi_poll(picofuse_t *);
extern void picofuse_handle_wifi_init(picofuse_t *, picofuse_wifi_t *);
// extern void picofuse_handle_wifi_status(picofuse_t *, picofuse_wifi_t *);

#endif
