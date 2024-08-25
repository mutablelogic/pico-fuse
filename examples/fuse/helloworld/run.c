#include <fuse/fuse.h>

int run(fuse_t *fuse)
{
    // Make and free a NULL value
    fuse_value_t *value = fuse_alloc(fuse, FUSE_MAGIC_NULL, NULL);
    fuse_debugf("value=%p\n", value);
    fuse_free(fuse, value);

    // Make and free a DATA value
    value = fuse_alloc(fuse, FUSE_MAGIC_DATA, NULL);
    fuse_debugf("value=%p\n", value);
    fuse_free(fuse, value);

    // Make and free a U8 value
    value = fuse_alloc(fuse, FUSE_MAGIC_U8, (void *)0xFF);
    fuse_debugf("value=%p\n", value);
    fuse_free(fuse, value);

    // Make and free a U16 value
    value = fuse_alloc(fuse, FUSE_MAGIC_U16, (void *)0xFFFF);
    fuse_debugf("value=%p\n", value);
    fuse_free(fuse, value);

    // Make and free a U32 value
    value = fuse_alloc(fuse, FUSE_MAGIC_U32, (void *)0xFFFFFFFF);
    fuse_debugf("value=%p\n", value);
    fuse_free(fuse, value);

    // Make and free a U64 value
    value = fuse_alloc(fuse, FUSE_MAGIC_U64, (void *)0xFFFFFFFFFFFFFFFF);
    fuse_debugf("value=%p\n", value);
    fuse_free(fuse, value);

    // Make and free a S8 value
    value = fuse_alloc(fuse, FUSE_MAGIC_S8, (void *)0x7F);
    fuse_debugf("value=%p\n", value);
    fuse_free(fuse, value);

    // Make and free a S16 value
    value = fuse_alloc(fuse, FUSE_MAGIC_S16, (void *)0x7FFF);
    fuse_debugf("value=%p\n", value);
    fuse_free(fuse, value);

    // Make a list
    fuse_value_t* list = fuse_alloc(fuse, FUSE_MAGIC_LIST, 0);
    fuse_debugf("value=%p\n", value);

    // Add a value
    //fuse_append(fuse, list, fuse_alloc(fuse, FUSE_MAGIC_U8, (void *)0xFF));

    // Free the list
    fuse_free(fuse, list);

    return 0;
}
