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

/**
 * Ideally soc should NOT depends on any other modules, but it's really helpful
 * if we can debug soc directly.
 * For ones who want to port soc to other platform, just need to implement following
 * interfaces.
 */
#include <components/log.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SOC_TAG "soc"
#define SOC_LOGI(...) BK_LOGI(SOC_TAG, ##__VA_ARGS__)
#define SOC_LOGW(...) BK_LOGW(SOC_TAG, ##__VA_ARGS__)
#define SOC_LOGE(...) BK_LOGE(SOC_TAG, ##__VA_ARGS__)
#define SOC_LOGD(...) BK_LOGD(SOC_TAG, ##__VA_ARGS__)

#if CONFIG_BK_WHILE
#define CONFIG_LOOP_MAX_CNT    (1000)

/* TODO:
 * dump icu
 * dump system control
 * dump mac/phy registers
 * dump software module control info
 */
#define BK_WHILE(_condition) do {\
	uint32_t loop_cnt = 0;\
	while (_condition) {\
		loop_cnt ++;\
		if (!(loop_cnt % CONFIG_LOOP_MAX_CNT)) {\
			SOC_LOGW("potential dead loop: %u, %s:%d, cpsr: %x\r\n", loop_cnt, __FUNCTION__, __LINE__, rtos_get_cpsr());\
		}\
	}\
} while (0)

#define BK_DO_WHILE(_do, _condition) do {\
	uint32_t loop_cnt = 0;\
	do {\
		_do;\
		loop_cnt ++;\
		if (!(loop_cnt % CONFIG_LOOP_MAX_CNT)) {\
			SOC_LOGW("potential dead loop: %u, %s:%d, cpsr: %x\r\n", loop_cnt, __FUNCTION__, __LINE__, rtos_get_cpsr());\
		}\
	} while (_condition);\
} while (0)

#define BK_WHILE_DO(_condition, _do) do {\
	uint32_t loop_cnt = 0;\
	while (_condition) {\
		_do;\
		loop_cnt ++;\
		if (!(loop_cnt % CONFIG_LOOP_MAX_CNT)) {\
			SOC_LOGW("potential dead loop: %u, %s:%d, cpsr: %x\r\n", loop_cnt, __FUNCTION__, __LINE__, rtos_get_cpsr());\
		}\
	}\
} while (0)
#else
#define BK_WHILE(_condition) while(_condition)

#define BK_DO_WHILE(_do, _condition) do {\
	do {\
		_do;\
	} while (_condition);\
} while (0)

#define BK_WHILE_DO(_condition, _do) do {\
	while (_condition) {\
		_do;\
	}\
} while (0)
#endif

#ifdef __cplusplus
}
#endif

