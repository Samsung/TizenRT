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

//TODO add full comments according to reg spec
typedef volatile struct {
	struct {
		union {
			struct {
				/* PWM mode
				 * 000: idle
				 * 001: PWM mode
				 * 010: Timer mode
				 * 011: Counter mode
				 * 100: Capture mode (pos -> pos)
				 * 101: Capture mode (neg -> neg)
				 * 110: Capture mode (edge -> edge)
				 */
				uint32_t chan0_mode:       3; /**< bit[0:2] pwm0_mode, R/W */
				uint32_t chan0_en:         1; /**< bit[3] pwm0_int_enable, R/W */
				uint32_t chan0_int_en:     1; /**< bit[4] */
				uint32_t chan0_timer_stop: 1; /**< bit[5] */
				uint32_t chan0_init_level: 1; /**< bit[6] */
				uint32_t chan0_cfg_update: 1; /**< bit[7] */

				uint32_t chan1_mode:       3; /**< bit[8:10] pwm0_mode, R/W */
				uint32_t chan1_en:         1; /**< bit[11] pwm0_int_enable, R/W */
				uint32_t chan1_int_en:     1; /**< bit[12] */
				uint32_t chan1_timer_stop: 1; /**< bit[13] */
				uint32_t chan1_init_level: 1; /**< bit[14] */
				uint32_t chan1_cfg_update: 1; /**< bit[15] */

				uint32_t pre_div:          8; /**< bit[16:23] */
				uint32_t group_pwm_en:     1; /**< bit[24] */
				uint32_t group_en:         1; /**< bit[25] */

				uint32_t reserved:         4; /**< bit[26:29] */

				uint32_t chan0_int_st:     1; /**< bit[30] */
				uint32_t chan1_int_st:     1; /**< bit[31] */
			};
			uint32_t v;
		} ctrl;

		struct {
			uint32_t t1;
			uint32_t t2;
			uint32_t t3;
			uint32_t t4; /**< PWM period cycle, t4 >= (t1 + t2 + t3) */
		} duty_cycle[SOC_PWM_CHAN_NUM_PER_GROUP];

		union {
			struct {
				uint32_t rd0: 1; /**< bit[0], W, software write 1 to begin the read, HW clear it when data ready */
				uint32_t rd1: 1; /**< bit[0], R/W,  */
				uint32_t reserved: 30; /**< bit[2:31] */
			};
			uint32_t v;
		} cnt_read_ctrl;

		uint32_t cnt_or_capture_rdata[SOC_PWM_CHAN_NUM_PER_GROUP];

		uint32_t reserved[0x10 - 0xb - 1];

	} group[SOC_PWM_GROUP_NUM];
} pwm_hw_t;

#ifdef __cplusplus
}
#endif
