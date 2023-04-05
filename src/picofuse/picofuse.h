
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

// Initialize picofuse
extern picofuse_t *picofuse_init(picofuse_flags_t);

// Run the loop
extern void picofuse_main(picofuse_t *);

// Free the picofuse structure
extern void picofuse_free(picofuse_t *);

///////////////////////////////////////////////////////////////////////////////
// EV_INIT

typedef struct
{
    const char *appName;          // The name of the application, or NULL
    const char *countryCode;      // Two-letter country code for the device, or NULL
    const char *deviceIdentifier; // Unique identifier for the device
} picofuse_init_t;

#endif // PICOFUSE_H
