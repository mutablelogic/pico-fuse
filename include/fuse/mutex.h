/** @file mutex.h
 *  @brief Mutually exclusive lock
 *
 *  This file contains methods for mutex locks
 */
#ifndef FUSE_MUTEX_H
#define FUSE_MUTEX_H

/* @brief Mutex lock
 */
typedef struct fuse_mutex fuse_mutex_t;

#ifdef DEBUG
#define fuse_new_mutex(self) \
    ((fuse_mutex_t* )fuse_new_value_ex((self), (FUSE_MAGIC_MUTEX), (0), __FILE__, __LINE__))
#else
#define fuse_new_mutex(self) \
    ((fuse_mutex_t* )fuse_new_value_ex((self), (FUSE_MAGIC_MUTEX), (0), 0, 0))
#endif

/* @brief Lock critical section of code
 *
 * Block until the mutex lock is acquired, then lock the critical section of code
 * 
 * @param mutex The mutex lock
 */
void fuse_mutex_lock(fuse_mutex_t* mutex);

/* @brief Unlock critical section of code
 *
 * Unlock the critical section of code
 *
 * @param mutex The mutex lock
 */
void fuse_mutex_unlock(fuse_mutex_t* mutex);

#endif /* FUSE_MUTEX_H */
