#include <fuse/fuse.h>
#include "fuse.h"
#include "alloc.h"
#include "list.h"
#include "printf.h"

////////////////////////////////////////////////////////////////////////////////
// DECLARATIONS

static bool fuse_init_list(fuse_t *self, fuse_value_t *list, const void *user_data);
static void fuse_destroy_list(fuse_t *self, fuse_value_t *list);
static size_t fuse_str_list(fuse_t *self, char *buf, size_t sz, size_t i, fuse_value_t *list, bool json);

////////////////////////////////////////////////////////////////////////////////
// LIFECYCLE

/** @brief Register type for a list value
 */
void fuse_register_value_list(fuse_t *self)
{
    assert(self);

    fuse_value_desc_t fuse_list_type = {
        .size = sizeof(struct fuse_list),
        .name = "LIST",
        .init = fuse_init_list,
        .destroy = fuse_destroy_list,
        .str = fuse_str_list,
    };

    fuse_register_value_type(self, FUSE_MAGIC_LIST, fuse_list_type);
}

////////////////////////////////////////////////////////////////////////////////
// PRIVATE METHODS

/* @brief Initialise a list
 */
static bool fuse_init_list(fuse_t *self, fuse_value_t *list, const void *user_data)
{
    assert(self);
    assert(list);
    assert(self->allocator->magic(self->allocator, list) == FUSE_MAGIC_LIST);

    // Set the count to 0
    ((fuse_list_t* )list)->count = 0;

    // Return success
    return true;
}

/* @brief Destroy the list
 */
static void fuse_destroy_list(fuse_t *self, fuse_value_t *list)
{
    assert(self);
    assert(list);
    assert(self->allocator->magic(self->allocator, list) == FUSE_MAGIC_LIST);

    fuse_value_t *elem = fuse_list_next(self, (fuse_list_t* )list, NULL);
    while (elem != NULL)
    {
        // Get the next element
        fuse_value_t *tmp = fuse_list_next(self, (fuse_list_t* )list, elem);

        // Release the value
        fuse_release(self, elem);

        // Move to the next element
        elem = tmp;
    }
}

static inline fuse_value_t *fuse_get_head(fuse_t *self, fuse_value_t *list)
{
    assert(self);
    assert(list);

    // Return the head
    fuse_value_t **ptr = (fuse_value_t **)self->allocator->headptr(list);
    assert(ptr);
    return *ptr;
}

static inline void fuse_set_head(fuse_t *self, fuse_value_t *list, fuse_value_t *elem)
{
    assert(self);
    assert(list);

    // Set the head
    fuse_value_t **ptr = (fuse_value_t **)self->allocator->headptr(list);
    assert(ptr);
    *ptr = elem;
}

static inline fuse_value_t *fuse_get_tail(fuse_t *self, fuse_value_t *list)
{
    assert(self);
    assert(list);

    // Return the tail
    fuse_value_t **ptr = (fuse_value_t **)self->allocator->tailptr(list);
    assert(ptr);
    return *ptr;
}

static inline void fuse_set_tail(fuse_t *self, fuse_value_t *list, fuse_value_t *elem)
{
    assert(self);
    assert(list);

    // Set the tail
    fuse_value_t **ptr = (fuse_value_t **)self->allocator->tailptr(list);
    assert(ptr);
    *ptr = elem;
}

static size_t fuse_str_list(fuse_t *self, char *buf, size_t sz, size_t i, fuse_value_t *list, bool json)
{
    assert(self);
    assert(buf == NULL || sz > 0);
    assert(list);

    // Add prefix
    i = chtostr_internal(buf, sz, i, '[');

    fuse_value_t *elem = fuse_list_next(self, (fuse_list_t* )list, NULL);
    while (elem != NULL)
    {
        // Append quoted string
        i = vtostr_internal(self, buf, sz, i, elem, true);

        // Get the next element
        elem = fuse_list_next(self, (fuse_list_t* )list, elem);

        // Add separator
        if (elem != NULL)
        {
            i = chtostr_internal(buf, sz, i, ',');
        }
    }

    // Add suffix
    i = chtostr_internal(buf, sz, i, ']');

    // Return the index
    return i;
}

////////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

/* @brief Append an element to the end of a list and return it
 */
fuse_value_t *fuse_list_append(fuse_t *self, fuse_list_t *list, fuse_value_t *elem)
{
    assert(self);
    assert(list);
    assert(elem);
    assert(fuse_get_head(self, elem) == NULL);
    assert(fuse_get_tail(self, elem) == NULL);
    assert(self->allocator->magic(self->allocator, list) == FUSE_MAGIC_LIST);

    // Retain the element, return NULL if the retain failed
    elem = fuse_retain(self, elem);
    if (elem == NULL)
    {
        return NULL;
    }

    // Link into the list
    fuse_value_t *head = fuse_get_head(self, (fuse_value_t* )list);
    fuse_value_t *tail = fuse_get_tail(self, (fuse_value_t* )list);
    if (head == NULL)
    {
        fuse_set_head(self,  (fuse_value_t* )list, elem);
    }
    if (tail != NULL)
    {
        fuse_set_tail(self, tail, elem);
    }
    fuse_set_tail(self,  (fuse_value_t* )list, elem);
    fuse_set_head(self, elem, tail);
    fuse_set_tail(self, elem, NULL);

    // Increment the list count
    ((struct fuse_list *)list)->count++;

    // Return the element
    return elem;
}

/** @brief Return the number of elements in the list or map
 */
inline size_t fuse_list_count(fuse_t *self, fuse_list_t *value)
{
    assert(self);
    assert(value);
    assert(self->allocator->magic(self->allocator, value) == FUSE_MAGIC_LIST);

    return ((struct fuse_list *)value)->count;
}

/** @brief Return the next list element
 */
inline fuse_value_t *fuse_list_next(fuse_t *self, fuse_list_t *list, fuse_value_t *elem)
{
    assert(self);
    assert(list);
    assert(self->allocator->magic(self->allocator, list) == FUSE_MAGIC_LIST);

    return (elem == NULL) ? fuse_get_head(self,  (fuse_value_t* )list) : fuse_get_tail(self, elem);
}

/** @brief Remove an element from the end of the list and return it
 */
fuse_value_t *fuse_list_pop(fuse_t *self, fuse_list_t *list)
{
    assert(self);
    assert(list);
    assert(self->allocator->magic(self->allocator, list) == FUSE_MAGIC_LIST);

    // If tail is NULL, then the list is empty
    fuse_value_t *tail = fuse_get_tail(self,  (fuse_value_t* )list);
    if (tail == NULL)
    {
        assert(((struct fuse_list *)list)->count == 0);
        return NULL;
    }

    // Get the previous element
    fuse_value_t *prev = fuse_get_head(self, tail);

    // Update the tail pointer of the list
    if (prev != NULL)
    {
        fuse_set_tail(self,  (fuse_value_t* )list, prev);
        fuse_set_tail(self, prev, NULL);
    }
    else
    {
        // This was the only element in the list
        fuse_set_head(self,  (fuse_value_t* )list, NULL);
        fuse_set_tail(self,  (fuse_value_t* )list, NULL);
    }

    // Decrement the list count
    ((struct fuse_list *)list)->count--;

    // Unlink the element and return it
    fuse_set_head(self, tail, NULL);
    fuse_set_tail(self, tail, NULL);

    // Release the element
    fuse_release(self, tail);

    return tail;
}

/** @brief Append an element to the beginning of a list
 */
fuse_value_t *fuse_list_push(fuse_t *self, fuse_list_t *list, fuse_value_t *elem)
{
    assert(self);
    assert(list);
    assert(elem);
    assert(fuse_get_head(self, elem) == NULL);
    assert(fuse_get_tail(self, elem) == NULL);
    assert(self->allocator->magic(self->allocator, list) == FUSE_MAGIC_LIST);

    // Retain the element, return NULL if the retain failed
    elem = fuse_retain(self, elem);
    if (elem == NULL)
    {
        return NULL;
    }

    // Link into the list
    fuse_value_t *head = fuse_get_head(self,  (fuse_value_t* )list);
    if (head != NULL)
    {
        fuse_set_head(self, head, elem);
    }
    fuse_set_head(self,  (fuse_value_t* )list, elem);
    fuse_set_tail(self, elem, head);
    fuse_set_head(self, elem, NULL);

    // If there is no tail, set element as the new tail
    if (fuse_get_tail(self,  (fuse_value_t* )list) == NULL)
    {
        fuse_set_tail(self, (fuse_value_t* ) list, elem);
    }

    // Increment the list count
    ((struct fuse_list *)list)->count++;

    // Return the element
    return elem;
}
