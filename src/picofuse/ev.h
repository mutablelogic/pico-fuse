
#ifndef PICOFUSE_EV_H
#define PICOFUSE_EV_H

extern void picofuse_callback(picofuse_t *, picofuse_event_t, void *);
extern void picofuse_handle_init(picofuse_t *, picofuse_init_t *);
extern void picofuse_handle_quit(picofuse_t *, picofuse_init_t *);

#endif
