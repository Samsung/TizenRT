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

#ifdef __cplusplus
extern "C" {
#endif

#include <common/bk_err.h>

typedef enum
{
    PWM_MODE_IDLE = 0,
    PWM_MODE_PWM  = 1,
    PWM_MODE_CAPTUR = 2,
}pwm_mode_t;

typedef enum {
	PWM_ID_0 = 0, /**< pwm id 0 */
	PWM_ID_1,     /**< pwm id 1 */
	PWM_ID_2,     /**< pwm id 2 */
	PWM_ID_3,     /**< pwm id 3 */
	PWM_ID_4,     /**< pwm id 4 */
	PWM_ID_5,     /**< pwm id 5 */

	PWM_ID_6,
	PWM_ID_7,
	PWM_ID_8,
	PWM_ID_9,
	PWM_ID_10,
	PWM_ID_11,

	PWM_ID_MAX    /**< pwm id max */
} pwm_id_t;

typedef enum {
	PWM_CH_0 = 0,	/* pwm channel id 0 */
	PWM_CH_1,
	PWM_CH_2,
	PWM_CH_3,
	PWM_CH_4,
	PWM_CH_5,
	PWM_CH_MAX
}pwm_ch_t;

typedef enum {
	PWM_SCLK_CLK32 = 0,  /**< PWM source clock dco */
	PWM_SCLK_XTAL,       /**< PWM source clock xtal 26M */
	PWM_SCLK_80M,        /**< PWM source clock 80M */
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

typedef enum {
	PWM_POLARITY_POSITIVE = 0x0,  /**< PWM positive polarity */
	PWM_POLARITY_NEGATIVE = 0x3,  /**< PWM negative polarity */
	PWM_POLARITY_MAX,             /**< Invalid polarity */
} pwm_polarity_t;

typedef uint32_t pwm_unit_t;
typedef uint32_t pwm_chan_t;	//pwm_id_t

#define BK_ERR_PWM_HAL_CLOCK (BK_ERR_PWM_HAL_BASE - 1)

#ifdef __cplusplus
}
#endif
