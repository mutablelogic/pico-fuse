/** @file epoll.h
 *  @brief Private function prototypes and structure definitions for event polling
 */
#ifndef FUSE_PRIVATE_EPOLL_H
#define FUSE_PRIVATE_EPOLL_H

#include <fuse/fuse.h>

/** @brief Represents an instance of an event poller
 */
struct fuse_epoll_instance
{
    int fd; ///< File descriptor for epoll
};

/** @brief Create a new event polling instance
 *
 *  @param fuse The fuse instance
 *  @return The new event polling instance, or NULL on error
 */
struct fuse_epoll_instance *fuse_epoll_new(fuse_t *fuse);

/** @brief Create a new event polling instance
 *
 *  @param fuse The fuse instance
 *  @param epoll The epolling instance
 */
void fuse_epoll_destroy(fuse_t *fuse, struct fuse_epoll_instance *epoll);

#endif
