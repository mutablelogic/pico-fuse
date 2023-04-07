#include <string.h>
#include <ctype.h>

#include <picofuse/picofuse.h>
#include <pico/unique_id.h>
#include <pico/stdlib.h>

#ifdef PICO_CYW43_SUPPORTED
#include <pico/cyw43_arch.h>
#endif

#include "ev.h"
#include "debug.h"

///////////////////////////////////////////////////////////////////////////////
// Get unique board identifier

#ifdef PICO_UNIQUE_BOARD_ID_SIZE_BYTES
static const char *get_unique_board_id()
{
    static char id[PICO_UNIQUE_BOARD_ID_SIZE_BYTES * 2 + 1];
    pico_get_unique_board_id_string(id, PICO_UNIQUE_BOARD_ID_SIZE_BYTES * 2 + 1);
    return id;
}
#else
static const char *get_unique_board_id()
{
    return NULL;
}
#endif

///////////////////////////////////////////////////////////////////////////////
// Handle the EVENT_INIT event

void picofuse_handle_init(picofuse_t *self, picofuse_init_t *data)
{
    // Set the deviceIdentifier
    data->deviceIdentifier = get_unique_board_id();

    // Call the registered event handler
    picofuse_callback(self, picofuse_event(self), (void *)(data));

    // appName
    if (data->appName != NULL)
    {
        picofuse_debug("picofuse_handle_init: appName=%s\n", data->appName);
    }

#ifdef PICO_CYW43_SUPPORTED
    // countryCode
    int errorCode;
    if (data->countryCode != NULL && strlen(data->countryCode) == 2)
    {
        picofuse_debug("picofuse_handle_init: cyw43_arch_init_with_country=%c%c\n", toupper(data->countryCode[0]), toupper(data->countryCode[1]));
        errorCode = cyw43_arch_init_with_country(CYW43_COUNTRY(toupper(data->countryCode[0]), toupper(data->countryCode[1]), 0));
    }
    else
    {
        picofuse_debug("picofuse_handle_init: cyw43_arch_init\n");
        errorCode = cyw43_arch_init();
    }

    // Check error code and bail out if there was an error
    if (errorCode)
    {
        picofuse_debug("picofuse_handle_init: cyw43_arch_init failed with error code %d", errorCode);
        data->errorCode = errorCode;
    }
#endif

}
