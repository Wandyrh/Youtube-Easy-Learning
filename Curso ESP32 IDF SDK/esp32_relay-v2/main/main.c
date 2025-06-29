#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <led_strip.h>

#define relay1 14
#define relay2 21
#define relay3 47
#define relay4 48

#define LED_STRIP 1
#define LED_STRIP_MAX_LEDS 1

led_strip_handle_t led_strip;

static void init_gpios()
{
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;

    int relays[] = {relay1, relay2, relay3, relay4};
    int num_relays = sizeof(relays) / sizeof(relays[0]);

    for (int i = 0; i < num_relays; i++)
    {
        io_conf.pin_bit_mask = (1ULL << relays[i]);
        gpio_config(&io_conf);
        gpio_set_level(relays[i], 0);
    }
}

static void init_led_strip()
{
    /* LED strip initialization with the GPIO and pixels number*/
    led_strip_config_t strip_config = {
        .strip_gpio_num = LED_STRIP,    // The GPIO that connected to the LED strip's data line
        .max_leds = LED_STRIP_MAX_LEDS, // The number of LEDs in the strip,
        .led_model = LED_MODEL_WS2812,  // LED strip model
        .flags.invert_out = false,      // whether to invert the output signal (useful when your hardware has a level inverter)
    };

    led_strip_rmt_config_t rmt_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT,    // different clock source can lead to different power consumption
        .resolution_hz = 10 * 1000 * 1000, // 10MHz
        .flags.with_dma = false,           // whether to enable the DMA feature
    };
    led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip);
}

void app_main(void)
{

    init_gpios();
    init_led_strip();

    int count = 0;

    while (true)
    {
        switch (count)
        {
        case 0:
            gpio_set_level(relay1, 1);
            gpio_set_level(relay2, 0);
            gpio_set_level(relay3, 0);
            gpio_set_level(relay4, 0);
            led_strip_set_pixel(led_strip, 0, 20, 0, 0);
            break;

        case 1:
            gpio_set_level(relay1, 0);
            gpio_set_level(relay2, 1);
            gpio_set_level(relay3, 0);
            gpio_set_level(relay4, 0);
            led_strip_set_pixel(led_strip, 0, 0, 20, 0);

            break;

        case 2:
            gpio_set_level(relay1, 0);
            gpio_set_level(relay2, 0);
            gpio_set_level(relay3, 1);
            gpio_set_level(relay4, 0);
            led_strip_set_pixel(led_strip, 0, 0, 0, 20);
            break;

        case 3:
            gpio_set_level(relay1, 0);
            gpio_set_level(relay2, 0);
            gpio_set_level(relay3, 0);
            gpio_set_level(relay4, 1);
            led_strip_set_pixel(led_strip, 0, 20, 20, 0);
            break;

        case 4:
            gpio_set_level(relay1, 0);
            gpio_set_level(relay2, 0);
            gpio_set_level(relay3, 1);
            gpio_set_level(relay4, 0);
            led_strip_set_pixel(led_strip, 0, 20, 0, 20);
            break;

        case 5:
            gpio_set_level(relay1, 0);
            gpio_set_level(relay2, 1);
            gpio_set_level(relay3, 0);
            gpio_set_level(relay4, 0);
            led_strip_set_pixel(led_strip, 0, 0, 20, 20);
            break;

            break;

        default:
            break;
        }
        led_strip_refresh(led_strip);
        count++;
        if (count > 5)
        {
            count = 0;
        }

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
