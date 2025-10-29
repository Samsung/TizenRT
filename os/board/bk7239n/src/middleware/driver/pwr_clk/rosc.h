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

#define ROSC_TAG "rosc"
#define ROSC_LOGI(...) BK_LOGI(ROSC_TAG, ##__VA_ARGS__)
#define ROSC_LOGW(...) BK_LOGW(ROSC_TAG, ##__VA_ARGS__)
#define ROSC_LOGE(...) BK_LOGE(ROSC_TAG, ##__VA_ARGS__)
#define ROSC_LOGD(...) BK_LOGD(ROSC_TAG, ##__VA_ARGS__)
#define ROSC_LOGV(...) BK_LOGV(ROSC_TAG, ##__VA_ARGS__)

#define ROSC_CALIB_DEBUG 0
#define ROSC_COMPE_DEBUG 0

typedef struct rosc_calib_record
{
	uint8_t progress_count;
	uint32_t cin0;
	uint32_t cin1;
	float freq;
	float loss;
} rosc_calib_record_t;

static bk_err_t rosc_calib_records_insert(rosc_calib_record_t* record);
static void rosc_calib_records_dump(void);
