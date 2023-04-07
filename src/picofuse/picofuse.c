#include <stdlib.h>
#include <pico/stdlib.h>
#include <picofuse/picofuse.h>

#include "ev.h"
#include "hashmap.h"
#include "debug.h"

// Number of elements in the hashmap (so can register up to 100 handlers)
#define HASHMAP_SIZE 100

///////////////////////////////////////////////////////////////////////////////
// Structures

struct picofuse_node_t
{
    picofuse_event_t type;
    void *data;
    struct picofuse_node_t *next;
};

struct picofuse_instance_t
{
    picofuse_state_t state;
    picofuse_event_t event;
    picofuse_flags_t flags;
    struct picofuse_node_t *head;
    struct picofuse_node_t *tail;
    hashmap_t *hashmap;
};

///////////////////////////////////////////////////////////////////////////////
// Global variables

static picofuse_t *self = NULL;
static picofuse_init_t picofuse_init_data;
static picofuse_led_t picofuse_led_data;
static picofuse_wifi_t picofuse_wifi_data;

///////////////////////////////////////////////////////////////////////////////
// Initialize the runloop structure

picofuse_t *picofuse_init(picofuse_flags_t flags)
{
    if (self != NULL)
    {
        return self;
    }

    self = malloc(sizeof(picofuse_t));
    if (self == NULL)
    {
        return NULL;
    }

    self->head = NULL;
    self->tail = NULL;
    self->state = ZERO;
    self->flags = flags;

    // Create hashmap
    self->hashmap = hashmap_init(HASHMAP_SIZE);
    if (self->hashmap == NULL)
    {
        free(self);
        self = NULL;
        return NULL;
    }

    // Add an EV_INIT event onto the queue to get things started
    if (picofuse_fire(self, EV_INIT, &picofuse_init_data))
    {
        free(self);
        self = NULL;
        return NULL;
    }

    // Return the picofuse object
    return self;
}

///////////////////////////////////////////////////////////////////////////////
// Determine if list is empty

bool picofuse_is_empty(picofuse_t *runloop)
{
    return runloop->head == NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Get next event, or returns EVENT_NONE

picofuse_event_t picofuse_next(picofuse_t *self, void **data)
{
    if (picofuse_is_empty(self))
    {
        return EV_NONE;
    }
    struct picofuse_node_t *node = self->head;
    self->head = self->head->next;
    if (self->head == NULL)
    {
        self->tail = NULL;
    }
    *data = node->data;
    picofuse_event_t type = node->type;
    free(node);
    return type;
}

///////////////////////////////////////////////////////////////////////////////
// Fire an event immediately, returns -1 on error or 0 on success

int picofuse_fire(picofuse_t *self, picofuse_event_t type, void *data)
{
    struct picofuse_node_t *node = malloc(sizeof(struct picofuse_node_t));
    if (node == NULL)
    {
        return -1;
    }
    node->type = type;
    node->data = data;
    node->next = NULL;
    if (picofuse_is_empty(self))
    {
        self->head = node;
        self->tail = node;
    }
    else
    {
        self->tail->next = node;
        self->tail = node;
    }
    return 0;
}

inline int picofuse_fire_bool(picofuse_t * self,picofuse_event_t event,bool value) {
    return picofuse_fire(self, event, (void *)(value ? 1 : 0));
}

///////////////////////////////////////////////////////////////////////////////
// Call a handler and update state as needed

void picofuse_callback(picofuse_t *self, picofuse_event_t event, void *data)
{
    picofuse_callback_t *callback = hashmap_get(self->hashmap, self->state, event);
    if (callback == NULL)
    {
        callback = hashmap_get(self->hashmap, ANY, event);
    }
    if (callback != NULL)
    {
        picofuse_debug("picofuse_callback: event=%s callback\n", picofuse_event_str(event));
        picofuse_state_t state = callback(self, event, data);
        if (state != ANY && state != self->state)
        {
            picofuse_debug("picofuse_callback: event=%s state %d -> %d\n", picofuse_event_str(event), self->state, state);
            self->state = state;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Register an event handler for a given (state, event) pair. Returns -1 on
// error or 0 on success.

int picofuse_register(picofuse_t *self, picofuse_state_t state,
                      picofuse_event_t event, picofuse_callback_t *callback)
{
    // Register init handlers
    switch (event)
    {
    case EV_LED:
    case EV_LED_INIT:
        picofuse_fire(self, EV_LED_INIT, &picofuse_led_data);
        break;
    case EV_WIFI_INIT:
        picofuse_fire(self, EV_WIFI_INIT, &picofuse_wifi_data);
        break;
    }

    return hashmap_put(self->hashmap, state, event, (void *)(callback));
}

///////////////////////////////////////////////////////////////////////////////
// Return the current event

inline picofuse_event_t picofuse_event(picofuse_t *self)
{
    return self->event;
}

///////////////////////////////////////////////////////////////////////////////
// Return the current state

inline picofuse_state_t picofuse_state(picofuse_t *self)
{
    return self->state;
}

///////////////////////////////////////////////////////////////////////////////
// Free resources

void picofuse_free(picofuse_t *self)
{
    if (self)
    {
        while (!picofuse_is_empty(self))
        {
            void *data;
            picofuse_next(self, &data);
        }
        hashmap_free(self->hashmap);
        free(self);
    }
}

///////////////////////////////////////////////////////////////////////////////
// Main

int picofuse_main(picofuse_t *self)
{
    // Initialize STDIO
    stdio_init_all();

    // Wait for 1500ms
    sleep_ms(1500);

    // Debug
    picofuse_debug("picofuse_main: Starting run loop\n");

    int errorCode = 0;
    while (!errorCode)
    {
        if (picofuse_is_empty(self))
        {
            sleep_ms(10);
            continue;
        }

        // Get next event from the queue
        void *data;
        self->event = picofuse_next(self, &data);

        // Debug
        picofuse_debug("picofuse_main: state %d: %s\n", self->state, picofuse_event_str(self->event));

        // Process the event
        switch (self->event)
        {
        case EV_NONE:
            break;
        case EV_INIT:
            picofuse_handle_init(self, (picofuse_init_t *)data);
            break;
        case EV_QUIT:
            picofuse_handle_quit(self, (picofuse_init_t *)data);
            break;
        case EV_LED_INIT:
            picofuse_handle_led_init(self, (picofuse_led_t *)data);
            break;
        case EV_LED:
            picofuse_handle_led(self, &picofuse_led_data, (bool)data);
            break;
        case EV_TIMER_INIT:
            picofuse_handle_timer_init(self, (picofuse_timer_t *)data);
            break;
        case EV_TIMER:
            picofuse_handle_timer(self, (picofuse_timer_t *)data);
            break;
        case EV_WIFI_INIT:
            picofuse_handle_wifi_init(self, (picofuse_wifi_t *)data);
            break;
        default:
            picofuse_debug("picofuse_main: unhandled event=%s data=0x%08X\n", picofuse_event_str(self->event), data);
        }
    }

    // Debug
    picofuse_debug("picofuse_main: quit with errorCode=%d\n", errorCode);

    // Return error code
    return errorCode;
}
