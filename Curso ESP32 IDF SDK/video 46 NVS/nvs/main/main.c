#include <stdio.h>
#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/task.h"
#include "led_strip.h"
#include "esp_log.h"
#include "nvs.h"
#include "nvs_flash.h"

#define BLINK_GPIO 8

uint8_t led_color = 0;
static char *TAG = "NVS_TEST";

led_strip_handle_t led_strip;
nvs_handle_t app_nvs_handle;

void static init_led_rgb(void)
{
    /* LED strip initialization with the GPIO and pixels number*/
    led_strip_config_t strip_config = {
        .strip_gpio_num = BLINK_GPIO,             // The GPIO that connected to the LED strip's data line
        .max_leds = 1,                            // The number of LEDs in the strip,
        .led_pixel_format = LED_PIXEL_FORMAT_GRB, // Pixel format of your LED strip
        .led_model = LED_MODEL_WS2812,            // LED strip model
        .flags.invert_out = false,                // whether to invert the output signal (useful when your hardware has a level inverter)
    };

    led_strip_rmt_config_t rmt_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT,    // different clock source can lead to different power consumption
        .resolution_hz = 10 * 1000 * 1000, // 10MHz
        .flags.with_dma = false,           // whether to enable the DMA feature
    };
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
}

static esp_err_t init_nvs()
{
    esp_err_t error;
    nvs_flash_init();

    error = nvs_open(TAG,NVS_READWRITE, &app_nvs_handle );
    if(error != ESP_OK)
    {
        ESP_LOGE(TAG, "Error opening NVS");
    }
    else{
        ESP_LOGI(TAG, "init_nvs completed");
    }

    return error;
}

static esp_err_t read_nvs(char * key, uint8_t * value)
{
    esp_err_t error;
    error = nvs_get_u8(app_nvs_handle, key,value );

    if(error != ESP_OK)
    {
        ESP_LOGE(TAG, "Error reading NVS");
    }
    else{
        ESP_LOGI(TAG, "value read : %u", * value);
    }

    return error;
}

static esp_err_t write_nvs(char * key, uint8_t value)
{
    esp_err_t error;
    error =nvs_set_u8(app_nvs_handle, key, value );

    if(error != ESP_OK)
    {
        ESP_LOGE(TAG, "Error writing NVS");
    }
    else{
        ESP_LOGI(TAG, "value write : %u", value);
    }

    return error;
}

void app_main(void)
{   
    char * key = "color"; 
    init_led_rgb(); 
    ESP_ERROR_CHECK(init_nvs()); 
    read_nvs(key, &led_color);  

    while (true)
    {
        switch (led_color)
        {
        case 0:
            led_strip_set_pixel(led_strip, 0, 255, 0, 0); // red
            break;
        case 1:
            led_strip_set_pixel(led_strip, 0, 0, 255, 0); // green
            break;
        case 2:
            led_strip_set_pixel(led_strip, 0, 0, 0, 255); // blue
            break;
        case 3:
            led_strip_set_pixel(led_strip, 0, 255, 255, 0); // yellow
            break;
        case 4:
            led_strip_set_pixel(led_strip, 0, 255, 0, 255); // purple
            break;
        case 5:
            led_strip_set_pixel(led_strip, 0, 0, 255, 255); // Aqua
            break;
        case 6:
            led_strip_set_pixel(led_strip, 0, 255, 255, 255); // white
            break;

        default:
            break;
        }
        led_strip_refresh(led_strip);

        led_color++;
        if (led_color > 6)
        {
            led_color = 0;
        }
        vTaskDelay(pdMS_TO_TICKS(1000)); 
        write_nvs(key, led_color) ;       
    }
}
