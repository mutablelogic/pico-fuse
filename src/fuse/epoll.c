#if TARGET == darwin
// Noop
#elif TARGET == pico
// Noop
#elif TARGET == linux

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

#include <sys/epoll.h>
#include <sys/errno.h>
#include <string.h>
#include <fuse/fuse.h>
#include <unistd.h>

// Private includes
#include "epoll.h"

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

struct fuse_epoll_instance *fuse_epoll_new(fuse_t *fuse)
{
    assert(fuse);

    // Initialize the epoll file descriptor
    int fd = epoll_create(0);
    if(fd < 0) {
        fuse_debugf(fuse,"fuse_epoll_new: %s",strerror(errno));        
        return NULL;
    }

    // Allocate memory for epolling
    struct fuse_epoll_instance *epoll = fuse_alloc(fuse, sizeof(struct fuse_epoll_instance));
    if (epoll == NULL)
    {
        return NULL;
    }

    // Fill in the instance members
    epoll->fd = fd;

    // Retrun success
    return epoll;
}

void fuse_epoll_destroy(fuse_t *fuse, struct fuse_epoll_instance *epoll)
{
    assert(fuse);
    assert(epoll);

    // TODO: Free all the events being watched

    // Close the file descriptor
    assert(epoll->fd);
    if(close(epoll->fd) != 0) {
        fuse_debugf(fuse,"fuse_epoll_destroy: %s",strerror(errno));     
    }

    // Free epoll
    fuse_free(fuse, epoll);
}

#endif // TARGET == linux
