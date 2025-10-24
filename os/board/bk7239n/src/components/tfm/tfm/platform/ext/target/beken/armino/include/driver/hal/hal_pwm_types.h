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

#include <common/bk_err.h>

typedef enum {
	PWM_ID_0 = 0, /**< pwm id 0 */
	PWM_ID_1,     /**< pwm id 1 */
	PWM_ID_2,     /**< pwm id 2 */
	PWM_ID_3,     /**< pwm id 3 */
	PWM_ID_4,     /**< pwm id 4 */
	PWM_ID_5,     /**< pwm id 5 */
	PWM_ID_MAX    /**< pwm id max */
} pwm_id_t;

typedef enum {
	PWM_SCLK_CLK32 = 0,  /**< PWM source clock dco */
	PWM_SCLK_XTAL,       /**< PWM source clock xtal 26M */
} pwm_src_clk_t;

typedef enum
{
	SYS_SEL_PWM0 = 0,
	SYS_SEL_PWM1,
} sys_sel_pwm_t;

typedef enum {
	PWM_CAPTURE_POS = 0,  /**< Calculate cycles between two post edges */
	PWM_CAPTURE_NEG,      /**< Calculate cycles between two negtive edges */
	PWM_CAPTURE_EDGE,     /**< Calculate cycles between two edges (post or negative) */
	PWM_CAPTURE_MAX,      /**< Invalid capture mode */
} pwm_capture_edge_t;

typedef uint8_t pwm_unit_t;
typedef uint8_t pwm_chan_t;

#define BK_ERR_PWM_HAL_CLOCK (BK_ERR_PWM_HAL_BASE - 1)

#ifdef __cplusplus
}
#endif
