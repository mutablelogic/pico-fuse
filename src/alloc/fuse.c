#include <stdint.h>
#include <alloc/fuse.h>
#include "fuse.h"

fuse_t *fuse_new() {
    // Create an allocator
    fuse_allocator_t *allocator = fuse_allocator_builtin_new();
    if (allocator == NULL) {
        return NULL;
    }

    // Allocate a fuse application
    fuse_t *fuse = fuse_allocator_malloc(allocator, sizeof(fuse_t), (uint16_t)(FUSE_MAGIC_APP), NULL,0);
    if (fuse == NULL) {
        fuse_allocator_destroy(allocator);
        return NULL;
    }

    // Return the fuse application
    return fuse;
}

int fuse_destroy(fuse_t *fuse) {
    assert(fuse);

    // Store the exit code
    int exit_code = fuse->exit_code;

    // Free the application
    fuse_allocator_free(fuse->allocator, fuse);

    // TODO: Walk through any remaining memory blocks

    // Free the allocator
    fuse_allocator_destroy(fuse->allocator);

    // Return the exit code
    return exit_code;
}

void *fuse_alloc_ex(fuse_t *self, size_t size, const char *file, int line) {
    assert(self);
    void *ptr = fuse_allocator_malloc(self->allocator, size, (uint16_t)(FUSE_MAGIC_BLOCK), file, line);
    if (ptr == NULL) {
        // TODO: Report error in debugging
        return NULL;
    }
    return ptr;
}

void fuse_free(fuse_t *self, void *ptr) {
    assert(self);
    fuse_allocator_free(self->allocator, ptr);
}
