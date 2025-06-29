#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "led_strip.h"

static const char *TAG = "led-strip";

#define RGB_LED 8
static led_strip_t *pStrip_a;

char *data = 0xFF;

static void configure_led(void)
{
    pStrip_a = led_strip_init(0, RGB_LED, 1);
    pStrip_a->clear(pStrip_a, 50);
}

void app_main(void)
{
    configure_led();
    while (true)
    {
        ESP_LOGI(TAG, "LED RED");
        pStrip_a->set_pixel(pStrip_a, 0, 255, 0, 0);
        pStrip_a->refresh(pStrip_a, 100);
        vTaskDelay(pdMS_TO_TICKS(1000));

        ESP_LOGI(TAG, "LED GREEN");
        pStrip_a->set_pixel(pStrip_a, 0, 0, 255, 0);
        pStrip_a->refresh(pStrip_a, 100);
        vTaskDelay(pdMS_TO_TICKS(1000));

        ESP_LOGI(TAG, "LED BLUE");
        pStrip_a->set_pixel(pStrip_a, 0, 0, 0, 255);
        pStrip_a->refresh(pStrip_a, 100);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}