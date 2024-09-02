#include <fuse/fuse.h>

int run(fuse_t *fuse)
{
    // Make and free a NULL value
    fuse_value_t *value = fuse_alloc(fuse, FUSE_MAGIC_NULL, NULL);
    fuse_printf(fuse, "null value=%q\n", value);
    fuse_free(fuse, value);

    // Make and free a DATA value
    value = fuse_alloc(fuse, FUSE_MAGIC_DATA, NULL);
    fuse_printf(fuse, "data value=%q\n", value);
    fuse_free(fuse, value);

    // Make and free a U8 value
    value = fuse_alloc(fuse, FUSE_MAGIC_U8, (void *)0xFF);
    fuse_printf(fuse, "u8 value=%q\n", value);
    fuse_free(fuse, value);

    // Make and free a U16 value
    value = fuse_alloc(fuse, FUSE_MAGIC_U16, (void *)0xFFFF);
    fuse_printf(fuse, "u16 value=%q\n", value);
    fuse_free(fuse, value);

    // Make and free a U32 value
    value = fuse_alloc(fuse, FUSE_MAGIC_U32, (void *)0xFFFFFFFF);
    fuse_printf(fuse, "u32 value=%q\n", value);
    fuse_free(fuse, value);

    // Make and free a U64 value
    value = fuse_alloc(fuse, FUSE_MAGIC_U64, (void *)0xFFFFFFFFFFFFFFFF);
    fuse_printf(fuse, "u64 value=%q\n", value);
    fuse_free(fuse, value);

    // Make and free a S8 value
    value = fuse_alloc(fuse, FUSE_MAGIC_S8, (void *)0x7F);
    fuse_printf(fuse, "s8 value=%v\n", value);
    fuse_free(fuse, value);

    // Make and free a S16 value
    value = fuse_alloc(fuse, FUSE_MAGIC_S16, (void *)0x7FFF);
    fuse_printf(fuse, "s16 value=%q\n", value);
    fuse_free(fuse, value);

    // Make a list
    fuse_value_t *list = fuse_alloc(fuse, FUSE_MAGIC_LIST, 0);
    fuse_printf(fuse, "list value=%q\n", value);

    // Free the list
    fuse_free(fuse, list);

    return 0;
}
