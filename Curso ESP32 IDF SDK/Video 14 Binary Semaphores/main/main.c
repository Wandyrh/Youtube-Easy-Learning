#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"

#define ledR 33
#define ledG 25
#define ledB 26
#define STACK_SIZE 1024 * 2
#define R_delay 10000
#define G_delay 2000

xSemaphoreHandle GlobalKey = 0;
const char *tag = "Main";

esp_err_t init_led(void);
esp_err_t create_tasks(void);
esp_err_t shared_resource(int led);
void vTaskR(void *pvParameters);
void vTaskG(void *pvParameters);

void app_main(void)
{

    GlobalKey = xSemaphoreCreateBinary();
    init_led();
    create_tasks();
}
esp_err_t init_led()
{
    gpio_reset_pin(ledR);
    gpio_set_direction(ledR, GPIO_MODE_OUTPUT);
    gpio_reset_pin(ledG);
    gpio_set_direction(ledG, GPIO_MODE_OUTPUT);
    gpio_reset_pin(ledB);
    gpio_set_direction(ledB, GPIO_MODE_OUTPUT);
    return ESP_OK;
}

esp_err_t create_tasks(void)
{
    static uint8_t ucParameterToPass;
    TaskHandle_t xHandle = NULL;

    xTaskCreatePinnedToCore(vTaskR,
                            "vTaskR",
                            STACK_SIZE,
                            &ucParameterToPass,
                            1,
                            &xHandle,
                            0);

    xTaskCreatePinnedToCore(vTaskG,
                            "vTaskG",
                            STACK_SIZE,
                            &ucParameterToPass,
                            1,
                            &xHandle,
                            1);

    return ESP_OK;
}

esp_err_t shared_resource(int led)
{
    for (size_t i = 0; i < 8; i++)
    {
        vTaskDelay(pdMS_TO_TICKS(400));
        gpio_set_level(led, 1);
        vTaskDelay(pdMS_TO_TICKS(400));
        gpio_set_level(led, 0);
    }

    return ESP_OK;
}

void vTaskR(void *pvParameters)
{
    while (1)
    {
        printf("stick to MS: %i", pdTICKS_TO_MS(portMAX_DELAY));
        for (size_t i = 0; i < 8; i++)
        {
            vTaskDelay(pdMS_TO_TICKS(400));
            gpio_set_level(ledR, 1);
            vTaskDelay(pdMS_TO_TICKS(400));
            gpio_set_level(ledR, 0);
        }
        ESP_LOGE(tag, "Task R is giving the key");
        xSemaphoreGive(GlobalKey);

        vTaskDelay(pdMS_TO_TICKS(R_delay));
    }
}

void vTaskG(void *pvParameters)
{

    while (1)
    {
        if (xSemaphoreTake(GlobalKey, portMAX_DELAY))
        {
            ESP_LOGI(tag, "Task G is working");
            for (size_t i = 0; i < 8; i++)
            {
                vTaskDelay(pdMS_TO_TICKS(400));
                gpio_set_level(ledG, 1);
                vTaskDelay(pdMS_TO_TICKS(400));
                gpio_set_level(ledG, 0);
            }
            ESP_LOGW(tag, "Task G is sleeping");
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}