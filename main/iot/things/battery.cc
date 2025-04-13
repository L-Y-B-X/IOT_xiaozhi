#include "iot/thing.h"
#include "board.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>

#define TAG "Battery"

namespace iot {

class Battery : public Thing {
private:
    gpio_num_t gpio_cool_ = GPIO_NUM_11;  // 冷风引脚
    gpio_num_t gpio_heat_ = GPIO_NUM_12;  // 热风引脚

    void InitializeGpio(gpio_num_t gpio_num) {
        gpio_config_t config = {
            .pin_bit_mask = (1ULL << gpio_num),
            .mode = GPIO_MODE_OUTPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_DISABLE,
        };
        ESP_ERROR_CHECK(gpio_config(&config));
        gpio_set_level(gpio_num, 0);
    }

    void PulseGpio(gpio_num_t gpio_num) {
        gpio_set_level(gpio_num, 1);
        vTaskDelay(pdMS_TO_TICKS(1000));
        gpio_set_level(gpio_num, 0);
    }

public:
    Battery() : Thing("冷热风", "控制冷热风的功能") {
        InitializeGpio(gpio_cool_);
        InitializeGpio(gpio_heat_);

        // 冷风指令
        methods_.AddMethod("冷风", "打开冷风", ParameterList(), [this](const ParameterList&) {
            ESP_LOGI(TAG, "执行冷风");
            PulseGpio(gpio_cool_);
        });

        methods_.AddMethod("关闭冷风", "关闭冷风", ParameterList(), [this](const ParameterList&) {
            ESP_LOGI(TAG, "关闭冷风");
            PulseGpio(gpio_cool_);
        });

        // 热风指令
        methods_.AddMethod("热风", "打开热风", ParameterList(), [this](const ParameterList&) {
            ESP_LOGI(TAG, "执行热风");
            PulseGpio(gpio_heat_);
        });

        methods_.AddMethod("关闭热风", "关闭热风", ParameterList(), [this](const ParameterList&) {
            ESP_LOGI(TAG, "关闭热风");
            PulseGpio(gpio_heat_);
        });
    }
};

} // namespace iot

DECLARE_THING(Battery);
