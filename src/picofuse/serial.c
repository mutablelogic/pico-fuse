#include <picofuse/picofuse.h>
#include <pico/unique_id.h>

///////////////////////////////////////////////////////////////////////////////
// GLOBALS

char unique_id[2 * PICO_UNIQUE_BOARD_ID_SIZE_BYTES + 1] = {0};

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

const char *fuse_get_serial_number()
{
    if (unique_id[0] == 0)
    {
        pico_get_unique_board_id_string(unique_id, sizeof(unique_id));
    }
    return unique_id;
}
