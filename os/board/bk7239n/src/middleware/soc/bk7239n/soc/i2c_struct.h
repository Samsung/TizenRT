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
			uint32_t idle_cr:     3;  /**< bit[0:2] */
			uint32_t scl_cr:      3;  /**< bit[3:5] */
			uint32_t freq_div:    10; /**< bit[6:15] */
			uint32_t slave_addr:  10; /**< bit[16:25] */
			uint32_t clk_src:     2;  /**< bit[26:27] */
			uint32_t timeout_en:  1;  /**< bit[28] */
			uint32_t idle_det_en: 1;  /**< bit[29] bus idle detection enable */
			uint32_t inh:         1;  /**< bit[30] */
			uint32_t en:          1;  /**< bit[31] */
		};
		uint32_t v;
	} sm_bus_cfg;

	/* REG_0x05 */
	union {
		struct {
			uint32_t sm_int:        1;  /**< bit[0] */
			uint32_t scl_timeout:   1;  /**< bit[1] */
			uint32_t reserved0:     1;  /**< bit[2] */
			uint32_t arb_lost:      1;  /**< bit[3] */
			uint32_t rx_fifo_empty: 1;  /**< bit[4] */
			uint32_t tx_fifo_full:  1;  /**< bit[5] */
			uint32_t int_mode:      2;  /**< bit[6:7] */
			uint32_t ack:           1;  /**< bit[8] */
			uint32_t stop:          1;  /**< bit[9] */
			uint32_t start:         1;  /**< bit[10] */
			uint32_t addr_match:    1;  /**< bit[11] */
			uint32_t ack_req:       1;  /**< bit[12] */
			uint32_t tx_mode:       1;  /**< bit[13] */
			uint32_t master:        1;  /**< bit[14] */
			uint32_t busy:          1;  /**< bit[15] */
			uint32_t reserved1:     16; /**< bit[16:31] */
		};
		uint32_t v;
	} sm_bus_status;

	/* REG_0x06 */
	union {
		struct {
			uint32_t data:     8;  /**< bit[0:7] */
			uint32_t reserved: 24; /**< bit[8:31] */
		};
		uint32_t v;
	} sm_bus_data;

	/* REG_0x07 */
	union {
		struct {
			uint32_t data_h_outen:     1;  /**< bit[0] */
			uint32_t addr_h_outen:     1;  /**< bit[1] */
			uint32_t reserved0:        6;  /**< bit[2:7] */
			uint32_t byte_interval:    8;  /**< bit[8:15] */
			uint32_t reserved1:        16; /**< bit[16:31] */
		};
		uint32_t v;
	} sm_bus_ex_cfg;
} i2c_typedef_t;

typedef struct {
	i2c_typedef_t *i2c0_hw;
	i2c_typedef_t *i2c1_hw;
} i2c_hw_t;

#ifdef __cplusplus
}
#endif

