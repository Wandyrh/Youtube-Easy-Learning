#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#define led1 2

uint8_t led_level = 0;
uint8_t count = 0;
static const char* tag = "Main";

esp_err_t init_led(void);
esp_err_t blink_led(void);


void app_main(void)
{
    init_led();

    while (1)
    {
        vTaskDelay(200 / portTICK_PERIOD_MS);
        blink_led();

        count +=1;
        if(count > 30)
        {
            count=0;
        }
        if(count < 10)
        {
            ESP_LOGI(tag, "Value: %u.", count );
        }
        if(count >= 10 && count < 20)
        {
            ESP_LOGW(tag, "Value: %u.", count );
        }
        if(count >= 20)
        {
            ESP_LOGE(tag, "Value: %u.", count );
        }       
    }    
    
}

esp_err_t init_led(void)
{
    gpio_reset_pin(led1);
    gpio_set_direction(led1, GPIO_MODE_OUTPUT );
    return ESP_OK;
}
esp_err_t blink_led(void)
{
    led_level = !led_level;
    gpio_set_level(led1,led_level );
    return ESP_OK;
}
