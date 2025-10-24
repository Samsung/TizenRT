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

#include <common/bk_include.h>
#include <driver/hal/hal_aon_rtc_types.h>
#include <driver/int_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ALARM_NAME_MAX_LEN ((uint32_t)7)
#define ALARM_LOOP_FOREVER (0xffffffff)
#define AON_RTC_MS_TICK_CNT (32)
#define AON_RTC_PRECISION_TICK (32)	//
//#define AON_RTC_SET_TICK_TIME (8)	//CPU compute can be set tick but the time consumes about 4~8 ticks
#define AON_RTC_ROUND_TICK (0XFFFFFFFF)	//we can set to other value, but maybe easy to cause a new round as the max value is less then 38 hours.
#define AON_RTC_MAX_ALARM_CNT (8)

/**
 * @brief AON RTC interrupt service routine
 */

#define BK_ERR_AON_RTC_DRIVER_NOT_INIT    (BK_ERR_AON_RTC_BASE - 1) /**< AON_RTC driver not init */

typedef void (*aon_rtc_isr_t)(aon_rtc_id_t id, uint8_t *name_p, void *param);

typedef struct {
        aon_rtc_id_t id;
        icu_int_src_t int_src;
        int_group_isr_t isr;
} aon_rtc_int_config_t;

typedef struct _alarm_info_t
{
	uint8_t name[ALARM_NAME_MAX_LEN+1];
	uint32_t period_tick;	
	uint32_t period_cnt;	//total period count == 0Xffffffff means forever period;else period how many times
	aon_rtc_isr_t callback;
	void *param_p;
}alarm_info_t;

typedef struct _alarm_node_t
{
	struct _alarm_node_t *next;
	uint8_t name[ALARM_NAME_MAX_LEN+1];
	uint32_t period_tick;	//tick:AON RTC use it
	uint32_t period_cnt;	//total period count == 0Xffffffff means forever period;else period how many times
	aon_rtc_isr_t callback;
	void *cb_param_p;
	uint64_t start_tick;	//timer tick, SW use it
	uint64_t expired_tick;	//first expired time, timer tick
} alarm_node_t;

#ifdef __cplusplus
}
#endif


