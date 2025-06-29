#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "led_strip.h"
#include "driver/ledc.h"
#include "esp_adc/adc_oneshot.h"
#include "freertos/semphr.h"

#define LED_1 48
#define LED_2 47
#define LED_3 21
#define LED_4 14
#define RELAY 45
#define BUZZER 46
#define LED_STRIP 1
#define BT1 2
#define BT2 15

#define LED_STRIP_MAX_LEDS 4
#define STACK_SIZE 1024 * 2

int8_t ADC_1 = 0;
int8_t rgb_color = 0;

static const char *TAG = "devkit";
SemaphoreHandle_t GlobalKey = 0;
uint8_t Enable_isr = 1;
void isr_handler_button_dimemer_up(void *args);
void isr_handler_button_dimemer_down(void *args);
adc_oneshot_unit_handle_t ret_unit;
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
    gpio_reset_pin(RELAY);

    gpio_set_direction(LED_1, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_2, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_3, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_4, GPIO_MODE_OUTPUT);
    gpio_set_direction(RELAY, GPIO_MODE_OUTPUT);

    return ESP_OK;
}

esp_err_t set_pwm(void)
{
    ledc_channel_config_t channelConfigBuzzer = {0};
    channelConfigBuzzer.gpio_num = BUZZER;
    channelConfigBuzzer.speed_mode = LEDC_LOW_SPEED_MODE;
    channelConfigBuzzer.channel = LEDC_CHANNEL_0;
    channelConfigBuzzer.intr_type = LEDC_INTR_DISABLE;
    channelConfigBuzzer.timer_sel = LEDC_TIMER_0;
    channelConfigBuzzer.duty = 0;

    ledc_channel_config(&channelConfigBuzzer);

    ledc_timer_config_t timerConfig = {0};
    timerConfig.speed_mode = LEDC_LOW_SPEED_MODE;
    timerConfig.duty_resolution = LEDC_TIMER_10_BIT;
    timerConfig.timer_num = LEDC_TIMER_0;
    timerConfig.freq_hz = 3000; //

    ledc_timer_config(&timerConfig);
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

static esp_err_t set_adc(void)
{
    adc_oneshot_unit_init_cfg_t init_config = {};
    init_config.unit_id = ADC_UNIT_1;
    init_config.clk_src = ADC_RTC_CLK_SRC_DEFAULT;
    init_config.ulp_mode = ADC_ULP_MODE_DISABLE;

    adc_oneshot_new_unit(&init_config, &ret_unit);

    adc_oneshot_chan_cfg_t config = {};
    config.atten = ADC_ATTEN_DB_11;
    config.bitwidth = ADC_BITWIDTH_DEFAULT;

    adc_oneshot_config_channel(ret_unit, ADC_CHANNEL_3, &config);
    return ESP_OK;
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
    set_adc();
    init_led();
    init_led_strip();
    set_pwm();
    init_irs();

    uint8_t countLed = 0;

    while (true)
    {
        vTaskDelay(pdMS_TO_TICKS(200));
        int adc_value = 0;
        adc_oneshot_read(ret_unit, ADC_CHANNEL_3, &adc_value);
        ESP_LOGI(TAG, "adc value: %i", adc_value);
        ESP_LOGI(TAG, "rgb color value: %i", rgb_color);
        adc_value = adc_value / 1000;

        switch (adc_value)
        {
        case 0:
            gpio_set_level(LED_1, 0);
            gpio_set_level(LED_2, 0);
            gpio_set_level(LED_3, 0);
            gpio_set_level(LED_4, 0);
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
            gpio_set_level(RELAY, 1);
            break;
        case 1:
            gpio_set_level(LED_1, 1);
            gpio_set_level(LED_2, 0);
            gpio_set_level(LED_3, 0);
            gpio_set_level(LED_4, 0);
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
            break;
        case 2:
            gpio_set_level(LED_1, 1);
            gpio_set_level(LED_2, 1);
            gpio_set_level(LED_3, 0);
            gpio_set_level(LED_4, 0);
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
            break;
        case 3:
            gpio_set_level(LED_1, 1);
            gpio_set_level(LED_2, 1);
            gpio_set_level(LED_3, 1);
            gpio_set_level(LED_4, 0);
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
            break;
        case 4:
            gpio_set_level(LED_1, 1);
            gpio_set_level(LED_2, 1);
            gpio_set_level(LED_3, 1);
            gpio_set_level(LED_4, 1);
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 512);
            gpio_set_level(RELAY, 0);
            break;

        default:
            break;
        }
      

        
        switch (rgb_color)
        {
        case 0:  
            for (size_t i = 0; i < LED_STRIP_MAX_LEDS; i++)
            {
                led_strip_set_pixel(led_strip,i ,20,0,0);
            } 
            break;

        case 1:
            for (size_t i = 0; i < LED_STRIP_MAX_LEDS; i++)
            {
                led_strip_set_pixel(led_strip,i ,0,20,0);
            } 
            break;
        case 2:
            for (size_t i = 0; i < LED_STRIP_MAX_LEDS; i++)
            {
                led_strip_set_pixel(led_strip,i ,0,0,20);
            } 
            break;
        case 3:
            for (size_t i = 0; i < LED_STRIP_MAX_LEDS; i++)
            {
                led_strip_set_pixel(led_strip,i ,20,20,0);
            } 
            break;
        case 4:
            for (size_t i = 0; i < LED_STRIP_MAX_LEDS; i++)
            {
                led_strip_set_pixel(led_strip,i ,20,0,20);
            } 
            break;
        case 5:
            for (size_t i = 0; i < LED_STRIP_MAX_LEDS; i++)
            {
                led_strip_set_pixel(led_strip,i ,0,20,20);
            } 
            break;
        case 6:
            for (size_t i = 0; i < LED_STRIP_MAX_LEDS; i++)
            {
                led_strip_set_pixel(led_strip,i ,20,20,20);
            } 
            break;

        default:
            break;
        }

        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);        
        led_strip_refresh(led_strip);
    }
}
