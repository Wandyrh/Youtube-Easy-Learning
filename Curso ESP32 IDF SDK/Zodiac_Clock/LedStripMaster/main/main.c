#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "led_strip.h"
#include "driver/ledc.h"
#include "freertos/timers.h"
#include "freertos/semphr.h"

static const char *TAG = "RGB";
SemaphoreHandle_t GlobalKey = 0;
uint8_t Enable_isr = 1;

#define STACK_SIZE 1024 * 2
#define BLINK_GPIO 8
#define CONFIG_BLINK_LED_RMT_CHANNEL 0
#define RGB_MAX_QTTY 12
#define MODES_MAX_QTTY 14
#define Default_Color 0
#define Default_Dimmer_Level 40

#define Button_Dimmer_Up 5
#define Button_Dimmer_Dowm 4
#define Button_Mode 7
#define Button_Color 6

uint16_t start_rgb = 0;
uint32_t red = 0;
uint32_t green = 0;
uint32_t blue = 0;
uint16_t hue = 0;

typedef enum Operation_Mode_t
{
    Mode_Off = 0,
    Mode_Single_Color,
    Mode_Change_Color,
    Mode_Single_Color_Auto_Dimmer,
    Mode_Change_Color_Auto_Dimmer,
    Mode_Single_Color_One_Bit,
    Mode_Change_Color_One_Bit,
    Mode_Change_Color_One_Bit_No_Clear,
    Mode_Change_Multi_Color_One_Bit,
    Mode_Shift_Color,
    Mode_Flash_Single_Color,
    Mode_Flash_Change_Color,
    MODE_RAINBOW,
    Mode_Zodiac_clock_Single_COlor,
    Mode_Zodiac_clock_Change_COlor
} Operation_Mode_t;

typedef struct Colors
{
    char Name[50];
    uint8_t R;
    uint8_t G;
    uint8_t B;
} Color_t;

typedef struct
{
    int8_t Dimmer_Level;
    int Transaction_Time;
    int Dimmer_Increase_Time;
    int8_t Dimmer_Increase_Value;
    int Flash_Time;
    Operation_Mode_t Mode;
    Operation_Mode_t Last_Mode;
    uint8_t color_index;
    uint8_t Multi_Color_Index;
    union
    {
        Color_t Color;
    };

} RGB_Control_t;

Color_t colorList[7];
RGB_Control_t RGB_Control;

led_strip_handle_t led_strip;

void isr_handler_button_dimemer_up(void *args);
void isr_handler_button_dimemer_down(void *args);
void isr_handler_button_mode(void *args);
void isr_handler_button_color(void *args);

static void create_colors()
{
    strcpy(colorList[0].Name, "Red");
    colorList[0].R = 255;
    colorList[0].G = 0;
    colorList[0].B = 0;

    strcpy(colorList[1].Name, "Green");
    colorList[1].R = 0;
    colorList[1].G = 255;
    colorList[1].B = 0;

    strcpy(colorList[2].Name, "Blue");
    colorList[2].R = 0;
    colorList[2].G = 0;
    colorList[2].B = 255;

    strcpy(colorList[3].Name, "Yellow");
    colorList[3].R = 255;
    colorList[3].G = 255;
    colorList[3].B = 0;

    strcpy(colorList[4].Name, "Purple");
    colorList[4].R = 255;
    colorList[4].G = 0;
    colorList[4].B = 255;

    strcpy(colorList[5].Name, "Cyan");
    colorList[5].R = 0;
    colorList[5].G = 255;
    colorList[5].B = 255;

    strcpy(colorList[6].Name, "white");
    colorList[6].R = 255;
    colorList[6].G = 255;
    colorList[6].B = 255;
}

static void configure_led(void)
{
    ESP_LOGI(TAG, "Configured addressable LED!");

    /* LED strip initialization with the GPIO and pixels number*/
    led_strip_config_t strip_config = {
        .strip_gpio_num = BLINK_GPIO,             // The GPIO that connected to the LED strip's data line
        .max_leds = RGB_MAX_QTTY,                 // The number of LEDs in the strip,
        .led_pixel_format = LED_PIXEL_FORMAT_GRB, // Pixel format of your LED strip
        .led_model = LED_MODEL_WS2812,            // LED strip model
        .flags.invert_out = false,                // whether to invert the output signal (useful when your hardware has a level inverter)
    };

    led_strip_rmt_config_t rmt_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT,    // different clock source can lead to different power consumption
        .resolution_hz = 10 * 1000 * 1000, // 10MHz
        .flags.with_dma = false,           // whether to enable the DMA feature
    };
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
}

static void init_rgb_control(void)
{
    RGB_Control.Mode = Mode_Zodiac_clock_Single_COlor;
    RGB_Control.Last_Mode = Mode_Zodiac_clock_Single_COlor;
    RGB_Control.Color = colorList[Default_Color];
    RGB_Control.color_index = Default_Color;
    RGB_Control.Dimmer_Level = Default_Dimmer_Level;
    RGB_Control.Transaction_Time = 1000;
    RGB_Control.Dimmer_Increase_Time = 200;
    RGB_Control.Dimmer_Increase_Value = 10;
    RGB_Control.Flash_Time = 100;
    RGB_Control.Multi_Color_Index = 0;
}

static void led_strip_hsv2rgb(uint32_t h, uint32_t s, uint32_t v, uint32_t *r, uint32_t *g, uint32_t *b)
{
    h %= 360; // h -> [0,360]
    uint32_t rgb_max = v * 2.55f;
    uint32_t rgb_min = rgb_max * (100 - s) / 100.0f;

    uint32_t i = h / 60;
    uint32_t diff = h % 60;

    // RGB adjustment amount by hue
    uint32_t rgb_adj = (rgb_max - rgb_min) * diff / 60;

    switch (i)
    {
    case 0:
        *r = rgb_max;
        *g = rgb_min + rgb_adj;
        *b = rgb_min;
        break;
    case 1:
        *r = rgb_max - rgb_adj;
        *g = rgb_max;
        *b = rgb_min;
        break;
    case 2:
        *r = rgb_min;
        *g = rgb_max;
        *b = rgb_min + rgb_adj;
        break;
    case 3:
        *r = rgb_min;
        *g = rgb_max - rgb_adj;
        *b = rgb_max;
        break;
    case 4:
        *r = rgb_min + rgb_adj;
        *g = rgb_min;
        *b = rgb_max;
        break;
    default:
        *r = rgb_max;
        *g = rgb_min;
        *b = rgb_max - rgb_adj;
        break;
    }
}
static void init_irs(void)
{
    gpio_config_t pGPIOConfig;
    pGPIOConfig.pin_bit_mask = (1ULL << Button_Dimmer_Up) | (1ULL << Button_Dimmer_Dowm) | (1ULL << Button_Color) | (1ULL << Button_Mode);
    pGPIOConfig.mode = GPIO_MODE_DEF_INPUT;
    pGPIOConfig.pull_up_en = GPIO_PULLUP_ENABLE;
    pGPIOConfig.pull_down_en = GPIO_PULLDOWN_DISABLE;
    pGPIOConfig.intr_type = GPIO_INTR_NEGEDGE;

    gpio_config(&pGPIOConfig);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(Button_Dimmer_Up, isr_handler_button_dimemer_up, NULL);
    gpio_isr_handler_add(Button_Dimmer_Dowm, isr_handler_button_dimemer_down, NULL);
    gpio_isr_handler_add(Button_Color, isr_handler_button_color, NULL);
    gpio_isr_handler_add(Button_Mode, isr_handler_button_mode, NULL);

    ESP_LOGI(TAG, "Init isr completed");
}

void isr_handler_button_mode(void *args)
{
    if (Enable_isr == 1)
    {
        xSemaphoreGive(GlobalKey);
        RGB_Control.Mode += 1;
        if (RGB_Control.Mode > MODES_MAX_QTTY)
        {
            RGB_Control.Mode = 0;
        }
    }
}

void isr_handler_button_color(void *args)
{
    if (Enable_isr == 1)
    {
        xSemaphoreGive(GlobalKey);

        RGB_Control.color_index += 1;
        if (RGB_Control.color_index > sizeof(colorList) / sizeof(*colorList) - 1)
        {
            RGB_Control.color_index = Default_Color;
        }

        RGB_Control.Color = colorList[RGB_Control.color_index];
    }
}

void isr_handler_button_dimemer_up(void *args)
{
    if (Enable_isr == 1)
    {
        xSemaphoreGive(GlobalKey);
        if (RGB_Control.Mode == Mode_Flash_Single_Color || RGB_Control.Mode == Mode_Flash_Change_Color)
        {
            RGB_Control.Flash_Time += 10;
            if (RGB_Control.Flash_Time > 2000)
            {
                RGB_Control.Flash_Time = 2000;
            }
        }
        else
        {
            RGB_Control.Dimmer_Level += RGB_Control.Dimmer_Increase_Value;
            if (RGB_Control.Dimmer_Level > 100)
            {
                RGB_Control.Dimmer_Level = 100;
            }
        }
    }
}

void isr_handler_button_dimemer_down(void *args)
{
    if (Enable_isr == 1)
    {
        xSemaphoreGive(GlobalKey);
        if (RGB_Control.Mode == Mode_Flash_Single_Color || RGB_Control.Mode == Mode_Flash_Change_Color)
        {
            RGB_Control.Flash_Time -= 10;
            if (RGB_Control.Flash_Time < 10)
            {
                RGB_Control.Flash_Time = 10;
            }
        }
        else
        {
            RGB_Control.Dimmer_Level -= RGB_Control.Dimmer_Increase_Value;
            if (RGB_Control.Dimmer_Level < 1)
            {
                RGB_Control.Dimmer_Level = 1;
            }
        }
    }
}

static void change_color(void)
{
    RGB_Control.color_index += 1;
    if (RGB_Control.color_index > sizeof(colorList) / sizeof(*colorList) - 1)
    {
        RGB_Control.color_index = Default_Color;
    }

    RGB_Control.Color = colorList[RGB_Control.color_index];
}

static void vTask_Evaluate_inputs(void *pvParameters)
{
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(RGB_Control.Transaction_Time));

        switch (RGB_Control.Mode)
        {

        case Mode_Change_Color:
        case Mode_Flash_Change_Color:

            change_color();

            break;

        default:
            break;
        }
    }
}

static void vTask_Evaluate_modes(void *pvParameters)
{
    while (1)
    {
        ESP_LOGI(TAG, "Mode: %d Level: %d Index: %d R: %i G: %i B: %i Color: %s", RGB_Control.Mode, RGB_Control.Dimmer_Level, RGB_Control.color_index, RGB_Control.Color.R, RGB_Control.Color.G, RGB_Control.Color.B, RGB_Control.Color.Name);

        vTaskDelay(pdMS_TO_TICKS(RGB_Control.Transaction_Time));
        RGB_Control.Last_Mode = RGB_Control.Mode;

        switch (RGB_Control.Mode)
        {
        case Mode_Off:
            led_strip_clear(led_strip);
            break;
        case Mode_Single_Color:
        case Mode_Change_Color:
            for (size_t i = 0; i < RGB_MAX_QTTY; i++)
            {
                led_strip_set_pixel(led_strip, i, (RGB_Control.Color.R * RGB_Control.Dimmer_Level / 100),
                                    (RGB_Control.Color.G * RGB_Control.Dimmer_Level / 100),
                                    (RGB_Control.Color.B * RGB_Control.Dimmer_Level / 100));
            }
            led_strip_refresh(led_strip);

            break;

        case Mode_Single_Color_Auto_Dimmer:
        case Mode_Change_Color_Auto_Dimmer:
            for (size_t j = 1; j < 130; j += j)
            {
                if (j > 100)
                {
                    j = 100;
                }

                for (size_t i = 0; i < RGB_MAX_QTTY; i++)
                {
                    if (RGB_Control.Mode != RGB_Control.Last_Mode)
                    {
                        break;
                    }

                    led_strip_set_pixel(led_strip, i, (RGB_Control.Color.R * j / 100),
                                        (RGB_Control.Color.G * j / 100),
                                        (RGB_Control.Color.B * j / 100));
                }
                led_strip_refresh(led_strip);
                vTaskDelay(pdMS_TO_TICKS(RGB_Control.Dimmer_Increase_Time));
            }

            if (RGB_Control.Mode == Mode_Change_Color_Auto_Dimmer)
            {
                change_color();
            }

            break;

        case Mode_Single_Color_One_Bit:
        case Mode_Change_Color_One_Bit:
        case Mode_Change_Color_One_Bit_No_Clear:
        case Mode_Change_Multi_Color_One_Bit:

            if (RGB_Control.Mode != Mode_Change_Color_One_Bit_No_Clear)
            {
                led_strip_clear(led_strip);
            }

            for (size_t i = 0; i < RGB_MAX_QTTY; i++)
            {
                if (RGB_Control.Mode != RGB_Control.Last_Mode)
                {
                    break;
                }

                led_strip_set_pixel(led_strip, i, (RGB_Control.Color.R * RGB_Control.Dimmer_Level / 100),
                                    (RGB_Control.Color.G * RGB_Control.Dimmer_Level / 100),
                                    (RGB_Control.Color.B * RGB_Control.Dimmer_Level / 100));
                led_strip_refresh(led_strip);
                vTaskDelay(pdMS_TO_TICKS(RGB_Control.Dimmer_Increase_Time * 1 / 4));

                if (RGB_Control.Mode == Mode_Change_Multi_Color_One_Bit)
                {
                    RGB_Control.Multi_Color_Index += 1;
                    if (RGB_Control.Multi_Color_Index > 5)
                    {
                        RGB_Control.Multi_Color_Index = 0;
                        change_color();
                    }
                }
            }

            if (RGB_Control.Mode == Mode_Change_Color_One_Bit_No_Clear || RGB_Control.Mode == Mode_Change_Color_One_Bit)
            {
                change_color();
            }

            break;

        case Mode_Shift_Color:
            while (RGB_Control.Mode == Mode_Shift_Color)
            {
                for (size_t i = 0; i < RGB_MAX_QTTY; i++)
                {
                    if (RGB_Control.Mode != RGB_Control.Last_Mode)
                    {
                        break;
                    }

                    led_strip_set_pixel(led_strip, i, (RGB_Control.Color.R * RGB_Control.Dimmer_Level / 100),
                                        (RGB_Control.Color.G * RGB_Control.Dimmer_Level / 100),
                                        (RGB_Control.Color.B * RGB_Control.Dimmer_Level / 100));

                    RGB_Control.Multi_Color_Index += 1;
                    if (RGB_Control.Multi_Color_Index > 5)
                    {
                        RGB_Control.Multi_Color_Index = 0;
                        change_color();
                    }
                }
                led_strip_refresh(led_strip);
                vTaskDelay(pdMS_TO_TICKS(200));
            }

            break;

        case Mode_Flash_Single_Color:
        case Mode_Flash_Change_Color:

            while (RGB_Control.Mode == Mode_Flash_Single_Color || RGB_Control.Mode == Mode_Flash_Change_Color)
            {
                for (size_t i = 0; i < RGB_MAX_QTTY; i++)
                {
                    led_strip_set_pixel(led_strip, i, (RGB_Control.Color.R), (RGB_Control.Color.G), (RGB_Control.Color.B));
                }

                led_strip_refresh(led_strip);
                vTaskDelay(pdMS_TO_TICKS(RGB_Control.Flash_Time / 2));
                led_strip_clear(led_strip);
                vTaskDelay(pdMS_TO_TICKS(RGB_Control.Flash_Time * 2));
            }

            break;
        case MODE_RAINBOW:
            while (RGB_Control.Mode == MODE_RAINBOW)
            {
                for (int i = 0; i < 3; i++)
                {
                    for (int j = i; j < RGB_MAX_QTTY; j += 3)
                    {
                        if (RGB_Control.Mode != RGB_Control.Last_Mode)
                        {
                            break;
                        }
                        hue = j * 360 / RGB_MAX_QTTY + start_rgb;
                        led_strip_hsv2rgb(hue, 100, 100, &red, &green, &blue);

                        led_strip_set_pixel(led_strip, j,
                                            (red * RGB_Control.Dimmer_Level / 100),
                                            (green * RGB_Control.Dimmer_Level / 100),
                                            (blue * RGB_Control.Dimmer_Level / 100));
                    }

                    ESP_ERROR_CHECK(led_strip_refresh(led_strip));
                    vTaskDelay(pdMS_TO_TICKS(80));
                }
                start_rgb += 60;
            }

            break;
        case Mode_Zodiac_clock_Single_COlor:
        case Mode_Zodiac_clock_Change_COlor:

            for (size_t i = 0; i < RGB_MAX_QTTY; i++)
            {
                led_strip_set_pixel(led_strip, i, (RGB_Control.Color.R * RGB_Control.Dimmer_Level / 100),
                                        (RGB_Control.Color.G * RGB_Control.Dimmer_Level / 100),
                                        (RGB_Control.Color.B * RGB_Control.Dimmer_Level / 100));
            }
            led_strip_refresh(led_strip);

            uint8_t count = 0;
            for (size_t i = 0; i < RGB_MAX_QTTY; i++)
            {
                for (size_t j = 105; j > 0; j -= 5)
                { 
                    led_strip_set_pixel(led_strip, count, (RGB_Control.Color.R * (j-5) / 100),
                                        (RGB_Control.Color.G * (j-5) / 100),
                                        (RGB_Control.Color.B * (j-5) / 100));

                    led_strip_refresh(led_strip);
                    vTaskDelay(pdMS_TO_TICKS(RGB_Control.Dimmer_Increase_Time));
                    if(RGB_Control.Mode != Mode_Zodiac_clock_Single_COlor && RGB_Control.Mode != Mode_Zodiac_clock_Change_COlor )
                    {
                        break;
                    }
                }  

                count++;                
            }

            if (RGB_Control.Mode == Mode_Zodiac_clock_Change_COlor)
            {
                change_color();
            }
            break;

        default:
            break;
        }
    }
}

static void vTask_Evaluate_irs(void *pvParameters)
{
    while (1)
    {
        if (xSemaphoreTake(GlobalKey, portMAX_DELAY))
        {
            Enable_isr = 0;
        }

        vTaskDelay(pdMS_TO_TICKS(500));
        Enable_isr = 1;
    }
}

static void create_tasks(void)
{
    static uint8_t ucParameterToPass;
    TaskHandle_t xHandle = NULL;

    xTaskCreate(vTask_Evaluate_inputs,
                "vTask_Evaluate_inputs",
                STACK_SIZE,
                &ucParameterToPass,
                1,
                &xHandle);
    xTaskCreate(vTask_Evaluate_modes,
                "vTask_Evaluate_modes",
                STACK_SIZE,
                &ucParameterToPass,
                1,
                &xHandle);
    xTaskCreate(vTask_Evaluate_irs,
                "vTask_Evaluate_irs",
                STACK_SIZE,
                &ucParameterToPass,
                1,
                &xHandle);
}

void app_main(void)
{
    GlobalKey = xSemaphoreCreateBinary();
    create_colors();
    configure_led();
    init_irs();
    init_rgb_control();
    create_tasks();
}