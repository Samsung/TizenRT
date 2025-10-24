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
			uint32_t tx_enable:  1; /**< bit[0] uart tx enable */
			uint32_t rx_enable:  1; /**< bit[1] uart rx enable */
			uint32_t reserved1:  1; /**< bit[2] */
			/* uart length
			 * 0: 5bit
			 * 1: 6bit
			 * 2: 7bit
			 * 3: 8bit
			 */
			uint32_t data_bits: 2; /**< bit[3:4] data bits */
			uint32_t parity_en: 1; /**< bit[5] 0: disable, 1: enable */
			uint32_t parity:    1; /**< bit[6] parity 0: Even, 1: Odd */
			uint32_t stop_bits: 1; /**< bit[7] stop bits, 0: 1 bit, 1: 2bit */
			uint32_t clk_div:   16; /**< bit[8:23] clk_div = uart_clk/baud_rate */
			uint32_t reserved:  8;  /**< bit[24:31] */
		};
		uint32_t v;
	} config;

	/* REG_0x05 */
	union {
		struct {
			uint32_t tx_fifo_threshold:   8; /**< bit[0:7] */
			uint32_t rx_fifo_threshold:   8; /**< bit[8:15] */
			/* rx stop detect time
			 * 0: 32
			 * 1: 64
			 * 2: 128
			 * 3: 256
			 */
			uint32_t rx_stop_detect_time: 2; /**< bit[16:17] */
			uint32_t reserved:            14;
		};
		uint32_t v;
	} fifo_config;

	/* REG_0x06 */
	union {
		struct {
			uint32_t tx_fifo_count: 8; /**< bit[0:7] */
			uint32_t rx_fifo_count: 8; /**< bit[8:15] */
			uint32_t tx_fifo_full:  1; /**< bit[16] */
			uint32_t tx_fifo_empty: 1; /**< bit[17] */
			uint32_t rx_fifo_full:  1; /**< bit[18] */
			uint32_t rx_fifo_empty: 1; /**< bit[19] */
			uint32_t fifo_wr_ready: 1; /**< bit[20] */
			uint32_t fifo_rd_ready: 1; /**< bit[21] */
			uint32_t reserved:      10;
		};
		uint32_t v;
	} fifo_status;

	/* REG_0x07 */
	union {
		struct {
			uint32_t tx_fifo_data_in:  8; /**< bit[0:7] */
			uint32_t rx_fifo_data_out: 8; /**< bit[8:15] */
			uint32_t reserved:         16;
		};
		uint32_t v;
	} fifo_port;

	/* REG_0x08 */
	union {
		struct {
			uint32_t tx_fifo_need_write: 1; /**< bit[0] */
			uint32_t rx_fifo_need_read:  1; /**< bit[1] */
			uint32_t rx_fifo_overflow:   1; /**< bit[2] */
			uint32_t rx_parity_err:      1; /**< bit[3] */
			uint32_t rx_stop_bits_err:   1; /**< bit[4] */
			uint32_t tx_finish:          1; /**< bit[5] */
			uint32_t rx_finish:          1; /**< bit[6] */
			uint32_t rxd_wakeup:         1; /**< bit[7] */
			uint32_t reserved:           24;
		};
		uint32_t v;
	} int_enable;

	/* REG_0x09 */
	union {
		struct {
			uint32_t tx_fifo_need_write: 1; /**< bit[0] */
			uint32_t rx_fifo_need_read:  1; /**< bit[1] */
			uint32_t rx_fifo_overflow:   1; /**< bit[2] */
			uint32_t rx_parity_err:      1; /**< bit[3] */
			uint32_t rx_stop_bits_err:   1; /**< bit[4] */
			uint32_t tx_finish:          1; /**< bit[5] */
			uint32_t rx_finish:          1; /**< bit[6] */
			uint32_t rxd_wakeup:         1; /**< bit[7] */
			uint32_t reserved:           24;
		};
		uint32_t v;
	} int_status;

	/* REG_0x0A */
	union {
		struct {
			uint32_t flow_ctrl_low_cnt:  8; /**< bit[0:7] */
			uint32_t flow_ctrl_high_cnt: 8; /**< bit[8:15] */
			uint32_t flow_ctrl_en:       1; /**< bit[16] */
			uint32_t rts_polarity_sel:   1; /**< bit[17] */
			uint32_t cts_polarity_sel:   1; /**< bit[18] */
			uint32_t reserved:           13; /**< bit[19:31] */
		};
		uint32_t v;
	} flow_ctrl_config;

	/* REG_0x0B */
	union {
		struct {
			uint32_t wake_cnt:             10; /**< bit[0:9] */
			uint32_t txd_wait_cnt:         10; /**< bit[10:19] */
			uint32_t rxd_wake_en:          1; /**< bit[20] */
			uint32_t txd_wake_en:          1; /**< bit[21] */
			uint32_t rxd_neg_edge_wake_en: 1; /**< bit[22] */
			uint32_t reserved:             9;
		};
		uint32_t v;
	} wake_config;

#if CONFIG_ENABLE_FILTER_GLITCH
	/* REG_0x0C */
	union {
		struct {
			uint32_t glitch_width:         15; /**< bit[0:14] */
			uint32_t cancel_en:            1;  /**< bit[15] */
			uint32_t reserved:             16;
		};
		uint32_t v;
	} glitch_cancel_config;
#endif
} uart_hw_t;

#ifdef __cplusplus
}
#endif

