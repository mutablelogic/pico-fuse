#include <stdio.h>
#include <picofuse/picofuse.h>

#include "ev.h"

static char strbuf[40];

const char *picofuse_event_str(picofuse_event_t event)
{
    switch (event)
    {
    case EV_NONE:
        return "EV_NONE";
    case EV_INIT:
        return "EV_INIT";
    case EV_QUIT:
        return "EV_QUIT";
    case EV_LED:
        return "EV_LED";
    case EV_LED_INIT:
        return "EV_LED_INIT";
    case EV_TIMER_INIT:
        return "EV_TIMER_INIT";
    case EV_TIMER:
        return "EV_TIMER";
    case EV_WIFI_INIT:
        return "EV_WIFI_INIT";
    case EV_WIFI:
        return "EV_WIFI";
    case EV_GPIO_INIT:
        return "EV_GPIO_INIT";
    case EV_GPIO:
        return "EV_GPIO";
    default:
        snprintf(strbuf, 40, "EV_UNKNOWN(0x%08X)", event);
        return strbuf;
    }
}
