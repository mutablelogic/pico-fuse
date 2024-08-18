/** @file main.h
 *  @brief Function prototypes for the main picofuse event loop
 */
#ifndef PICOFUSE_MAIN_H
#define PICOFUSE_MAIN_H

#include <fuse/fuse.h>

/** @brief Call the main function
 *
 *  @param flags The flags to pass to the fuse application
 *  @param main The main function to call, which should return 0 on success
 */
int fuse_main(int main(fuse_t *fuse));

#endif
