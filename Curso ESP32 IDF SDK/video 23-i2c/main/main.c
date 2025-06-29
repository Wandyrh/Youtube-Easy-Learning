#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/Task.h"
#include "esp_log.h"
#include "driver/i2c.h"

#define I2C_SLAVE_ADDR 0x68
#define TIMEOUT_MS 1000
#define DELAY_MS 1000

static const char *TAG = "i2c";

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

void app_main(void)
{
    uint8_t rx_data[12];
    uint8_t command = 0x00;

    ESP_ERROR_CHECK(set_i2c());

    while (1)
    {
        i2c_master_write_read_device(I2C_NUM_0,
                                     I2C_SLAVE_ADDR,
                                     &command,
                                     1,
                                     rx_data,
                                     12,
                                     pdMS_TO_TICKS(TIMEOUT_MS));

        
        ESP_LOG_BUFFER_HEX(TAG, rx_data, 12);
        vTaskDelay(pdMS_TO_TICKS(DELAY_MS));
    }
}
