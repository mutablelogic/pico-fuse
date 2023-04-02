
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "wifi.h"
#include "mqtt.h"
#include "opts.h"

int main()
{
    stdio_init_all();
    sleep_ms(250);

    // WiFi
    int err = wifi_init(WIFI, WIFI_PASSWORD);
    if (err != PICO_OK)
    {
        printf("wifi init failed with error code=%d\n", err);
        return 0;
    }

    // Broker address
    ip_addr_t addr;
    if (!ip4addr_aton(MQTT, &addr))
    {
        printf("wifi init failed with error code=%d\n", err);
        return 0;
    }

    // Create a MQTT broker with no authentication
    mq *client = mq_init(addr, NULL, NULL, MQTT_SUB);
    if (client == NULL)
    {
        printf("mqtt init failed\n");
        return 0;
    }

    // Main loop
    while (true)
    {
        int err = mq_connect(client);
        switch (err)
        {
        case ERR_OK:
        case ERR_ISCONN:
            err = mq_publish(client, MQTT_PUB, "hello world");
            if(err) {
                printf("mqtt publish failed with error %d\n", err);
                sleep_ms(1000);
            }
            break;
        default:
            printf("mqtt connect failed with error %d\n", err);
            sleep_ms(1000);
        }

        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, !cyw43_arch_gpio_get(CYW43_WL_GPIO_LED_PIN));
        sleep_ms(250);
    }
}
