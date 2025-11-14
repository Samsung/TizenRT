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

#include <soc/soc.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef volatile struct {
	/* REG_0x0 ~ REG_2f */
	struct {
		union {
			struct {
				uint32_t gpio_input:         1; /**< bit[0] gpio_input_bit, R */
				uint32_t gpio_output:        1; /**< bit[1] gpio_output_bit, R/W */
				uint32_t gpio_input_en:      1; /**< bit[2] gpio_input_en_bit, R/W */
				uint32_t gpio_output_en:     1; /**< bit[3] gpio_output_en_bit, R/W */
				uint32_t gpio_pull_mode:     1; /**< bit[4] gpio_pull_mode_bit: 1:pull_up 0:pull_down*/
				uint32_t gpio_pull_mode_en:  1; /**< bit[5] gpio_pull_mode_en_bit, defult:1R/W */
				uint32_t gpio_2_func_en:     1; /**< bit[6] gpio_2_func_en_bit, R/W */
				uint32_t gpio_input_monitor: 1; /**< bit[7] view gpio input values, R/W */
				uint32_t gpio_capacity:      2; /**< bit[8:9] gpio driver capacity,R/W */
				uint32_t gpio_int_type:      2; /**< bit[10:11] */
				uint32_t gpio_int_en:        1; /**< bit[12] */
				uint32_t gpio_int_clear:     1; /**< bit[13] */
				uint32_t reserved:          18; /**< bit[14:31] */
			};
			uint32_t v;
		} cfg;
	} gpio_num[SOC_GPIO_NUM];

	uint32_t reserved[0x40 - 0x17 - 1];

	/* REG_0x40 */
	union {
		struct {
			uint32_t gpio0_int_st:  1; /**< bit[0] interrupt status R */
			uint32_t gpio1_int_st:  1; /**< bit[1] interrupt status R */
			uint32_t gpio2_int_st:  1; /**< bit[2] interrupt status R */
			uint32_t gpio3_int_st:  1; /**< bit[3] interrupt status R */
			uint32_t gpio4_int_st:  1; /**< bit[4] interrupt status R */
			uint32_t gpio5_int_st:  1; /**< bit[5] interrupt status R */
			uint32_t gpio6_int_st:  1; /**< bit[6] interrupt status R */
			uint32_t gpio7_int_st:  1; /**< bit[7] interrupt status R */
			uint32_t gpio8_int_st:  1; /**< bit[8] interrupt status R */
			uint32_t gpio9_int_st:  1; /**< bit[9] interrupt status R */
			uint32_t gpio10_int_st: 1; /**< bit[10] interrupt status R */
			uint32_t gpio11_int_st: 1; /**< bit[11] interrupt status R */
			uint32_t gpio12_int_st: 1; /**< bit[12] interrupt status R */
			uint32_t gpio13_int_st: 1; /**< bit[13] interrupt status R */
			uint32_t gpio14_int_st: 1; /**< bit[14] interrupt status R */
			uint32_t gpio15_int_st: 1; /**< bit[15] interrupt status R */
			uint32_t gpio16_int_st: 1; /**< bit[16] interrupt status R */
			uint32_t gpio17_int_st: 1; /**< bit[17] interrupt status R */
			uint32_t gpio18_int_st: 1; /**< bit[18] interrupt status R */
			uint32_t gpio19_int_st: 1; /**< bit[19] interrupt status R */
			uint32_t gpio20_int_st: 1; /**< bit[20] interrupt status R */
			uint32_t gpio21_int_st: 1; /**< bit[21] interrupt status R */
			uint32_t gpio22_int_st: 1; /**< bit[22] interrupt status R */
			uint32_t gpio23_int_st: 1; /**< bit[23] interrupt status R */
			uint32_t reserved:      8; /**< bit[24:31] */
		};
		uint32_t v;
	} gpio_0_31_int_st;
} gpio_hw_t;

typedef volatile struct {
	union {
		struct {
			uint32_t gpio_system_group0: 4; /**< bit[0:3] 0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5 R/W */
			uint32_t gpio_system_group1: 4; /**< bit[4:7] 0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5 R/W */
			uint32_t gpio_system_group2: 4; /**< bit[8:11] 0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5 R/W */
			uint32_t gpio_system_group3: 4; /**< bit[12:15] 0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5 R/W */
			uint32_t gpio_system_group4: 4; /**< bit[16:19] 0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5 R/W */
			uint32_t gpio_system_group5: 4; /**< bit[20:23] 0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5 R/W */
			uint32_t gpio_system_group6: 4; /**< bit[24:27] 0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5 R/W */
			uint32_t gpio_system_group7: 4; /**< bit[28:32] 0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5 R/W */
		};
		uint32_t v;
	} gpio_sys_num[SOC_GPIO_SYSTEM_GROUP_NUM];
} system_gpio_func_mode_hw_t;

#ifdef __cplusplus
}
#endif

