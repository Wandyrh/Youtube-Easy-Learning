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
#define DELAY 500

static const char *TAG = "LORA SLAVE";
static led_strip_t *pStrip_a;
static QueueHandle_t uart_queue;

typedef struct LoraData
{
    char *Address;
    char *DataLength;
    char *Data;
    char *SignalStrength;
    char *SignalNoise;
} Lora_Data_t;

Lora_Data_t Lora_data;

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

                //+RCV=1,11,000|255|000,-65,51
                if (strncmp((const char *)data, "+RCV=", 5) == 0)
                {
                    char *token = strtok((char *)data, "=");
                    // loop through the string to extract all other tokens
                    uint8_t count_token = 0;
                    while (token != NULL)
                    {
                        token = strtok(NULL, ",");
                        count_token++;
                        switch (count_token)
                        {
                        case 1:
                            Lora_data.Address = token;
                            break;
                        case 2:
                            Lora_data.DataLength = token;
                            break;
                        case 3:
                            Lora_data.Data = token;
                            break;
                        case 4:
                            Lora_data.SignalStrength = token;
                            break;
                        case 5:
                            Lora_data.SignalNoise = token;
                            break;

                        default:
                            break;
                        }
                    }
                    uint8_t r_value;
                    uint8_t g_value;
                    uint8_t b_value;

                    r_value = atoi(strtok((char *)Lora_data.Data, "|")); //000|255|000
                    g_value = atoi(strtok(NULL, "|"));
                    b_value = atoi(strtok(NULL, "|"));

                    pStrip_a->set_pixel(pStrip_a, 0, r_value, g_value, b_value);
                    pStrip_a->refresh(pStrip_a, 100);
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
    xTaskCreate(uart_task, "uart_task", TASK_MEMORY, NULL, 5, NULL);

    ESP_LOGI(TAG, "init uart completed!");
}

static void init_lora_896(void)
{
    char *data = "AT\r\n";
    uart_write_bytes(UART_NUM, (const char *)data, strlen(data));
    vTaskDelay(pdMS_TO_TICKS(DELAY));

    data = "AT+ADDRESS=2\r\n";
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

    char *command_data2 = "AT+SEND?\r\n";
    uart_write_bytes(UART_NUM, (const char *)command_data2, strlen(command_data2));
    vTaskDelay(pdMS_TO_TICKS(DELAY));
}
void app_main(void)
{
    configure_led();
    init_uart();
    init_lora_896();
}
