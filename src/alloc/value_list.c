#include <stdint.h>
#include <alloc/fuse.h>
#include "value.h"
#include "fuse.h"

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

    // Append the element to the list
    // Set head
    if (value->next == NULL) {
        value->next = element; 
    }
    


}
