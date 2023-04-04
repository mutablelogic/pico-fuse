
#ifndef RUNLOOP_H
#define RUNLOOP_H

// Flags for what to enable in the runloop
typedef enum
{
    ADC_0 = (1 << 0),  // Initialize ADC 0
    ADC_1 = (1 << 1),  // Initialize ADC 1
    ADC_2 = (1 << 2),  // Initialize ADC 2
    ADC_3 = (1 << 3),  // Initialize ADC 3
    ADC_4 = (1 << 4),  // Initialize ADC 4 (Temperature)
    UART_0 = (1 << 6), // Initialize UART 0
    UART_1 = (1 << 7), // Initialize UART 1
    WIFI = (1 << 8),   // Initialize WiFi connections
    DNS = (1 << 9),    // Initialize DNS resolution
    MQTT = (1 << 10),  // Initialize MQTT
} runloop_flags_t;

typedef enum
{
    EVENT_NONE,              // No event
    EVENT_INIT,              // Initialize the runloop
    EVENT_WIFI_INIT,         // Return the AP name, password and country
    EVENT_WIFI_CONNECTED,    // WiFi has been connected
    EVENT_WIFI_DISCONNECTED, // WiFi did not connect or is disconnected
    EVENT_MQTT_INIT,         // Return the broker address and credentials
    EVENT_MQTT_CONNECTED,    // MQTT connected to broker
    EVENT_MQTT_DISCONNECTED, // MQTT disconnected from broker
    EVENT_MQTT_SEND,         // MQTT publish to a topic
    EVENT_MQTT_SUBSCRIBE,    // MQTT subscribe to a topic
    EVENT_MQTT_UNSUBSCRIBE,  // MQTT unsubscribe from a topic
    EVENT_MQTT_RECEIVE,      // MQTT receive a message from a topic
    EVENT_ADC_INIT,          // ADC initialization
    EVENT_ADC_SAMPLE,        // ADC sample
    EVENT_GPIO_INIT,         // Set GPIO pin modes
} runloop_event_t;

// The current state of the runloop. Use ANY to match any state
// or to not match any state.
typedef enum
{
    ANY = -1, // Matches any state
    ZERO = 0, // Initial runloop state
} runloop_state_t;

// Opaque runloop structure
typedef struct runloop_instance_t runloop_t;

// Event callback
typedef runloop_state_t runloop_callback_t(
    runloop_t *runloop, runloop_state_t state, runloop_event_t event, void *args);

// Initialize the runloop structure
extern runloop_t *runloop_init(runloop_flags_t flags);

// Free runloop structure
extern void runloop_free(runloop_t *runloop);

// Run the loop forever
extern void runloop_main(runloop_t *runloop);

// Push an event onto the runloop, returns -1 on error or 0 on success
extern int runloop_push(runloop_t *runloop, runloop_event_t type, void *data);

// Register a handler for a specific state and event pair and return 0 on success
extern int runloop_event(runloop_t *runloop, runloop_state_t state,
                         runloop_event_t event, runloop_callback_t *callback);

///////////////////////////////////////////////////////////////////////////////
// EVENT_INIT argument

typedef struct
{
    const char *appName; // The name of the application
} runloop_init_t;

///////////////////////////////////////////////////////////////////////////////
// EVENT_ADC_INIT argument

typedef struct
{
    const char *pinName; // The name of the pin
    int channel;         // The ADC channel
    int gpio;            // The GPIO pin
} runloop_adc_t;

///////////////////////////////////////////////////////////////////////////////
// EVENT_GPIO_INIT argument

typedef struct
{
    const char *pinName; // The name of the pin
    int gpio;            // The GPIO pin
    int direction;       // The GPIO mode (GPIO_OUT, GPIO_IN)
    bool pullup;         // Enable pullup
    bool pulldown;       // Enable pulldown (setting both pulls enables a "bus keep" function)
    bool irqrise;        // The GPIO level GPIO_IRQ_LEVEL_RISE
    bool irqfall;        // The GPIO level GPIO_IRQ_LEVEL_FALL
} runloop_gpio_t;

#endif
