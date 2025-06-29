#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include <esp_sleep.h> ///////////////////////////

#define LED_1 2
#define LED_2 3
#define LED_3 7
#define wake_up 4   ///////////////////////////

static const char *TAG = "Sleep Mode";

static esp_err_t init_led(void)
{
    gpio_reset_pin(LED_1);
    gpio_reset_pin(LED_2);
    gpio_reset_pin(LED_3);

    gpio_set_direction(LED_1, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_2, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_3, GPIO_MODE_OUTPUT);

    gpio_set_direction(wake_up, GPIO_MODE_INPUT);  ///////////////////////////
    gpio_pulldown_dis(wake_up);                    ///////////////////////////
    gpio_pullup_en(wake_up);                       ///////////////////////////
    return ESP_OK;
}

void run_leds(void *pvParameters)
{
    int count = 0;
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(500));
        count = count + 1;

        if (count > 4)
        {
            count = 0;
        }
        ESP_LOGI(TAG, "count: %i", count);

        switch (count)
        {
        case 0:
            gpio_set_level(LED_1, 0);
            gpio_set_level(LED_2, 0);
            gpio_set_level(LED_3, 0);
            break;
        case 1:
            gpio_set_level(LED_1, 1);
            gpio_set_level(LED_2, 0);
            gpio_set_level(LED_3, 0);
            break;
        case 2:
            gpio_set_level(LED_1, 1);
            gpio_set_level(LED_2, 1);
            gpio_set_level(LED_3, 0);
            break;
        case 3:
            gpio_set_level(LED_1, 1);
            gpio_set_level(LED_2, 1);
            gpio_set_level(LED_3, 1);
            break;
        case 4:
            gpio_set_level(LED_1, 1);
            gpio_set_level(LED_2, 1);
            gpio_set_level(LED_3, 1);

            break;

        default:
            break;
        }
    }
}

void app_main(void)
{
    init_led();

    xTaskCreate(run_leds,
                "run_leds",
                2048,
                NULL,
                3,
                NULL);

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(7000));        

        gpio_wakeup_enable(wake_up, GPIO_INTR_LOW_LEVEL);
        esp_sleep_enable_gpio_wakeup();        
        esp_light_sleep_start();
		
		esp_deep_sleep_enable_gpio_wakeup((1ULL << wake_up), ESP_GPIO_WAKEUP_GPIO_LOW);         
        esp_deep_sleep_start();
        
    }
}
