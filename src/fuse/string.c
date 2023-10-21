#include <fuse/fuse.h>
#include <string.h>
   
// Private includes
#include "string.h"

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

fuse_str_t *fuse_str_new_ex(fuse_t *fuse, const char *str, const char *file, int line) {
    assert(fuse);

    // Calculate size of string
    size_t size = 0;
    if(str != NULL) {
        size = strlen(str)
    }

    // TODO
}

fuse_str_t *fuse_str_destroy(fuse_t *fuse, fuse_str_t *str) {

}
