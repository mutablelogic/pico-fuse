#include <ctype.h>
#include <stdbool.h>

#include <fuse/fuse.h>
#include <picofuse/picofuse.h>
#include <pico/types.h> // for absolute_time_t

#ifdef PICO_CYW43_SUPPORTED
#include <pico/cyw43_arch.h>
#endif

///////////////////////////////////////////////////////////////////////////////
// TYPES

struct wifi_context
{
    fuse_wifi_mode_t mode;
    absolute_time_t t;
    bool scanning;
};

void *fuse_wifi_new(fuse_t *fuse, void *userdata);
void fuse_wifi_destroy(fuse_t *fuse, void *context);

///////////////////////////////////////////////////////////////////////////////
// GLOBALS

fuse_device_t wifi = {
    .name = "wifi",
    .init = fuse_wifi_new,
    .destroy = fuse_wifi_destroy,
};

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

void *fuse_wifi_new(fuse_t *fuse, void *userdata)
{
    assert(fuse);
    assert(userdata);

#ifndef PICO_CYW43_SUPPORTED
    fuse_debugf("fuse_wifi_new: unsupported on this board\n");
    return NULL;
#else
    // Initialize the CYW43
    fuse_wifi_userdata_t *params = (fuse_wifi_userdata_t *)userdata;
    uint32_t cc = CYW43_COUNTRY_WORLDWIDE;
    if (params->country_code != NULL && strlen(params->country_code) == 2)
    {
        cc = CYW43_COUNTRY(toupper(params->country_code[0]), toupper(params->country_code[1]), 0);
    }

    err_t err = cyw43_arch_init_with_country(cc);
    if (err != ERR_OK)
    {
        fuse_debugf("fuse_wifi_new: cyw43_arch_init failed: code %d\n", err);
        return NULL;
    }
    else
    {
        fuse_debugf("fuse_wifi_new: countrycode=%c%c\n", cc & 0xff, cc >> 8);
    }

    // Create the context
    struct wifi_context *context = fuse_alloc(fuse, sizeof(struct wifi_context), FUSE_MAGIC_WIFI);
    if (context == NULL)
    {
        return NULL;
    }

    // Initialize the context
    context->mode = params->mode;
    context->t = 0;
    context->scanning = false;

    // Enable the wifi mode
    if (params->mode & FUSE_WIFI_STATION)
    {
        fuse_debugf("fuse_wifi_new: cyw43_arch_enable_sta_mode\n");
        cyw43_arch_enable_sta_mode();
    }
    else
    {
        fuse_debugf("fuse_wifi_new: cyw43_arch_disable_sta_mode\n");
        cyw43_arch_disable_sta_mode();
    }

    /*
    if (mode & FUSE_WIFI_ACCESSPOINT)
    {
        cyw43_arch_enable_ap_mode();
    }
    else
    {
        cyw43_arch_disable_ap_mode();
    }
    */
    return context;
#endif
}

void fuse_wifi_destroy(fuse_t *fuse, void *context)
{
    assert(fuse);
    assert(context);

    // TODO: Wait until scanning is false
#ifdef PICO_CYW43_SUPPORTED
    fuse_debugf("fuse_wifi_destroy\n");
    cyw43_arch_deinit();
#endif
    fuse_free(fuse, context);
}

/*
int fuse_wifi_scan_result(void *env, const cyw43_ev_scan_result_t *result)
{
    if (result)
    {
        fuse_debugf("ssid: %-32s rssi: %4d chan: %3d mac: %02x:%02x:%02x:%02x:%02x:%02x sec: %u\n",
                    result->ssid, result->rssi, result->channel,
                    result->bssid[0], result->bssid[1], result->bssid[2], result->bssid[3], result->bssid[4], result->bssid[5],
                    result->auth_mode);
    }
    return 0;
}
#endif

fuse_wifi_scan_context *fuse_wifi_scan()
{
    // Initialize the scan context
    if (wifi_scan_context == NULL)
    {
        wifi_scan_context = fuse_wifi_scan_new();
        assert(wifi_scan_context);
    }

#ifdef PICO_CYW43_SUPPORTED
    if (absolute_time_diff_us(get_absolute_time(), wifi_scan_context->t) < 0)
    {
        if (!wifi_scan_context->scanning)
        {
            cyw43_wifi_scan_options_t scan_options = {0};
            int err = cyw43_wifi_scan(&cyw43_state, &scan_options, NULL, fuse_wifi_scan_result);
            if (err == 0)
            {
                fuse_debugf("fuse_wifi_scan: scanning\n");
                wifi_scan_context->scanning = true;
            }
            else
            {
                // Wait 10s before starting another scan
                wifi_scan_context->t = make_timeout_time_ms(10000);
            }
        }
        else if (!cyw43_wifi_scan_active(&cyw43_state))
        {
            // Wait 10s before starting another scan
            wifi_scan_context->t = make_timeout_time_ms(10000);
            wifi_scan_context->scanning = false;
        }
    }
#endif

    // Return the scan context
    return wifi_scan_context;
}
*/
