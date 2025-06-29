#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "led_strip.h"
#include "driver/ledc.h"
#include "esp_adc/adc_oneshot.h"
#include "freertos/semphr.h"

#define LED_1 10
#define LED_2 15
#define LED_3 18
#define LED_4 19

#define LED_STRIP 8
#define BT1 20
#define BT2 21

#define LED_STRIP_MAX_LEDS 1
#define STACK_SIZE 1024 * 2

int8_t rgb_color = 0;

static const char *TAG = "esp32-c6-gateway-test";
SemaphoreHandle_t GlobalKey = 0;
uint8_t Enable_isr = 1;
void isr_handler_button_dimemer_up(void *args);
void isr_handler_button_dimemer_down(void *args);
led_strip_handle_t led_strip;

static esp_err_t init_led_strip(void)
{
    /* LED strip initialization with the GPIO and pixels number*/
    led_strip_config_t strip_config = {
        .strip_gpio_num = LED_STRIP,              // The GPIO that connected to the LED strip's data line
        .max_leds = LED_STRIP_MAX_LEDS,           // The number of LEDs in the strip,
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

    return ESP_OK;
}

static esp_err_t init_led(void)
{
    gpio_reset_pin(LED_1);
    gpio_reset_pin(LED_2);
    gpio_reset_pin(LED_3);    
    gpio_reset_pin(LED_4);

    gpio_set_direction(LED_1, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_2, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_3, GPIO_MODE_OUTPUT);    
    gpio_set_direction(LED_4, GPIO_MODE_OUTPUT);

    return ESP_OK;
}

static void init_irs(void)
{
    gpio_config_t pGPIOConfig;
    pGPIOConfig.pin_bit_mask = (1ULL << BT1) | (1ULL << BT2);
    pGPIOConfig.mode = GPIO_MODE_DEF_INPUT;
    pGPIOConfig.pull_up_en = GPIO_PULLUP_ENABLE;
    pGPIOConfig.pull_down_en = GPIO_PULLDOWN_DISABLE;
    pGPIOConfig.intr_type = GPIO_INTR_NEGEDGE;

    gpio_config(&pGPIOConfig);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(BT1, isr_handler_button_dimemer_up, NULL);
    gpio_isr_handler_add(BT2, isr_handler_button_dimemer_down, NULL);

    ESP_LOGI(TAG, "Init isr completed");
}

void isr_handler_button_dimemer_up(void *args)
{    
    if (Enable_isr == 1)
    {
        xSemaphoreGive(GlobalKey);
        rgb_color--;
        if (rgb_color < 0)
        {
            rgb_color = 0;
        }
    }
}
void isr_handler_button_dimemer_down(void *args)
{    
    if (Enable_isr == 1)
    {
        xSemaphoreGive(GlobalKey);
        rgb_color++;
        if (rgb_color > 6)
        {
            rgb_color = 6;
        }
    }
}

static void vTask_Evaluate_irs(void *pvParameters)
{
    while (1)
    {
        if (xSemaphoreTake(GlobalKey, portMAX_DELAY))
        {
            Enable_isr = 0;
        }

        vTaskDelay(pdMS_TO_TICKS(500));
        Enable_isr = 1;
    }
}

static void create_tasks(void)
{
    static uint8_t ucParameterToPass;
    TaskHandle_t xHandle = NULL;
    xTaskCreate(vTask_Evaluate_irs,
                "vTask_Evaluate_irs",
                STACK_SIZE,
                &ucParameterToPass,
                1,
                &xHandle);
}

void app_main(void)
{
    GlobalKey = xSemaphoreCreateBinary();
    create_tasks();    
    init_led();
    init_led_strip();
    init_irs();   

    while (true)
    {
        vTaskDelay(pdMS_TO_TICKS(200));  
        ESP_LOGI(TAG, "rgb color value: %i", rgb_color);
        
        switch (rgb_color)
        {
        case 0:  
            for (size_t i = 0; i < LED_STRIP_MAX_LEDS; i++)
            {
                led_strip_set_pixel(led_strip,i ,20,0,0);
                gpio_set_level(LED_1, 0);
                gpio_set_level(LED_2, 0);
                gpio_set_level(LED_3, 0);
                gpio_set_level(LED_4, 0);
            } 
            break;

        case 1:
            for (size_t i = 0; i < LED_STRIP_MAX_LEDS; i++)
            {
                led_strip_set_pixel(led_strip,i ,0,20,0);
                gpio_set_level(LED_1, 1);
                gpio_set_level(LED_2, 0);
                gpio_set_level(LED_3, 0);
                gpio_set_level(LED_4, 0);
            } 
            break;
        case 2:
            for (size_t i = 0; i < LED_STRIP_MAX_LEDS; i++)
            {
                led_strip_set_pixel(led_strip,i ,0,0,20);
                gpio_set_level(LED_1, 1);
                gpio_set_level(LED_2, 1);
                gpio_set_level(LED_3, 0);
                gpio_set_level(LED_4, 0);
            } 
            break;
        case 3:
            for (size_t i = 0; i < LED_STRIP_MAX_LEDS; i++)
            {
                led_strip_set_pixel(led_strip,i ,20,20,0);
                gpio_set_level(LED_1, 1);
                gpio_set_level(LED_2, 1);
                gpio_set_level(LED_3, 1);
                gpio_set_level(LED_4, 0);
            } 
            break;
        case 4:
            for (size_t i = 0; i < LED_STRIP_MAX_LEDS; i++)
            {
                led_strip_set_pixel(led_strip,i ,20,0,20);
                gpio_set_level(LED_1, 1);
                gpio_set_level(LED_2, 1);
                gpio_set_level(LED_3, 1);
                gpio_set_level(LED_4, 1);
            } 
            break;
        case 5:
            for (size_t i = 0; i < LED_STRIP_MAX_LEDS; i++)
            {
                led_strip_set_pixel(led_strip,i ,0,20,20);
                gpio_set_level(LED_1, 1);
                gpio_set_level(LED_2, 1);
                gpio_set_level(LED_3, 1);
                gpio_set_level(LED_4, 1);
            } 
            break;
        case 6:
            for (size_t i = 0; i < LED_STRIP_MAX_LEDS; i++)
            {
                led_strip_set_pixel(led_strip,i ,20,20,20);
                gpio_set_level(LED_1, 1);
                gpio_set_level(LED_2, 1);
                gpio_set_level(LED_3, 1);
                gpio_set_level(LED_4, 1);
            } 
            break;

        default:
            break;
        }
              
        led_strip_refresh(led_strip);
    }
}
