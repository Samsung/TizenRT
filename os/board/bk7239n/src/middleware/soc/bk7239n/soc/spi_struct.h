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
			uint32_t tx_fifo_int_level:    2; /**< bit[0:1] */
			uint32_t rx_fifo_int_level:    2; /**< bit[2:3] */
			uint32_t tx_udf_int_en:        1; /**< bit[4] tx fifo underflow int enable */
			uint32_t rx_ovf_int_en:        1; /**< bit[5] rx fifo overflow int enable */
			uint32_t tx_fifo_int_en:       1; /**< bit[6] */
			uint32_t rx_fifo_int_en:       1; /**< bit[7] */
			uint32_t clk_rate:             8; /**< bit[8:15] spi clock rate */
			uint32_t slave_release_int_en: 1; /**< bit[16] only for 4 wire mode */
			uint32_t wire3_en:             1; /**< bit[17] 0: 4 wire, 1: 3 wire */
			uint32_t bit_width:            1; /**< bit[18] 0: 8bit, 1: 16bit */
			uint32_t lsb_first_en:         1; /**< bit[19] 0: MSB first, 1: LSB first */
			uint32_t cpol:                 1; /**< bit[20] */
			uint32_t cpha:                 1; /**< bit[21] */
			uint32_t master_en:            1; /**< bit[22] */
			uint32_t enable:               1; /**< bit[23] */
			uint32_t byte_interval:        6; /**< bit[24:29] */
			uint32_t reserved:             2; /**< bit[30:31] */
		};
		uint32_t v;
	} ctrl;

	/* REG_0x5 */
	union {
		struct {
			uint32_t tx_en:            1;  /**< bit[0] */
			uint32_t rx_en:            1;  /**< bit[1] */
			uint32_t tx_finish_int_en: 1;  /**< bit[2] */
			uint32_t rx_finish_int_en: 1;  /**< bit[3] */
			uint32_t reserved:         4;  /**< bit[4:7] */
			uint32_t tx_trans_len:     12; /**< bit[8:19] */
			uint32_t rx_trans_len:     12; /**< bit[20:31] */
		};
		uint32_t v;
	} cfg;

	/* REG_0x6 */
	union {
		struct {
			uint32_t reserved0:         1;  /**< bit[0] */
			uint32_t tx_fifo_wr_ready:  1;  /**< bit[1] */
			uint32_t rx_fifo_rd_ready:  1;  /**< bit[2] */
			uint32_t reserved1:         1;  /**< bit[3] */
			uint32_t reserved2:         4;  /**< bit[4:7] */
			uint32_t tx_fifo_int:       1;  /**< bit[8] */
			uint32_t rx_fifo_int:       1;  /**< bit[9] */
			uint32_t slave_release_int: 1;  /**< bit[10] */
			uint32_t tx_underflow_int:  1;  /**< bit[11] */
			uint32_t rx_overflow_int:   1;  /**< bit[12] */
			uint32_t tx_finish_int:     1;  /**< bit[13] */
			uint32_t rx_finish_int:     1;  /**< bit[14] */
			uint32_t reserved3:         1;  /**< bit[15] */
			uint32_t tx_fifo_clr:       1;  /**< bit[16] */
			uint32_t rx_fifo_clr:       1;  /**< bit[17] */
			uint32_t reserved4:         14; /**< bit[18:31] */
		};
		uint32_t v;
	} int_status;

	/* REG_0x7 */
	union {
		struct {
			uint32_t fifo_data: 16; /**< bit[0:15] */
			uint32_t reserved:  16; /**< bit[16:31] */
		};
		uint32_t v;
	} data;
} spi_hw_t;

#ifdef __cplusplus
}
#endif

