#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/Task.h"
#include "esp_log.h"
#include "driver/i2c.h"
#include "driver/gpio.h"

#define I2C_SLAVE_ADDR 0x32
#define RX_BUFFER_LEN 255
#define TX_BUFFER_LEN 255
#define LED_R 14
#define LED_G 12
#define LED_B 13
#define LED_BOARD 2

static const char *TAG = "i2c-slave";

static esp_err_t set_i2c(void)
{
    i2c_config_t i2c_config = {};

    i2c_config.mode = I2C_MODE_SLAVE;
    i2c_config.sda_io_num = 21;
    i2c_config.scl_io_num = 22;
    i2c_config.sda_pullup_en = true;
    i2c_config.scl_pullup_en = true;
    i2c_config.slave.addr_10bit_en = 0;
    i2c_config.slave.slave_addr = I2C_SLAVE_ADDR; 
    i2c_config.clk_flags = 0;

    ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_0, &i2c_config));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, I2C_MODE_SLAVE, RX_BUFFER_LEN, TX_BUFFER_LEN, ESP_INTR_FLAG_LEVEL1));

    return ESP_OK;
}

static esp_err_t init_leds(void)
{
    gpio_reset_pin(LED_R);
    gpio_set_direction(LED_R,GPIO_MODE_OUTPUT);
    gpio_reset_pin(LED_G);
    gpio_set_direction(LED_G,GPIO_MODE_OUTPUT);
    gpio_reset_pin(LED_B);
    gpio_set_direction(LED_B,GPIO_MODE_OUTPUT);
    gpio_reset_pin(LED_BOARD);
    gpio_set_direction(LED_BOARD,GPIO_MODE_OUTPUT);

    gpio_set_level(LED_R,1);
    gpio_set_level(LED_G,1);
    gpio_set_level(LED_B,1);

    ESP_LOGI(TAG, "init_leds completed");

    return ESP_OK;
}
static void i2c_handle_task(void *pvParameters)
{
   ESP_LOGI(TAG, "i2c_handle_task start");
   uint8_t rx_data[RX_BUFFER_LEN] = {0};
   uint8_t tx_data[32] = "Hello from Slave!";

   while (1)
   {
    vTaskDelay(pdMS_TO_TICKS(100));
    gpio_set_direction(LED_BOARD,0);

    int len = i2c_slave_read_buffer(I2C_NUM_0,&rx_data,RX_BUFFER_LEN,pdMS_TO_TICKS(100));
    if (len > 0)
    {
        gpio_set_direction(LED_BOARD,1);
        ESP_LOG_BUFFER_CHAR(TAG, rx_data, 32);

        i2c_reset_rx_fifo(I2C_NUM_0); 

        if(strncmp((const char *)rx_data,"LEDR",4) == 0)
        {
            gpio_set_level(LED_R,0);
            gpio_set_level(LED_G,1);
            gpio_set_level(LED_B,1);
        }
        if(strncmp((const char *)rx_data,"LEDG",4) == 0)
        {
            gpio_set_level(LED_R,1);
            gpio_set_level(LED_G,0);
            gpio_set_level(LED_B,1);
        }
        if(strncmp((const char *)rx_data,"LEDB",4) == 0)
        {
            gpio_set_level(LED_R,1);
            gpio_set_level(LED_G,1);
            gpio_set_level(LED_B,0);
        }

        i2c_slave_write_buffer(I2C_NUM_0, &tx_data,32 ,pdMS_TO_TICKS(100));

        bzero(rx_data, RX_BUFFER_LEN);        
    }
    
   } 
}

static esp_err_t create_i2c_handle_task(void)
{
    xTaskCreate(i2c_handle_task,"i2c_handle_task",1024*4,NULL,1,NULL);
    return ESP_OK;
}
void app_main(void)
{
    ESP_ERROR_CHECK(set_i2c());
    ESP_ERROR_CHECK(init_leds());
    ESP_ERROR_CHECK(create_i2c_handle_task());
}
