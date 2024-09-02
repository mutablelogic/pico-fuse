#include <stdint.h>
#include <fuse/fuse.h>
#if defined(TARGET_PICO)
#include <pico/stdlib.h>
#endif
#include "alloc.h"
#include "alloc_builtin.h"
#include "data.h"
#include "event.h"
#include "fuse.h"
#include "list.h"
#include "map.h"
#include "mutex.h"
#include "number.h"
#include "null.h"
#include "printf.h"
#include "string.h"
#include "timer.h"

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

fuse_t *fuse_new()
{
    // Create an allocator
    struct fuse_allocator *allocator = fuse_allocator_builtin_new();
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
        fuse->exit_code = FUSE_EXIT_SUCCESS;
    }

    // Retain the application so it isn't autoreleased
    fuse_allocator_retain(allocator, fuse);

    // Reset name fields
    for(size_t i = 0; i < FUSE_MAGIC_COUNT; i++)
    {
        fuse->desc[i].name = 0;
    }

    fuse->desc[FUSE_MAGIC_APP] = (struct fuse_value_desc){
        .size = 0,
        .name = "APP",
    };

    // Register types
    fuse_register_value_null(fuse);
    fuse_register_value_int(fuse);
    fuse_register_value_uint(fuse);
    fuse_register_value_bool(fuse);
    fuse_register_value_data(fuse);
    fuse_register_value_float(fuse);
    fuse_register_value_event(fuse);
    fuse_register_value_mutex(fuse);
    fuse_register_value_string(fuse);
    fuse_register_value_timer(fuse);
    fuse_register_value_list(fuse);

    // Create the event queue for Core 0 
    fuse->core0 = (struct fuse_list *)fuse_retain(fuse, (fuse_value_t* )fuse_new_list(fuse));
    if (fuse->core0 == NULL)
    {
        fuse_allocator_free(allocator, fuse);
        fuse_allocator_destroy(allocator);
        return NULL;
    }

    // TODO: Don't create an event queue for Core 1
    fuse->core1 = NULL;

    // Return the fuse application
    return fuse;
}

int fuse_destroy(fuse_t *fuse)
{
    assert(fuse);

    // Release event queues
    fuse_release(fuse, (fuse_value_t* )fuse->core0);
    fuse_release(fuse, (fuse_value_t* )fuse->core1);

    // Store the exit code
    int exit_code = fuse->exit_code;
    struct fuse_allocator *allocator = fuse->allocator;

    // Repeatedly drain the allocator pool until no new memory blocks are freed
    // It will call fuse_destroy_callback for each memory block that is freed,
    // releasing resources
    size_t drained = 0;
    do
    {
        drained = fuse_drain(fuse, 0);
    } while (drained > 0);

    // Free the application
    fuse_allocator_free(allocator, fuse);

    // Walk through any remaining memory blocks
#ifdef DEBUG
    struct fuse_allocator_header *hdr = allocator->head;
    size_t count = 0;
    while (hdr != NULL)
    {
        // Print any memory leaks
        fuse_debugf("LEAK: %p %s (%d bytes)", hdr->ptr, fuse->desc[hdr->magic].name, hdr->size);
        if (hdr->file != NULL)
        {
            fuse_debugf(" [allocated at %s:%d]", hdr->file, hdr->line);
        }
        fuse_debugf("\n");
        count++;
        hdr = hdr->next;
    }

    // If the count is greater than zero, then there are memory leaks
    if (count > 0)
    {
        exit_code = FUSE_EXIT_MEMORYLEAKS;
    }
#endif

    // Free the allocator
    fuse_allocator_destroy(allocator);

    // Return the exit code
    return exit_code == FUSE_EXIT_SUCCESS ? 0 : exit_code;
}

/** @brief Drain the memory allocation pool
 */
size_t fuse_drain(fuse_t *self, size_t cap)
{
    assert(self);

    size_t count = 0;
    struct fuse_allocator_header *hdr = self->allocator->head;
    while (hdr != NULL && (cap == 0 || count < cap))
    {
        struct fuse_allocator_header *next = hdr->next;
        if (hdr->ref == 0)
        {
            fuse_free(self, hdr->ptr);
            count++;
        }
        hdr = next;
    }
    return count;
}

void *fuse_alloc_ex(fuse_t *self, const uint16_t magic, const void *user_data, const char *file, const int line)
{
    assert(self);
    assert(magic < FUSE_MAGIC_COUNT);

    // Determine the size of the memory block
    size_t size = self->desc[magic].size;
    switch (magic)
    {
    case FUSE_MAGIC_DATA:
        size = (size_t)user_data;
        break;
    default:
        size = self->desc[magic].size;
        break;
    }

    // Allocate the memory
    void *ptr = fuse_allocator_malloc(self->allocator, size, magic, file, line);
    if (ptr == NULL)
    {
#ifdef DEBUG
        fuse_debugf("fuse_alloc_ex: %s: Could not allocate %lu bytes", self->desc[magic].size);
        if (file != NULL)
        {
            fuse_debugf(" [allocated at %s:%d]", file, line);
        }
        fuse_debugf("\n");
#endif
        return NULL;
    }

    // Initialise the value
    if (self->desc[magic].init)
    {
        if (!self->desc[magic].init((struct fuse_application *)self, ptr, user_data))
        {
#ifdef DEBUG
            fuse_debugf("fuse_alloc_ex: %s: initialise failed", self->desc[magic].name);
            if (file != NULL)
            {
                fuse_debugf(" [allocated at %s:%d]", file, line);
            }
            fuse_debugf("\n");
#endif
            fuse_allocator_free(self->allocator, ptr);
            return NULL;
        }
    }

    // Return success
    return ptr;
}

void fuse_free(fuse_t *self, void *ptr)
{
    assert(self);
    assert(ptr);

    // Get the magic number
    uint16_t magic = fuse_allocator_magic(self->allocator, (void *)ptr);
    assert(magic < FUSE_MAGIC_COUNT);

    // Destroy the value
    if (self->desc[magic].destroy)
    {
        self->desc[magic].destroy((struct fuse_application *)self, (void *)ptr);
    }

    // Free the memory
    fuse_allocator_free(self->allocator, ptr);
}

void fuse_run(fuse_t *self, int (*callback)(fuse_t *))
{
    assert(self);
    assert(callback);

#if defined(TARGET_PICO)
    // Initialize STDIO
    stdio_init_all();

    // Wait for 1500ms
    sleep_ms(1500);
#endif

    // Call the callback, and exit if it returns a non-zero value
    int exit_code = callback(self);
    if (exit_code)
    {
        fuse_exit(self, exit_code);
        return;
    }

    // Run the loop
    fuse_printf(self, "picofuse_main: run\n");
    while (!self->exit_code)
    {
        sleep_ms(100);
    }

#if defined(TARGET_PICO)
    fuse_printf(self, "picofuse_main: halt\n");
    while (1)
    {
        sleep_ms(1000);
    }
#endif
}

inline void fuse_exit(fuse_t *self, int exit_code)
{
    assert(self);
    self->exit_code = exit_code ? exit_code : FUSE_EXIT_SUCCESS;
}
