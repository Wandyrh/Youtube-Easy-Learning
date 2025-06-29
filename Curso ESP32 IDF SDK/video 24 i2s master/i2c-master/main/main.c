#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/Task.h"
#include "esp_log.h"
#include "driver/i2c.h"

#define I2C_SLAVE_ADDR 0x32
#define RX_BUFFER_LEN 255
#define TIMEOUT_MS 1000
#define DELAY_MS 1000

static const char *TAG = "i2c-master";

static esp_err_t set_i2c(void)
{
    i2c_config_t i2c_config = {};

    i2c_config.mode = I2C_MODE_MASTER;
    i2c_config.sda_io_num = 21;
    i2c_config.scl_io_num = 22;
    i2c_config.sda_pullup_en = true;
    i2c_config.scl_pullup_en = true;
    i2c_config.master.clk_speed = 400000;
    i2c_config.clk_flags = 0;

    ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_0, &i2c_config));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, ESP_INTR_FLAG_LEVEL1));

    return ESP_OK;
}

static void copy_array(uint8_t *dest, uint8_t *src, uint8_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        dest[i] = src[i];
    }
}

void app_main(void)
{
    uint8_t rx_data[RX_BUFFER_LEN];
    uint8_t command[12];
    uint8_t command_led_r[12] = "LEDR";
    uint8_t command_led_g[12] = "LEDG";
    uint8_t command_led_b[12] = "LEDB";
    uint8_t count = 0;

    ESP_ERROR_CHECK(set_i2c());

    while (1)
    {
        count++;
        if (count > 2)
        {
            count = 0;
        }

        switch (count)
        {
        case 0:
            copy_array(&command, command_led_r, 12);
            break;
        case 1:
            copy_array(&command, command_led_g, 12);
            break;
        case 2:
            copy_array(&command, command_led_b, 12);
            break;

        default:
            break;
        }

        i2c_master_write_read_device(I2C_NUM_0,
                                     I2C_SLAVE_ADDR,
                                     &command,
                                     sizeof(command),
                                     rx_data,
                                     RX_BUFFER_LEN,
                                     pdMS_TO_TICKS(TIMEOUT_MS));

        ESP_LOG_BUFFER_CHAR(TAG, rx_data, 32);
        vTaskDelay(pdMS_TO_TICKS(DELAY_MS));
    }
}
