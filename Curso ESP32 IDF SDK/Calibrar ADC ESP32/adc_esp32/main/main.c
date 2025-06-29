#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_adc/adc_oneshot.h>
#include <esp_adc/adc_cali_scheme.h>
#include <esp_adc/adc_cali.h>
#include <esp_log.h>

adc_oneshot_unit_handle_t adc_oneshot_unit_handle;
int adc_gpio = 0;
const static char *TAG = "ADC one shot";

static void set_adc()
{
    adc_oneshot_unit_init_cfg_t adc_oneshot_unit_init_cfg = {
        .ulp_mode = ADC_ULP_MODE_DISABLE,
        .unit_id = ADC_UNIT_1};

    adc_oneshot_new_unit(&adc_oneshot_unit_init_cfg, &adc_oneshot_unit_handle);

    adc_unit_t adc_unit;
    adc_channel_t adc_channel;
    adc_oneshot_io_to_channel(adc_gpio, &adc_unit, &adc_channel);

    ESP_LOGI(TAG, "ADC channel io: %i", adc_channel);

    adc_oneshot_chan_cfg_t adc_oneshot_chan_cfg = {
        .atten = ADC_ATTEN_DB_11,
        .bitwidth = ADC_BITWIDTH_12};

    adc_oneshot_config_channel(adc_oneshot_unit_handle, adc_channel, &adc_oneshot_chan_cfg);
}

static int adc_read()
{
    adc_unit_t adc_unit;
    adc_channel_t adc_channel;
    int adc_row;
    adc_oneshot_io_to_channel(adc_gpio, &adc_unit, &adc_channel);
    adc_oneshot_read(adc_oneshot_unit_handle, adc_channel, &adc_row);
    return adc_row;
}

static int calibrate_adc(int row)
{

    adc_unit_t adc_unit;
    adc_channel_t adc_channel;
    adc_oneshot_io_to_channel(adc_gpio, &adc_unit, &adc_channel);

    adc_cali_curve_fitting_config_t adc_cali_curve_fitting_config = {
        .atten = ADC_ATTEN_DB_11,
        .bitwidth = ADC_BITWIDTH_12,
        .chan = adc_unit,
        .unit_id = adc_channel};

    adc_cali_handle_t adc_cali_handle;
    adc_cali_create_scheme_curve_fitting(&adc_cali_curve_fitting_config, &adc_cali_handle);
    int voltage = 0;
    adc_cali_raw_to_voltage(adc_cali_handle, row, &voltage);

    adc_cali_delete_scheme_curve_fitting(adc_cali_handle);

    return voltage;
}
void app_main(void)
{

    set_adc();

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(500));
        int adc_row = adc_read();
        ESP_LOGI(TAG, "ADC value: %i", adc_row);

        int adc_voltage = calibrate_adc(adc_row);
        ESP_LOGI(TAG, "ADC VOLTAGE: %i", adc_voltage);
    }
}
