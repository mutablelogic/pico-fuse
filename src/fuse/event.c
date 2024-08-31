#include <fuse/fuse.h>

//////////////////////////////////////////////////////////////////////////////
// DECLARATIONS

typedef struct event_context fuse_event_t;

struct event_context
{
    uint8_t type;
    fuse_value_t *source;
    void *user_data;
};

// Define the magic numbers for the values
#define FUSE_EVENT_NULL  0x00  ///< NULL value
#define FUSE_EVENT_TIMER 0x01 ///< Timer fired event

// Maximum number of event types
#define FUSE_EVENT_COUNT 0x02 ///< Maximum number of event types

/** @brief Place a new event on the event queue
 *
 * An event is created and placed on the event queues for the application. The event is owned by the
 * event queue and will be released when the event is removed from the queues.
 *
 * @param self The fuse instance
 * @param source The source of the event
 * @param type The event type
 * @param user_data User data associated with the event, if any
 * @return The event is returned, or NULL if the event could not be created
 */
fuse_event_t* fuse_new_event_ex(fuse_t *self, fuse_value_t* source, uint8_t type, void *user_data, const char* file, const int line);
