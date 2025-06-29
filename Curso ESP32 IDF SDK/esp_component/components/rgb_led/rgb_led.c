#include <stdio.h>
#include "rgb_led.h"

const char * TAG = "RGB_LED";

uint8_t ledR;
uint8_t ledG;
uint8_t ledB;
esp_err_t init_rgb_led(uint8_t led_r, uint8_t led_g, uint8_t led_b)
{
    ledR = led_r;
    ledG = led_g;
    ledB = led_b;
    gpio_reset_pin(led_r);
    gpio_reset_pin(led_g);
    gpio_reset_pin(led_b);
    gpio_set_direction(led_r, GPIO_MODE_OUTPUT);
    gpio_set_direction(led_g, GPIO_MODE_OUTPUT);
    gpio_set_direction(led_b, GPIO_MODE_OUTPUT);

    ESP_LOGI(TAG, "init_rgb_led");
    return ESP_OK;
}

esp_err_t set_rgb_red(void)
{
    gpio_set_level(ledR, 1);
    gpio_set_level(ledG, 0);
    gpio_set_level(ledB, 0);
    ESP_LOGI(TAG, "set_rgb_red");
    return ESP_OK;
}

esp_err_t set_rgb_green(void)
{
    gpio_set_level(ledR, 0);
    gpio_set_level(ledG, 1);
    gpio_set_level(ledB, 0);
    ESP_LOGI(TAG, "set_rgb_green");
    return ESP_OK;
}

esp_err_t set_rgb_blue(void)
{
    gpio_set_level(ledR, 0);
    gpio_set_level(ledG, 0);
    gpio_set_level(ledB, 1);
    ESP_LOGI(TAG, "set_rgb_blue");
    return ESP_OK;
}

esp_err_t set_rgb_off(void)
{
    gpio_set_level(ledR, 0);
    gpio_set_level(ledG, 0);
    gpio_set_level(ledB, 0);
    ESP_LOGI(TAG, "set_rgb_off");
    return ESP_OK;
}
