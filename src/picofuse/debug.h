
#ifndef PICOFUSE_DEBUG_H
#define PICOFUSE_DEBUG_H
#include <stdio.h>
#define picofuse_debug(...) fprintf(stderr, __VA_ARGS__)
#endif
