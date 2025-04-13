#include "iot/thing.h"
#include "board.h"
#include "audio_codec.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <driver/gpio.h>
#include <esp_log.h>

#define TAG "Lamp"

namespace iot {

// 这里仅定义 Lamp 的属性和方法，不包含具体的实现
class Lamp : public Thing {
private:
#ifdef CONFIG_IDF_TARGET_ESP32
    gpio_num_t gpio_num_ = GPIO_NUM_12;
#else
    gpio_num_t gpio_num_ = GPIO_NUM_12;
#endif
    bool power_ = false;

    void InitializeGpio() {
        gpio_config_t config = {
            .pin_bit_mask = (1ULL << gpio_num_),
            .mode = GPIO_MODE_OUTPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_DISABLE,
        };
        ESP_ERROR_CHECK(gpio_config(&config));
        gpio_set_level(gpio_num_, 0);
    }

    void PulseGpio() {
        gpio_set_level(gpio_num_, 1);             // 拉高
        vTaskDelay(pdMS_TO_TICKS(1000));           // 延迟 100ms
        gpio_set_level(gpio_num_, 0);             // 恢复低电平
    }
public:
    Lamp() : Thing("热风", "一个测试用的热风"), power_(false) {
        InitializeGpio();

        // 定义设备的属性
        properties_.AddBooleanProperty("power", "热风是否打开", [this]() -> bool {
            return power_;
        });

        // 定义设备可以被远程执行的指令
        methods_.AddMethod("TurnOn", "打开热风", ParameterList(), [this](const ParameterList& parameters) {
            power_ = true;
            PulseGpio();
        });

        methods_.AddMethod("TurnOff", "关闭热风", ParameterList(), [this](const ParameterList& parameters) {
            power_ = false;
            PulseGpio();
        });
    }
};

} // namespace iot

DECLARE_THING(Lamp);
