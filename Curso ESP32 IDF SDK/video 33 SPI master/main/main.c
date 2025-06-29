#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"
#include "matrix.c"

#define MOSI_GPIO 7
#define MISO_GPIO 2
#define SPI_SCLK 6
#define SPI_CS 10

#define DECODE_MODE_REG 0X9   // 0
#define INTENSITY_REG 0X0A    // 7
#define SCAN_LIMIT_REG 0X0B   // 7
#define SHUTDOWN_REG 0X0C     // 1
#define DISPLAY_TEST_REG 0x0F // 0

#define MAX_DEVICE 1

spi_device_handle_t spi_device_handle;

static esp_err_t init_spi(void)
{
    spi_bus_config_t spi_bus_config = {};
    spi_bus_config.mosi_io_num = MOSI_GPIO;
    spi_bus_config.miso_io_num = MISO_GPIO;
    spi_bus_config.sclk_io_num = SPI_SCLK;
    spi_bus_config.quadwp_io_num = -1;
    spi_bus_config.quadhd_io_num = -1;
    spi_bus_config.max_transfer_sz = 32;

    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &spi_bus_config, SPI_DMA_CH_AUTO));

    spi_device_interface_config_t spi_device_interface_config = {};
    spi_device_interface_config.clock_speed_hz = 1000000;
    spi_device_interface_config.duty_cycle_pos = 128;
    spi_device_interface_config.mode = 0;
    spi_device_interface_config.spics_io_num = SPI_CS;
    spi_device_interface_config.flags = SPI_DEVICE_HALFDUPLEX;
    spi_device_interface_config.queue_size = 1;
    spi_device_interface_config.pre_cb = NULL;
    spi_device_interface_config.post_cb = NULL;

    ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &spi_device_interface_config, &spi_device_handle));
    return ESP_OK;
}

static void spi_write(uint8_t reg, uint8_t value)
{
    uint8_t data[2] = {reg, value};
    spi_transaction_t spi_transaction = {
        .tx_buffer = data,
        .length = 16};
    ESP_ERROR_CHECK(spi_device_transmit(spi_device_handle, &spi_transaction));
}

static void max7219_clear(void)
{
    for (size_t i = 0; i < MAX_DEVICE; i++)
    {
        for (size_t j = 0; j < 8; j++)
        {
            spi_write(j + 1, 0);
        }
    }
}
static esp_err_t max7219_init(void)
{
    for (size_t i = 0; i < MAX_DEVICE; i++)
    {
        spi_write(DECODE_MODE_REG, 0);
        spi_write(INTENSITY_REG, 7);
        spi_write(SCAN_LIMIT_REG, 7);
        spi_write(SHUTDOWN_REG, 1);
        spi_write(DISPLAY_TEST_REG, 0);
    }

    return ESP_OK;
}

static void max7219_print_static_letters()
{
    for (size_t i = 0; i < images_len; i++)
    {
        for (size_t j = 0; j < 8; j++)
        {
            spi_write(j + 1, images[i][j]);
        }
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

void app_main(void)
{
    ESP_ERROR_CHECK(init_spi());
    ESP_ERROR_CHECK(max7219_init());
    max7219_clear();

    while (true)
    {
        max7219_print_static_letters();
    }
}