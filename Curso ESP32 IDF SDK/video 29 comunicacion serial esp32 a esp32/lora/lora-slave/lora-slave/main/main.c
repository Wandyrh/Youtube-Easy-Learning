#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "led_strip.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "freertos/queue.h"

#define UART_NUM UART_NUM_1
#define BUF_SIZE 1024
#define TASK_MEMORY 1024 * 2
#define RGB_LED 8
static const char *TAG = "ESP NOW SLAVE";
static led_strip_t *pStrip_a;
static QueueHandle_t uart_queue;

static void configure_led(void)
{
    pStrip_a = led_strip_init(0, RGB_LED, 1);
    pStrip_a->clear(pStrip_a, 50);
}

static void uart_task(void *pvParameters)
{
    uart_event_t event;
    uint8_t *data = (uint8_t *)malloc(BUF_SIZE);

    while (1)
    {
        if (xQueueReceive(uart_queue, (void *)&event, portMAX_DELAY))
        {
            bzero(data, BUF_SIZE);

            switch (event.type)
            {
            case UART_DATA:
                uart_read_bytes(UART_NUM, data, event.size, pdMS_TO_TICKS(100));
                uart_write_bytes(UART_NUM, (const char *)data, event.size);
                uart_flush(UART_NUM);

                ESP_LOGI(TAG, "Data received: %s", data);

                for (size_t i = 0; i < event.size - 2; i++)
                {
                    char value = data[i];

                    switch (value)
                    {
                    case 'R':
                        ESP_LOGI(TAG, "LED RED");
                        pStrip_a->set_pixel(pStrip_a, 0, 255, 0, 0);
                        pStrip_a->refresh(pStrip_a, 100);
                        vTaskDelay(pdMS_TO_TICKS(1000));

                        break;
                    case 'G':
                        ESP_LOGI(TAG, "LED GREEN");
                        pStrip_a->set_pixel(pStrip_a, 0, 0, 255, 0);
                        pStrip_a->refresh(pStrip_a, 100);
                        vTaskDelay(pdMS_TO_TICKS(1000));

                        break;
                    case 'B':
                        ESP_LOGI(TAG, "LED BLUE");
                        pStrip_a->set_pixel(pStrip_a, 0, 0, 0, 255);
                        pStrip_a->refresh(pStrip_a, 100);
                        vTaskDelay(pdMS_TO_TICKS(1000));

                        break;

                    default:
                        ESP_LOGI(TAG, "LED OFF");
                        pStrip_a->set_pixel(pStrip_a, 0, 0, 0, 0);
                        pStrip_a->refresh(pStrip_a, 100);
                        vTaskDelay(pdMS_TO_TICKS(1000));
                        break;
                    }
                }

                break;

            default:
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

    uart_set_pin(UART_NUM, 3, 2, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    uart_driver_install(UART_NUM, BUF_SIZE, BUF_SIZE, 5, &uart_queue, 0);
    xTaskCreate(uart_task, "uart_task", TASK_MEMORY, NULL, 5, NULL);

    ESP_LOGI(TAG, "init uart completed!");
}

void app_main(void)
{
    configure_led();
    init_uart();
}
