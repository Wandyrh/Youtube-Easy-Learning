#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#define ledR 33
#define ledG 25
#define ledB 26
#define button_isr 27

const char *tag = "Main";
uint8_t count =0;

esp_err_t init_led(void);
esp_err_t init_irs(void);
void isr_handler(void *args);

void app_main(void)
{    
    init_led();  
    init_irs();
}
esp_err_t init_led()
{
    gpio_reset_pin(ledR);
    gpio_set_direction(ledR, GPIO_MODE_OUTPUT);
    gpio_reset_pin(ledG);
    gpio_set_direction(ledG, GPIO_MODE_OUTPUT);
    gpio_reset_pin(ledB);
    gpio_set_direction(ledB, GPIO_MODE_OUTPUT);

    ESP_LOGI(tag, "Init led completed");
    return ESP_OK;
}

esp_err_t init_irs(void)
{
    gpio_config_t pGPIOConfig;
    pGPIOConfig.pin_bit_mask = (1ULL << button_isr);
    pGPIOConfig.mode = GPIO_MODE_DEF_INPUT;
    pGPIOConfig.pull_up_en = GPIO_PULLUP_DISABLE;
    pGPIOConfig.pull_down_en = GPIO_PULLDOWN_DISABLE;
    pGPIOConfig.intr_type = GPIO_INTR_NEGEDGE;  

    gpio_config(&pGPIOConfig);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(button_isr, isr_handler, NULL);

    ESP_LOGI(tag, "Init isr completed");
    return ESP_OK;
}

void isr_handler(void *args)
{
   count ++;
   if (count > 2)
   {
       count = 0;
   }

   switch (count)
   {
   case 0:
       gpio_set_level(ledR,1);
       gpio_set_level(ledG,0);
       gpio_set_level(ledB,0);
       break;
    case 1:
       gpio_set_level(ledR,0);
       gpio_set_level(ledG,1);
       gpio_set_level(ledB,0);
       break;
    case 2:
       gpio_set_level(ledR,0);
       gpio_set_level(ledG,0);
       gpio_set_level(ledB,1);
       break;
   
   default:
       break;
   }
    
}