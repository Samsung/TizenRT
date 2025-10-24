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

typedef volatile struct {
	struct {
		/* REG_0x00 */
		uint32_t dev_id;

		/* REG_0x01 */
		uint32_t dev_version;

		/* REG_0x02 */
		union {
			struct {
				uint32_t soft_reset:      1; /**< bit[0] uart soft reset */
				uint32_t clk_gate_bypass: 1; /**< bit[1] bypass uart clock gate */
				uint32_t reserved:       30; /**< bit[2:31] reserved */
			};
			uint32_t v;
		} global_ctrl;

		/* REG_0x03 */
		uint32_t dev_status;

		uint32_t timer_cnt[SOC_TIMER_CHAN_NUM_PER_GROUP];

		/* REG_0x07 */
		union {
			struct {
				uint32_t timer0_en:     1; /**< bit[0] */
				uint32_t timer1_en:     1; /**< bit[1] */
				uint32_t timer2_en:     1; /**< bit[2] */
				uint32_t clk_div:       4; /**< bit[3:6] */
				uint32_t timer0_int_en: 1; /**< bit[7] */
				uint32_t timer1_int_en: 1; /**< bit[8] */
				uint32_t timer2_int_en: 1; /**< bit[9] */
			};
			uint32_t v;
		} ctrl;

		/* REG_0x08 */
		union {
			struct {
				uint32_t timer_cnt_read: 1; /**< bit[0] software write 1, HW clear it after finish read */
				uint32_t reserved0:      1; /**< bit[1] */
				uint32_t timer_index:    2; /**< bit[2:3] timer index for read */
				uint32_t reserved1:     28; /**< bit[4:31] */
			};
			uint32_t v;
		} read_ctrl;

		/* REG_0x09 */
		uint32_t timer_read_value; /**< It is valid just when timer_cnt_read is cleared to 0 */

		uint32_t reserved[0x3FBFF6];
	} group[SOC_TIMER_GROUP_NUM];
} timer_hw_t;

#ifdef __cplusplus
}
#endif

