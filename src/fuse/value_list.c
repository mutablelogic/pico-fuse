#include <stdint.h>

// Includes
#include <fuse/fuse.h>
#include "value.h"
#include "fuse.h"

#include <fuse/fuse.h>

void fuse_value_append(fuse_t *self, fuse_value_t *value, fuse_value_t *element) {
    assert(self);
    assert(value);
    assert(element);
    assert(fuse_allocator_magic(self->allocator, value) == FUSE_MAGIC_LIST);

    // TODO: Do not allow circular references - if element is list or map, then
    // ensure that it does not contain value

    // Retain the element, increment the count
    fuse_value_retain(self, element);
    value->data.count++;

    // Link element
    element->l.prev = value->r.tail;
    if(element->l.prev != NULL) {
        // If element prev is not NULL, then set element prev next to element
        element->l.prev->r.next = element;
    }

    // Set head and tail
    if (value->l.head == NULL) {
        assert(value->r.tail == NULL);
        value->l.head = element;
        value->r.tail = element;
    } else {
        assert(value->r.tail != NULL);
        value->r.tail = element;
    }
}

void fuse_value_prepend(fuse_t *self, fuse_value_t *value, fuse_value_t *element) {
    assert(self);
    assert(value);
    assert(element);
    assert(fuse_allocator_magic(self->allocator, value) == FUSE_MAGIC_LIST);

    // TODO: Do not allow circular references - if element is list or map, then
    // ensure that it does not contain value

    // Retain the element, increment the count
    fuse_value_retain(self, element);
    value->data.count++;

    // TODO

}

