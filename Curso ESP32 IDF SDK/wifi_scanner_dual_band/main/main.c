/* WiFi Scan Example
    Lists available WiFi networks (2.4GHz and 5GHz) in the console.
    This example initializes the WiFi in station mode, scans for available networks,
    and prints their SSID, RSSI, channel, and band type (2.4GHz or 5GHz).
*/
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_mac.h"

static const char *TAG = "wifi_scan";

// Initialize WiFi in station mode
void wifi_init_sta(void)
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "wifi_init_sta finished.");
}

// Scan and print available WiFi networks (2.4GHz and 5GHz)
static void wifi_scan_and_print(void)
{
    uint16_t ap_num = 0;
    wifi_ap_record_t ap_records[20];
    wifi_scan_config_t scan_config = {
        .ssid = 0,
        .bssid = 0,
        .channel = 0,
        .show_hidden = false,
        .scan_type = WIFI_SCAN_TYPE_ACTIVE,
        .scan_time.active.min = 100,
        .scan_time.active.max = 500,
    };

    ESP_ERROR_CHECK(esp_wifi_scan_start(&scan_config, true));
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_num));
    if (ap_num > 20)
        ap_num = 20;
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&ap_num, ap_records));

    printf("\nAvailable WiFi networks:\n");
    printf("%-32s %-4s %-6s %-8s %-17s\n", "SSID", "RSSI", "Channel", "Band", "MAC");
    for (int i = 0; i < ap_num; i++)
    {
        const char *band = (ap_records[i].primary <= 14) ? "2.4 GHz" : "5 GHz";
        printf("%-32s %-4d %-6d %-8s " MACSTR "\n",
            (char *)ap_records[i].ssid,
            ap_records[i].rssi,
            ap_records[i].primary,
            band,
            MAC2STR(ap_records[i].bssid));
    }
}

void app_main(void)
{
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    wifi_init_sta();
    wifi_scan_and_print();
}