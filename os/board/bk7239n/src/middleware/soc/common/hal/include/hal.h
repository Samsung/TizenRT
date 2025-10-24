// Copyright 2020-2021 Beken
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

#include <soc/soc.h>
#include <common/bk_err.h>

/* Ideally HAL only depends on ll and soc, but it's do helpful to provide
 * debug mechanism for HAL. For ones who need to port to HAL to other platform,
 * all you need to do is to define the HAL_LOGx to empty macro.
 *
 **/
#include <components/log.h>

#define HAL_TAG "hal"
#define HAL_LOGI(...) BK_LOGI(HAL_TAG, ##__VA_ARGS__)
#define HAL_LOGW(...) BK_LOGW(HAL_TAG, ##__VA_ARGS__)
#define HAL_LOGE(...) BK_LOGE(HAL_TAG, ##__VA_ARGS__)
#define HAL_LOGD(...) BK_LOGD(HAL_TAG, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif
