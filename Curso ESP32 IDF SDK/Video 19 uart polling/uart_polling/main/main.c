#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/uart.h"

static const char *tag = "UART";
#define ledR 14
#define ledG 12
#define ledB 13

#define UART_NUM UART_NUM_2
#define BUF_SIZE 1024
#define TASK_MEMORY 1024 * 2

static void uart_task(void *pvParameters)
{
    uint8_t *data = (uint8_t *)malloc(BUF_SIZE);

    while (1)
    {
        bzero(data, BUF_SIZE);

        int len = uart_read_bytes(UART_NUM, data, BUF_SIZE, pdMS_TO_TICKS(100));
        if (len == 0)
        {
            continue;
        }

        uart_write_bytes(UART_NUM, (const char *)data, len);

        ESP_LOGI(tag, "Data received: %s", data);

        for (size_t i = 0; i < len - 2; i++)
        {
            char value = data[i];

            switch (value)
            {
            case 'R':
                gpio_set_level(ledR, 1);
                gpio_set_level(ledG, 0);
                gpio_set_level(ledB, 0);

                break;
            case 'G':
                gpio_set_level(ledR, 0);
                gpio_set_level(ledG, 1);
                gpio_set_level(ledB, 0);

                break;
            case 'B':
                gpio_set_level(ledR, 0);
                gpio_set_level(ledG, 0);
                gpio_set_level(ledB, 1);

                break;

            default:
                gpio_set_level(ledR, 0);
                gpio_set_level(ledG, 0);
                gpio_set_level(ledB, 0);
                break;
            }
        }
    }
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

    uart_set_pin(UART_NUM, 5, 4, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    uart_driver_install(UART_NUM, BUF_SIZE, BUF_SIZE, 0, NULL, 0);
    xTaskCreate(uart_task, "uart_task", TASK_MEMORY, NULL, 5, NULL);

    ESP_LOGI(tag, "init uart completed!");
}

static void init_led(void)
{
    gpio_reset_pin(ledR);
    gpio_set_direction(ledR, GPIO_MODE_OUTPUT);
    gpio_reset_pin(ledG);
    gpio_set_direction(ledG, GPIO_MODE_OUTPUT);
    gpio_reset_pin(ledB);
    gpio_set_direction(ledB, GPIO_MODE_OUTPUT);

    ESP_LOGI(tag, "Init led completed");
}

void app_main(void)
{
    init_led();
    init_uart();
}