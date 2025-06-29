#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "rgb_led.h"

#define LED_R 48
#define LED_G 47
#define LED_B 21

void app_main(void)
{
    init_rgb_led(LED_R, LED_G, LED_B);

    while (1)
    {        
        vTaskDelay(pdMS_TO_TICKS(2000));
        set_rgb_red();
        // vTaskDelay(pdMS_TO_TICKS(500));
        // set_rgb_green();
        // vTaskDelay(pdMS_TO_TICKS(500));
        // set_rgb_blue();
        // vTaskDelay(pdMS_TO_TICKS(500));
        vTaskDelay(pdMS_TO_TICKS(2000));
        set_rgb_off();
    }
}