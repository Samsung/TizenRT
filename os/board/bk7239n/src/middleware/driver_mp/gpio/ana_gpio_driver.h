// Copyright 2020-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once
#include <components/log.h>
#include <driver/gpio_types.h>

#define ANA_GPIO_TAG "ana_gpio"
#define ANA_GPIO_LOGI(...) BK_LOGI(ANA_GPIO_TAG, ##__VA_ARGS__)
#define ANA_GPIO_LOGW(...) BK_LOGW(ANA_GPIO_TAG, ##__VA_ARGS__)
#define ANA_GPIO_LOGE(...) BK_LOGE(ANA_GPIO_TAG, ##__VA_ARGS__)
#define ANA_GPIO_LOGD(...) BK_LOGD(ANA_GPIO_TAG, ##__VA_ARGS__)


bk_err_t ana_gpio_wakeup_init(void);
gpio_id_t ana_gpio_get_wakeup_pin(void);
bk_err_t ana_gpio_isr_register(gpio_id_t gpio_id, gpio_isr_t isr);
bk_err_t ana_gpio_register_wakeup_source(gpio_id_t gpio_id);
bk_err_t ana_gpio_unregister_wakeup_source(gpio_id_t gpio_id);
bk_err_t ana_gpio_config_wakeup_source(uint64_t gpio_bitmap);
bk_err_t ana_gpio_clear_wakeup_source(void);


#if defined(CONFIG_GPIO_WAKEUP_SUPPORT)
typedef struct {
	gpio_id_t id;
	gpio_int_type_t int_type;
} gpio_wakeup_t;
#endif