#include <stdint.h>
#include <alloc/fuse.h>
#include "fuse.h"

fuse_t *fuse_new()
{
    // Create an allocator
    fuse_allocator_t *allocator = fuse_allocator_builtin_new();
    if (allocator == NULL)
    {
        return NULL;
    }

    // Allocate a fuse application
    fuse_t *fuse = fuse_allocator_malloc(allocator, sizeof(fuse_t), (uint16_t)(FUSE_MAGIC_APP), NULL, 0);
    if (fuse == NULL)
    {
        fuse_allocator_destroy(allocator);
        return NULL;
    }
    else
    {
        fuse->allocator = allocator;
        fuse->exit_code = 0;
    }

    // Return the fuse application
    return fuse;
}

void fuse_destroy_callback(void *ptr, size_t size, uint16_t magic, const char *file, int line, void *user)
{
    // Increment the count
    (*((uint32_t *)user))++;

    // Print any errors
    fuse_debugf("LEAK: %p %s (%d bytes)", ptr, fuse_magic_cstr(magic),size);
    if (file != NULL)
    {
        fuse_debugf(" [allocated at %s:%d]", file, line);
    }
    fuse_debugf("\n");
}

int fuse_destroy(fuse_t *fuse)
{
    assert(fuse);

    // Store the exit code and allocator object
    int exit_code = fuse->exit_code;
    fuse_allocator_t *allocator = fuse->allocator;

    // Free the application
    fuse_allocator_free(allocator, fuse);

    // Walk through any remaining memory blocks
#ifdef DEBUG
    void *ctx = NULL;
    uint32_t count = 0;
    while ((ctx = fuse_allocator_walk(allocator, ctx, fuse_destroy_callback, &count)) != NULL)
    {
        // Do nothing
    }
    // If the count is greater than zero, then there are memory leaks
    if(count > 0)
    {
        exit_code = -1;
    }
#endif
    
    // Free the allocator
    fuse_allocator_destroy(allocator);

    // Return the exit code
    return exit_code;
}

void *fuse_alloc_ex(fuse_t *self, size_t size, uint16_t magic, const char *file, int line)
{
    assert(self);
    void *ptr = fuse_allocator_malloc(self->allocator, size, magic, file, line);
    if (ptr == NULL)
    {
        // TODO: Report error in debugging
        return NULL;
    }
    return ptr;
}

void fuse_free(fuse_t *self, void *ptr)
{
    assert(self);
    fuse_allocator_free(self->allocator, ptr);
}
