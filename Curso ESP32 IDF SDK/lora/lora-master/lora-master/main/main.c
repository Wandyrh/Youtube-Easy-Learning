#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "led_strip.h"
#include "driver/gpio.h"
#include "driver/uart.h"

#define UART_NUM UART_NUM_1
#define BUF_SIZE 1024
#define TASK_MEMORY 1024 * 2
#define RGB_LED 8
#define DELAY 1000
static const char *TAG = "ESP NOW MASTER";
static led_strip_t *pStrip_a;

static void configure_led(void)
{
    pStrip_a = led_strip_init(0, RGB_LED, 1);
    pStrip_a->clear(pStrip_a, 50);
}

static void init_uart(void)
{
    uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    uart_param_config(UART_NUM, &uart_config);

    uart_set_pin(UART_NUM, 3, 2, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    uart_driver_install(UART_NUM, BUF_SIZE, BUF_SIZE, 0, NULL, 0);

    ESP_LOGI(TAG, "init uart completed!");
}

void app_main(void)
{
    configure_led();
    init_uart();
    char *dataR = "R\r\n";
    char *dataG = "G\r\n";
    char *dataB = "B\r\n";

    while (true)
    {
        pStrip_a->set_pixel(pStrip_a, 0, 255, 0, 0);
        pStrip_a->refresh(pStrip_a, 100);
        uart_write_bytes(UART_NUM, (const char *)dataR, 3);
        ESP_LOGI(TAG, "sendind data %s", dataR);
        vTaskDelay(pdMS_TO_TICKS(DELAY));

        pStrip_a->set_pixel(pStrip_a, 0, 0, 255, 0);
        pStrip_a->refresh(pStrip_a, 100);
        uart_write_bytes(UART_NUM, (const char *)dataG, 3);
        ESP_LOGI(TAG, "sendind data %s", dataG);
        vTaskDelay(pdMS_TO_TICKS(DELAY));

        pStrip_a->set_pixel(pStrip_a, 0, 0, 0, 255);
        pStrip_a->refresh(pStrip_a, 100);
        uart_write_bytes(UART_NUM, (const char *)dataB, 3);
        ESP_LOGI(TAG, "sendind data %s", dataB);
        vTaskDelay(pdMS_TO_TICKS(DELAY));
    }
}
