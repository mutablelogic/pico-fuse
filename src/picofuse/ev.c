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
    default:
        snprintf(strbuf, 40, "EV_UNKNOWN(%02X)", event);
        return strbuf;
    }
}
