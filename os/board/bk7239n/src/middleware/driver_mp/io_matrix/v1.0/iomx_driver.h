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

#define IOMX_TAG "iomx"
#define IOMX_LOGI(...) BK_LOGI(IOMX_TAG, ##__VA_ARGS__)
#define IOMX_LOGW(...) BK_LOGW(IOMX_TAG, ##__VA_ARGS__)
#define IOMX_LOGE(...) BK_LOGE(IOMX_TAG, ##__VA_ARGS__)
#define IOMX_LOGD(...) BK_LOGD(IOMX_TAG, ##__VA_ARGS__)

bk_err_t bk_iomx_driver_init(void);
uint32_t bk_iomx_get_gpio_func_code(uint32_t gpio_id);
bk_err_t bk_iomx_set_gpio_func(uint32_t gpio_id, IOMX_CODE_T func_code);
bk_err_t bk_iomx_driver_deinit(void);
#if defined(CONFIG_GPIO_WAKEUP_SUPPORT)
bk_err_t iomx_enter_low_power(void);
bk_err_t iomx_exit_low_power(void);
#endif
// eof

