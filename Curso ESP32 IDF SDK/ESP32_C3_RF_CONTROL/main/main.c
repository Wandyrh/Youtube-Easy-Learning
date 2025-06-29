#include <stdio.h>
#include <string.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "led_strip.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_now.h"
#include "esp_wifi.h"
#include "esp_netif.h"
#include "esp_mac.h"
#include "esp_event.h"
#include "nvs_flash.h"

#define LED_STRIP 10
#define LED_STRIP_MAX_LEDS 1

#define ESP_NOW_PMK "pmk1234567890123"
#define ESP_NOW_LMK "lmk1234567890123"
#define ESP_CHANNEL 1

static uint8_t peer_mac[ESP_NOW_ETH_ALEN] = {0xdc, 0xda, 0x0c, 0x87, 0x7d, 0xd4};
// static uint8_t peer_mac[ESP_NOW_ETH_ALEN] = {0x54, 0x32, 0x04, 0x4b, 0xcc, 0x1c};
adc_oneshot_unit_handle_t ret_unit;
static const char *TAG = "RF-CONTROL";
led_strip_handle_t led_strip;

static esp_err_t esp_now_send_data(const uint8_t *peer_addr, const uint8_t *data, const uint8_t len);

static esp_err_t set_adc(void)
{
    adc_oneshot_unit_init_cfg_t init_config = {};
    init_config.unit_id = ADC_UNIT_1;    
    init_config.ulp_mode = ADC_ULP_MODE_DISABLE;
    adc_oneshot_new_unit(&init_config, &ret_unit);
    adc_oneshot_chan_cfg_t config = {};
    config.atten = ADC_ATTEN_DB_11;
    config.bitwidth = ADC_BITWIDTH_DEFAULT;
    adc_oneshot_config_channel(ret_unit, ADC_CHANNEL_0, &config);
    adc_oneshot_config_channel(ret_unit, ADC_CHANNEL_1, &config);
    return ESP_OK;
}

static esp_err_t stop(void)
{  
    led_strip_set_pixel(led_strip, 0, 20, 0, 0);
    led_strip_refresh(led_strip);
    ESP_LOGI(TAG, "stop completed");
    return ESP_OK;
}

static esp_err_t move_forward(void)
{   
    led_strip_set_pixel(led_strip, 0, 0, 20, 0);
    led_strip_refresh(led_strip);
    ESP_LOGI(TAG, "move_forward completed");
    return ESP_OK;
}
static esp_err_t move_back(void)
{    
    led_strip_set_pixel(led_strip, 0, 0, 0, 20);
    led_strip_refresh(led_strip);
    ESP_LOGI(TAG, "move_back completed");
    return ESP_OK;
}

static esp_err_t turn_right(void)
{    
    led_strip_set_pixel(led_strip, 0, 20, 20, 0);
    led_strip_refresh(led_strip);
    ESP_LOGI(TAG, "turn_right completed");
    return ESP_OK;
}
static esp_err_t turn_left(void)
{    
    led_strip_set_pixel(led_strip, 0, 0, 20, 20);
    led_strip_refresh(led_strip);
    ESP_LOGI(TAG, "turn_left completed");
    return ESP_OK;
}

static void esp_now_recv_cb(const esp_now_recv_info_t *esp_now_info, const uint8_t *data, int data_len)
{

    ESP_LOGI(TAG, "esp_now_recv_cb:" MACSTR ": %s", MAC2STR(esp_now_info->src_addr), data);    
}

static void esp_now_send_cb(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    if (status == ESP_NOW_SEND_SUCCESS)
    {
        ESP_LOGI(TAG, "ESP_NOW_SEND_SUCCESS " MACSTR "", MAC2STR(mac_addr));
    }
    else
    {
        ESP_LOGI(TAG, "ESP_NOW_SEND fails");
    }
}
static esp_err_t init_wifi(void)
{
    wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
    esp_netif_init();
    esp_event_loop_create_default();
    esp_wifi_init(&wifi_init_config);
    esp_wifi_set_storage(WIFI_STORAGE_RAM);
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_start();
    ESP_LOGI(TAG, "wifi init completed");

    return ESP_OK;
}

static esp_err_t init_esp_now(void)
{
    esp_now_init();
    esp_now_register_recv_cb(esp_now_recv_cb);
    esp_now_register_send_cb(esp_now_send_cb);
    esp_now_set_pmk((uint8_t *)ESP_NOW_PMK);
    return ESP_OK;
}

static esp_err_t register_peer(uint8_t *peer_addr)
{
    esp_now_peer_info_t esp_now_peer_info = {};
    memcpy(esp_now_peer_info.peer_addr, peer_addr, ESP_NOW_ETH_ALEN);
    esp_now_peer_info.channel = ESP_CHANNEL;
    esp_now_peer_info.ifidx = ESP_IF_WIFI_STA;
    memcpy(esp_now_peer_info.lmk, ESP_NOW_LMK, ESP_NOW_KEY_LEN);
    esp_now_peer_info.encrypt = true;

    esp_now_add_peer(&esp_now_peer_info);
    return ESP_OK;
}

static esp_err_t esp_now_send_data(const uint8_t *peer_addr, const uint8_t *data, const uint8_t len)
{
    esp_now_send(peer_addr, data, len);
    return ESP_OK;
}

static esp_err_t init_led_strip(void)
{
    /* LED strip initialization with the GPIO and pixels number*/
    led_strip_config_t strip_config = {
        .strip_gpio_num = LED_STRIP,              // The GPIO that connected to the LED strip's data line
        .max_leds = LED_STRIP_MAX_LEDS,           // The number of LEDs in the strip,
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
    ESP_LOGI(TAG, "init_led_strip completed");
    return ESP_OK;
}

static esp_err_t set_motors(int x, int y)
{
    uint8_t m_stop[] = "0";
    uint8_t m_move_forward[] = "1";
    uint8_t m_move_back[] = "2";
    uint8_t m_turn_left[] = "3";
    uint8_t m_turn_right[] = "4";    

    if(y < 1000)
    {
        move_forward();
        esp_now_send_data(peer_mac, m_move_forward, 1);
        return ESP_OK;
    }
    if(y > 3000)
    {
        move_back();
        esp_now_send_data(peer_mac, m_move_back, 1);
        return ESP_OK;
    }

    if(x < 1000)
    {
        turn_left();
        esp_now_send_data(peer_mac, m_turn_left, 1);
        return ESP_OK;
    }
    if(x > 3000)
    {
        turn_right();
        esp_now_send_data(peer_mac, m_turn_right, 1);
        return ESP_OK;
    }

    stop();
    esp_now_send_data(peer_mac, m_stop, 1);

    return ESP_OK;
}

void app_main(void)
{    
    init_led_strip();
    stop();
    nvs_flash_init();
    ESP_ERROR_CHECK(init_wifi());
    ESP_ERROR_CHECK(init_esp_now());
    ESP_ERROR_CHECK(register_peer(peer_mac));
    set_adc();    
   
    while (1)
    {        
       
        vTaskDelay(pdMS_TO_TICKS(200));
        int adc_y = 0;
        int adc_x = 0;
        adc_oneshot_read(ret_unit, ADC_CHANNEL_0, &adc_y);
        ESP_LOGI(TAG, "adc_y: %i", adc_y);
        adc_oneshot_read(ret_unit, ADC_CHANNEL_1, &adc_x);
        ESP_LOGI(TAG, "adc_x: %i", adc_x);
        
        set_motors(adc_x, adc_y);
    }
}
