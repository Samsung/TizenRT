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

#include <common/bk_include.h>

typedef enum
{
    BK_TIMER_ID0        = 0,//BKTIMER0
    BK_TIMER_ID1,
    BK_TIMER_ID2,
    BK_TIMER_ID3,
    BK_TIMER_ID4,
    BK_TIMER_ID5,
    BK_PWM_TIMER_ID0     = (6),//PWM0
    BK_PWM_TIMER_ID1,
    BK_PWM_TIMER_ID2,
    BK_PWM_TIMER_ID3,
    BK_PWM_TIMER_ID4,
    BK_PWM_TIMER_ID5,
    BK_TIMER_ID_MAX_COUNT
}BK_HW_TIMER_INDEX;

#define FCLK_PWM_ID           0

#if (CONFIG_SOC_BK7271)
#define FCLK_TIMER_ID          BKTIMER1
#define CAL_TIMER_ID           BKTIMER2
#elif (!CONFIG_SOC_BK7231)
#define FCLK_TIMER_ID          BKTIMER3
#define CAL_TIMER_ID           BKTIMER2
#endif

#if (CONFIG_RTT)
#define rtos_get_ms_per_tick()      (1000 / RT_TICK_PER_SECOND)
#define rtos_get_tick_per_second()           (RT_TICK_PER_SECOND)
#else
#define rtos_get_ms_per_tick()      2
#define rtos_get_tick_per_second()           (1000/rtos_get_ms_per_tick())
#endif

#define TICK_PER_SECOND       rtos_get_tick_per_second()

#define BK_MS_TO_TICKS(x)     ((x) / (rtos_get_ms_per_tick()))
#define BK_TICKS_TO_MS(x)     ((x) * (rtos_get_ms_per_tick()))


UINT32 fclk_get_second(void);
void fclk_reset_count(void);
void fclk_init(void);
UINT32 fclk_from_sec_to_tick(UINT32 sec);
UINT32 fclk_cal_endvalue(UINT32 mode);
BK_HW_TIMER_INDEX fclk_get_tick_id(void);
UINT32 fclk_get_local_tick32(void);
UINT64 fclk_get_local_tick64(void);

#ifdef __cplusplus
}
#endif
