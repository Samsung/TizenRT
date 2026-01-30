// Copyright 2022-2025 Beken
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

#ifdef __cplusplus
extern "C" {
#endif

#include <components/log.h>

#define SDMADC_TAG "sdmadc"
#define SDMADC_LOGI(...) BK_LOGI(SDMADC_TAG, ##__VA_ARGS__)
#define SDMADC_LOGW(...) BK_LOGW(SDMADC_TAG, ##__VA_ARGS__)
#define SDMADC_LOGE(...) BK_LOGE(SDMADC_TAG, ##__VA_ARGS__)
#define SDMADC_LOGD(...) BK_LOGD(SDMADC_TAG, ##__VA_ARGS__)

typedef enum
{
    SDMADC_CALIBRATE_LOW,
    SDMADC_CALIBRATE_HIGH
} SDMADC_MODE;

bk_err_t bk_sdmadc_set_calibrate_val(uint16_t value, SDMADC_MODE mode);

#ifdef __cplusplus
}
#endif
