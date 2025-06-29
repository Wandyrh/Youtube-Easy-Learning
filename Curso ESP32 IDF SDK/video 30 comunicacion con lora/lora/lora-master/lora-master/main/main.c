#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "led_strip.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "freertos/queue.h"

#define LORA_SLAVE 2
#define UART_NUM UART_NUM_1
#define BUF_SIZE 1024
#define TASK_MEMORY 1024 * 2
#define RGB_LED 8
#define DELAY 500
static const char *TAG = "LORA MASTER";
static led_strip_t *pStrip_a;
static QueueHandle_t uart_queue;

static void configure_led(void)
{
    pStrip_a = led_strip_init(0, RGB_LED, 1);
    pStrip_a->clear(pStrip_a, 50);
    ESP_LOGI(TAG, "configure_led completed!");
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

                ESP_LOGI(TAG, "Data received: %s", data);             

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
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    uart_param_config(UART_NUM, &uart_config);

    uart_set_pin(UART_NUM, 3, 2, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    uart_driver_install(UART_NUM, BUF_SIZE, BUF_SIZE, 5, &uart_queue, 0);
    xTaskCreate(uart_task, "uart_task", TASK_MEMORY, NULL, 7, NULL);

    ESP_LOGI(TAG, "init uart completed!");
}

static void init_lora_896(void)
{
    char *data = "AT\r\n";
    uart_write_bytes(UART_NUM, (const char *)data, strlen(data));
    vTaskDelay(pdMS_TO_TICKS(DELAY));

    data = "AT+ADDRESS=1\r\n";
    uart_write_bytes(UART_NUM, (const char *)data, strlen(data));
    vTaskDelay(pdMS_TO_TICKS(DELAY));

    data = "AT+ADDRESS?\r\n";
    uart_write_bytes(UART_NUM, (const char *)data, strlen(data));
    vTaskDelay(pdMS_TO_TICKS(DELAY));

    data = "AT+NETWORKID=10\r\n";
    uart_write_bytes(UART_NUM, (const char *)data, strlen(data));
    vTaskDelay(pdMS_TO_TICKS(DELAY));

    data = "AT+NETWORKID?\r\n";
    uart_write_bytes(UART_NUM, (const char *)data, strlen(data));
    vTaskDelay(pdMS_TO_TICKS(DELAY));

    data = "AT+CPIN=AABCF002EEDCFA90FABC0002EEDCAAF0\r\n";
    uart_write_bytes(UART_NUM, (const char *)data, strlen(data));
    vTaskDelay(pdMS_TO_TICKS(DELAY));

    data = "AT+CPIN?\r\n";
    uart_write_bytes(UART_NUM, (const char *)data, strlen(data));
    vTaskDelay(pdMS_TO_TICKS(DELAY));

    ESP_LOGI(TAG, "init_lora_896 completed!");
}

static void lora_send(int address, const char *data)
{
    char command_data[300] = "";
    char command[] = "AT+SEND=";
    char separator[] = ",";
    char end_of_line[] = "\r\n";
    // "AT+SEND=2,11,000|255|000"
    snprintf(command_data, 200, "%s%d%s%d%s%s%s", command, address, separator, strlen(data), separator, data, end_of_line);
    ESP_LOGI(TAG, "command_data : %s", command_data);

    uart_write_bytes(UART_NUM, (const char *)command_data, strlen(command_data));
    vTaskDelay(pdMS_TO_TICKS(DELAY * 2));
}

void app_main(void)
{
    configure_led();
    init_uart();
    init_lora_896();

    uint8_t count = 0;
    while (true)
    {
        count++;
        count = count > 2 ? 0 : count;

        switch (count)
        {
        case 0:
            pStrip_a->set_pixel(pStrip_a, 0, 255, 0, 0);
            pStrip_a->refresh(pStrip_a, 100);
            vTaskDelay(pdMS_TO_TICKS(500));
            lora_send(LORA_SLAVE, "255|000|000");
            break;
        case 1:
            pStrip_a->set_pixel(pStrip_a, 0, 0, 255, 0);
            pStrip_a->refresh(pStrip_a, 100);
            lora_send(LORA_SLAVE, "000|255|000");
            break;
        case 2:
            pStrip_a->set_pixel(pStrip_a, 0, 0, 0, 255);
            pStrip_a->refresh(pStrip_a, 100);
            lora_send(LORA_SLAVE, "000|000|255");
            break;

        default:
            break;
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
