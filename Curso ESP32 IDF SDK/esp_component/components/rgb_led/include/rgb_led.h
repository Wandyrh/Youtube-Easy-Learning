#include "esp_err.h"
#include "driver/gpio.h"
#include "esp_log.h"

/**
 * @brief RGB LED component initialization
 *
 * @param led_r: GPIO number corresponding to red color
 * @param led_g: GPIO number corresponding to grren color
 * @param led_b: GPIO number corresponding to blue color
 *
 * @return
 *      - ESP_OK: RGB LED component initialization successfully
 */
esp_err_t init_rgb_led(uint8_t led_r, uint8_t led_g, uint8_t led_b);
/**
 * @brief Set RGB LED to red color
 *
 * @return
 *      - ESP_OK: RGB LED was set to red
 */
esp_err_t set_rgb_red(void);
/**
 * @brief Set RGB LED to red color
 *
 * @return
 *      - ESP_OK: RGB LED was set to green
 */
esp_err_t set_rgb_green(void);
/**
 * @brief Set RGB LED to red color
 *
 * @return
 *      - ESP_OK: RGB LED was set to blue
 */
esp_err_t set_rgb_blue(void);
/**
 * @brief Set RGB LED to red color
 *
 * @return
 *      - ESP_OK: RGB LED was set to off
 */
esp_err_t set_rgb_off(void);
