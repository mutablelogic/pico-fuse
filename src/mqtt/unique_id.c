
#include <pico/unique_id.h>
#include "unique_id.h"

const char* get_unique_board_id() {
    static char id[PICO_UNIQUE_BOARD_ID_SIZE_BYTES*2 + 1];
    pico_get_unique_board_id_string(id, PICO_UNIQUE_BOARD_ID_SIZE_BYTES*2+1);
    return id;
}
