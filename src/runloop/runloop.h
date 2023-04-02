
#ifndef RUNLOOP_H
#define RUNLOOP_H

// Opaque runloop structure
typedef struct runloop_instance_t* runloop_t;

// Initialize the runloop structure
extern runloop_t* runloop_init();

// Free runloop structure
extern void runloop_free(runloop_t* runloop);

// Run the loop forever
extern void runloop_run(runloop_t* runloop);

// Determine if list is empty
extern bool runloop_is_empty(runloop_t *runloop);

// Push an event onto the runloop, returns -1 on error or 0 on success
extern int runloop_push(runloop_t *runloop, rl_event_type type, void *data);

// Pop an event from the runloop, returns NULL on error or the data otherwise
extern void *runloop_pop(runloop_t *runloop, rl_event_type *type);

#endif
