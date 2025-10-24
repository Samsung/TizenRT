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

	/* REG_0x04 */
	union {
		struct {
			uint32_t en:        1; /**< bit[0] efuse operate enable, sw set 1, hw clear it after operate finished*/
			uint32_t dir:       1; /**< bit[1] efuse operate direction. 0: read, 1: write */
			uint32_t reserved0: 6; /**< bit[2:7] */
			uint32_t addr:      5; /**< bit[8:12] efuse operate address */
			uint32_t reserved1: 3; /**< bit[13:15] */
			uint32_t wr_data:   8; /**< bit[16:23] efuse write data */
			uint32_t reserved3: 7; /**< bit[24:30] */
			uint32_t vdd25_en:  1; /**< bit[31] */
		};
		uint32_t v;
	} ctrl;

	/* REG_0x05 */
	union {
		struct {
			uint32_t rd_data:       8; /**< bit[0:7] efuse read operate data */
			uint32_t rd_data_valid: 1; /**< bit[8] efuse read data valid indication */
			uint32_t reserved:      23; /**< bit[9:31] */
		};
		uint32_t v;
	} optr;
} efuse_hw_t;

#ifdef __cplusplus
}
#endif

