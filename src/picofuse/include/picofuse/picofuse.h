
#ifndef PICOFUSE_H
#define PICOFUSE_H

// picofuse flags
typedef enum
{
    PICOFUSE_DEBUG = (1 << 0), // Enable debug output
} picofuse_flags_t;

typedef enum
{
    EV_NONE, // No event
    EV_INIT, // Initialize the runloop
    EV_QUIT, // Quit the runloop
    EV_LED,  // LED changed state
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
typedef picofuse_state_t picofuse_callback_t(picofuse_t *,
                                             picofuse_state_t,
                                             picofuse_event_t,
                                             void *);

// Initialize picofuse
extern picofuse_t *picofuse_init(picofuse_flags_t);

// Run the loop
extern int picofuse_main(picofuse_t *);

// Free the picofuse structure
extern void picofuse_free(picofuse_t *);

// Fire an event on the runloop, returns -1 on error or 0 on success
extern int picofuse_fire(picofuse_t *,
                         picofuse_event_t,
                         void *);

// Register an event handler
extern int picofuse_register(picofuse_t *,
                             picofuse_state_t,
                             picofuse_event_t,
                             picofuse_callback_t *);

///////////////////////////////////////////////////////////////////////////////
// EV_INIT

typedef struct
{
    const char *appName;          // The name of the application, or NULL
    const char *countryCode;      // Two-letter country code for the device, or NULL
    const char *deviceIdentifier; // Unique identifier for the device
    int errorCode;                // Error code, or 0
} picofuse_init_t;

#endif // PICOFUSE_H
