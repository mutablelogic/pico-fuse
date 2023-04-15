
#ifndef PICOFUSE_H
#define PICOFUSE_H

#include <stdbool.h>
#include <pico/stdlib.h>

// picofuse flags
typedef enum
{
    PICOFUSE_DEBUG = (1 << 0), // Enable debug output
} picofuse_flags_t;

/*
 * Events
 */
typedef enum
{
    EV_NONE,       // No event
    EV_INIT,       // Initialize the runloop
    EV_QUIT,       // Quit the runloop
    EV_LED_INIT,   // LED initialization hook
    EV_LED,        // LED changed state
    EV_TIMER_INIT, // Timer initialization hook
    EV_TIMER,      // Timer fired
    EV_WIFI_INIT,  // WiFi initialization hook
    EV_WIFI,       // WiFi changed state
    EV_GPIO_INIT,  // GPIO initialization hook
    EV_GPIO,       // GPIO changed state
} picofuse_event_t;

// The current state of the picofuse. Use ANY to match any state
// or to not match any state.
typedef enum
{
    ANY = -1, // Matches any state
    ZERO = 0, // Initial runloop state
} picofuse_state_t;

// Opaque picofuse structure
typedef struct picofuse_instance_t picofuse_t;

// Event callback
typedef picofuse_state_t picofuse_callback_t(picofuse_t *, picofuse_event_t, void *);

// Driver structure
typedef struct
{
    const char *name;           // Driver name
    void (*init)(picofuse_t *); // Driver initialization hook
} picofuse_driver_t;

// Initialize picofuse
extern picofuse_t *picofuse_init(picofuse_flags_t);

// Run the loop
extern int picofuse_main(picofuse_t *);

// Free the picofuse structure
extern void picofuse_free(picofuse_t *);

// Register a driver
extern void picofuse_register_driver(picofuse_t *, picofuse_driver_t *);

// Fire an event on the runloop, returns -1 on error or 0 on success
extern int picofuse_fire(picofuse_t *,
                         picofuse_event_t,
                         void *);

// Fire an event with boolean state on the runloop, returns -1 on error or 0 on success
extern int picofuse_fire_bool(picofuse_t *,
                              picofuse_event_t,
                              bool);

// Register an event handler
extern int picofuse_register(picofuse_t *,
                             picofuse_state_t,
                             picofuse_event_t,
                             picofuse_callback_t *);

// Return the current event
extern picofuse_event_t picofuse_event(picofuse_t *);

// Return the current state
extern picofuse_state_t picofuse_state(picofuse_t *);

// Return event as a string
extern const char *picofuse_event_str(picofuse_event_t event);

///////////////////////////////////////////////////////////////////////////////
// EV_INIT

typedef struct
{
    const char *appName;          // The name of the application, or NULL
    const char *countryCode;      // Two-letter country code for the device, or NULL
    const char *deviceIdentifier; // Unique identifier for the device
    int errorCode;                // Error code, or 0
} picofuse_init_t;

///////////////////////////////////////////////////////////////////////////////
// EV_LED

typedef struct
{
    int gpio;        // Pin number
    bool cyw43_arch; // Pico W;
    bool value;      // LED value
} picofuse_led_t;

///////////////////////////////////////////////////////////////////////////////
// EV_GPIO

typedef enum
{
    GPIO_FUNC_INPUT,  // GPIO is an input
    GPIO_FUNC_OUTPUT, // GPIO is an output
} picofuse_gpio_func_t;

typedef struct
{
    int gpio;                  // Pin number
    bool value;                // The GPIO value
    picofuse_gpio_func_t func; // The GPIO function
    bool pullup;               // Enable pullup
    bool pulldown;             // Enable pulldown (setting both pulls enables a "bus keep" function)
    bool irqrise;              // The GPIO level GPIO_IRQ_LEVEL_RISE
    bool irqfall;              // The GPIO level GPIO_IRQ_LEVEL_FALL
} picofuse_gpio_t;

///////////////////////////////////////////////////////////////////////////////
// EV_TIMER

typedef struct
{
    int id;           // Timer ID
    int32_t delay_ms; // Delay in milliseconds
    bool periodic;    // Periodic timer (set to false to cancel the timer)
} picofuse_timer_t;

///////////////////////////////////////////////////////////////////////////////
// EV_WIFI

#define WIFI_ADDR_STRLEN 20

typedef enum
{
    WIFI_UNKNOWN = -1,
    WIFI_DOWN,
    WIFI_JOINING,
    WIFI_NOIP,
    WIFI_FAIL,
    WIFI_BADAUTH,
    WIFI_UP
} picofuse_wifi_status_t;

typedef struct
{
    const char *ssid;               // The SSID of the network
    const char *password;           // The password of the network, or NULL
    picofuse_wifi_status_t status;  // The status of the WiFi connection
    char addr[WIFI_ADDR_STRLEN];    // The IP address once connected
    char netmask[WIFI_ADDR_STRLEN]; // The netmask once connected
    char gateway[WIFI_ADDR_STRLEN]; // The gateway once connected
} picofuse_wifi_t;

// Return status as a astring
extern const char *picofuse_wifi_status_string(picofuse_wifi_status_t);

#endif // PICOFUSE_H
