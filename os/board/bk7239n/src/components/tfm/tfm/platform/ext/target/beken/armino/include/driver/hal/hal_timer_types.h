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

#define TIMER_CLOCK_FREQ_26M    26000
#define TIMER_CLOCK_FREQ_32K    32

typedef enum {
	TIMER_SCLK_CLK32 = 0,  /**< TIMER source clock dco */
	TIMER_SCLK_XTAL,       /**< TIMER source clock xtal 26M */
} timer_src_clk_t;

typedef enum
{
	SYS_SEL_TIMER0 = 0,
	SYS_SEL_TIMER1,
} sys_sel_timer_t;

/**
 * @brief Timer APIs
 * @addtogroup bk_api_timer_defs Timer API group
 * @{
 */

typedef uint8_t timer_unit_t; /**< Timer uint id */

typedef enum {
	TIMER_ID0 = 0, /**< TIMER id 0 */
	TIMER_ID1,     /**< TIMER id 1 */
	TIMER_ID2,     /**< TIMER id 2 */
	TIMER_ID3,     /**< TIMER id 3 */
	TIMER_ID4,     /**< TIMER id 4 */
	TIMER_ID5,     /**< TIMER id 5 */
	TIMER_ID_MAX   /**< TIMER id max */
} timer_id_t;

typedef enum{
	TIMER_UNIT_MS = 0, /**< TIMER unit type is ms */
    TIMER_UNIT_US      /**< TIMER unit type is us */
} timer_value_unit_t;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

