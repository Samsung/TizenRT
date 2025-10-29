// Copyright 2022-2025 Beken
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

// This is a generated file, if you need to modify it, use the script to
// generate and modify all the struct.h, ll.h, reg.h, debug_dump.c files!

#pragma once

#ifdef __cplusplus
extern "C" {
#endif


typedef volatile union {
	struct {
		uint32_t deviceid                 : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} irda_device_id_t;


typedef volatile union {
	struct {
		uint32_t versionid                : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} irda_version_id_t;


typedef volatile union {
	struct {
		uint32_t soft_reset               :  1; /**<bit[0 : 0] */
		uint32_t clkg_bypass              :  1; /**<bit[1 : 1] */
		uint32_t reserved_bit_2_31        : 30; /**<bit[2 : 31] */
	};
	uint32_t v;
} irda_soft_reset_t;


typedef volatile union {
	struct {
		uint32_t devstatus                : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} irda_dev_status_t;


typedef volatile union {
	struct {
		uint32_t rxenable                 :  1; /**<bit[0 : 0] */
		uint32_t rx_initial_level         :  1; /**<bit[1 : 1] */
		uint32_t txenable                 :  1; /**<bit[2 : 2] */
		uint32_t tx_initial_level         :  1; /**<bit[3 : 3] */
		uint32_t tx_start                 :  1; /**<bit[4 : 4] */
		uint32_t irda_pwd                 :  1; /**<bit[5 : 5] */
		uint32_t rfu7                     :  2; /**<bit[6 : 7] */
		uint32_t clk_freq_in              :  7; /**<bit[8 : 14] */
		uint32_t rstn                     :  1; /**<bit[15 : 15] */
		uint32_t txdata_num               : 10; /**<bit[16 : 25] */
		uint32_t reserved_bit_26_31       :  6; /**<bit[26 : 31] */
	};
	uint32_t v;
} irda_reg0x4_t;


typedef volatile union {
	struct {
		uint32_t tx_fifo_threshold        :  8; /**<bit[0 : 7] */
		uint32_t rx_fifo_threshold        :  8; /**<bit[8 : 15] */
		uint32_t rx_timeout_cnt           : 16; /**<bit[16 : 31] */
	};
	uint32_t v;
} irda_reg0x5_t;


typedef volatile union {
	struct {
		uint32_t tx_fifo_count            :  8; /**<bit[0 : 7] */
		uint32_t rx_fifo_count            :  8; /**<bit[8 : 15] */
		uint32_t tx_fifo_full             :  1; /**<bit[16 : 16] */
		uint32_t tx_fifo_empty            :  1; /**<bit[17 : 17] */
		uint32_t rx_fifo_full             :  1; /**<bit[18 : 18] */
		uint32_t rx_fifo_empty            :  1; /**<bit[19 : 19] */
		uint32_t fifo_wr_ready            :  1; /**<bit[20 : 20] */
		uint32_t fifo_rd_ready            :  1; /**<bit[21 : 21] */
		uint32_t rxdata_num               : 10; /**<bit[22 : 31] */
	};
	uint32_t v;
} irda_reg0x6_t;


typedef volatile union {
	struct {
		uint32_t fifo_data_rx             : 16; /**<bit[0 : 15] */
		uint32_t fifo_data_tx             : 16; /**<bit[16 : 31] */
	};
	uint32_t v;
} irda_reg0x7_t;


typedef volatile union {
	struct {
		uint32_t tx_need_wr_mask          :  1; /**<bit[0 : 0] */
		uint32_t rx_need_rd_mask          :  1; /**<bit[1 : 1] */
		uint32_t tx_done_mask             :  1; /**<bit[2 : 2] */
		uint32_t rx_timeout_mask          :  1; /**<bit[3 : 3] */
		uint32_t rx_overflow_status       :  1; /**<bit[4 : 4] */
		uint32_t rfu6                     : 27; /**<bit[5 : 31] */
	};
	uint32_t v;
} irda_reg0x8_t;


typedef volatile union {
	struct {
		uint32_t tx_need_wr_state         :  1; /**<bit[0 : 0] */
		uint32_t rx_need_rd_state         :  1; /**<bit[1 : 1] */
		uint32_t tx_done_status           :  1; /**<bit[2 : 2] */
		uint32_t rx_done_status           :  1; /**<bit[3 : 3] */
		uint32_t rx_overflow_status       :  1; /**<bit[4 : 4] */
		uint32_t rfu5                     : 27; /**<bit[5 : 31] */
	};
	uint32_t v;
} irda_reg0x9_t;


typedef volatile union {
	struct {
		uint32_t carrier_period           :  8; /**<bit[0 : 7] */
		uint32_t carrier_duty             :  8; /**<bit[8 : 15] */
		uint32_t carrier_polarity         :  1; /**<bit[16 : 16] */
		uint32_t carrier_enable           :  1; /**<bit[17 : 17] */
		uint32_t rfu4                     :  2; /**<bit[18 : 19] */
		uint32_t rx_start_thr             : 12; /**<bit[20 : 31] */
	};
	uint32_t v;
} irda_reg0xa_t;


typedef volatile union {
	struct {
		uint32_t glitch_enable            :  1; /**<bit[0 : 0] */
		uint32_t rfu0                     :  1; /**<bit[1 : 1] */
		uint32_t rfu1                     :  1; /**<bit[2 : 2] */
		uint32_t rfu2                     : 13; /**<bit[3 : 15] */
		uint32_t glitch_thr               : 12; /**<bit[16 : 27] */
		uint32_t rfu3                     :  4; /**<bit[28 : 31] */
	};
	uint32_t v;
} irda_reg0xb_t;

typedef volatile struct {
	volatile irda_device_id_t device_id;
	volatile irda_version_id_t version_id;
	volatile irda_soft_reset_t soft_reset;
	volatile irda_dev_status_t dev_status;
	volatile irda_reg0x4_t reg0x4;
	volatile irda_reg0x5_t reg0x5;
	volatile irda_reg0x6_t reg0x6;
	volatile irda_reg0x7_t reg0x7;
	volatile irda_reg0x8_t reg0x8;
	volatile irda_reg0x9_t reg0x9;
	volatile irda_reg0xa_t reg0xa;
	volatile irda_reg0xb_t reg0xb;
} irda_hw_t;

#ifdef __cplusplus
}
#endif
