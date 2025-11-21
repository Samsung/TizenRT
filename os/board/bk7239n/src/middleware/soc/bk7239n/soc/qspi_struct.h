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

typedef volatile struct {
	/* REG_0x00 */
	uint32_t dev_id;

	/* REG_0x01 */
	uint32_t version_id;

	/*REG_0x02*/
	union {
		struct {
			uint32_t soft_reset:     1;/**< bit[0] */
			uint32_t bps_clkgate:    1;/**< bit[1] */
			uint32_t reserved:       30;/**< bit[ 2:31]*/
		};
		uint32_t v;
	} glb_ctrl;

	/*REG_0x3*/
	uint32_t core_status;

	/* REG_0x4 ~ REG_0x7 */
	uint32_t reserved0[4];

	/*REG_0x8*/
	union {
		struct {
			uint32_t cmd5:    8; /**< bit[ 0: 7] */
			uint32_t cmd6:    8; /**< bit[ 8:15] */
			uint32_t cmd7:    8; /**< bit[16:23] */
			uint32_t cmd8:    8; /**< bit[24:31] */
		};
		uint32_t v;
	} cmd_a_l;

	/* REG_0x9 */
	union {
		struct {
			uint32_t cmd1:    8; /**< bit[ 0: 7] */
			uint32_t cmd2:    8; /**< bit[ 8:15] */
			uint32_t cmd3:    8; /**< bit[16:23] */
			uint32_t cmd4:    8; /**< bit[24:31] */
		};
		uint32_t v;
	} cmd_a_h;


	/* REG_0xA */
	union {
		struct {
			uint32_t cmd1_line:    2;   /**< bit[ 0: 1] */
			uint32_t cmd2_line:    2;   /**< bit[ 2: 3] */
			uint32_t cmd3_line:    2;   /**< bit[ 4: 5] */
			uint32_t cmd4_line:    2;   /**< bit[ 6: 7] */
			uint32_t cmd5_line:    2;   /**< bit[ 8: 9] */
			uint32_t cmd6_line:    2;   /**< bit[10:11] */
			uint32_t cmd7_line:    2;   /**< bit[12:13] */
			uint32_t cmd8_line:    2;   /**< bit[14:15] */
			uint32_t reserved:     16;  /**< bit[16:31] */
		};
		uint32_t v;
	} cmd_a_cfg1;

	/* REG_0xB */
	union {
		struct {
			uint32_t reserved:     14;   /**< bit[ 0:13] */
			uint32_t data_line:     2;   /**< bit[14:15] */
			uint32_t dummy_clock:   7;   /**< bit[16:22] */
			uint32_t reserved1:     1;   /**< bit[23]    */
			uint32_t dummy_mode:    3;   /**< bit[24:26] */
			uint32_t reserved2:     3;   /**< bit[27:29] */
			uint32_t cmd_mode:      2;   /**< bit[30:31] */
		};
		uint32_t v;
	} cmd_a_cfg2;

	/* REG_0xC */
	union {
		struct {
			uint32_t cmd5:    8; /**< bit[ 0: 7] */
			uint32_t cmd6:    8; /**< bit[ 8:15] */
			uint32_t cmd7:    8; /**< bit[16:23] */
			uint32_t cmd8:    8; /**< bit[24:31] */
		};
		uint32_t v;
	} cmd_b_l;

	/* REG_0xD */
	union {
		struct {
			uint32_t cmd1:    8; /**< bit[ 0: 7] */
			uint32_t cmd2:    8; /**< bit[ 8:15] */
			uint32_t cmd3:    8; /**< bit[16:23] */
			uint32_t cmd4:    8; /**< bit[24:31] */
		};
		uint32_t v;
	} cmd_b_h;

	/* REG_0xE */
	union {
		struct {
			uint32_t cmd1_line:    2;   /**< bit[ 0: 1] */
			uint32_t cmd2_line:    2;   /**< bit[ 2: 3] */
			uint32_t cmd3_line:    2;   /**< bit[ 4: 5] */
			uint32_t cmd4_line:    2;   /**< bit[ 6: 7] */
			uint32_t cmd5_line:    2;   /**< bit[ 8: 9] */
			uint32_t cmd6_line:    2;   /**< bit[10:11] */
			uint32_t cmd7_line:    2;   /**< bit[12:13] */
			uint32_t cmd8_line:    2;   /**< bit[14:15] */
			uint32_t reserved:     16;  /**< bit[16:31] */
		};
		uint32_t v;
	} cmd_b_cfg1;

	/* REG_0xF */
	union {
		struct {
			uint32_t reserved:     14;   /**< bit[ 0:13] */
			uint32_t data_line:     2;   /**< bit[14:15] */
			uint32_t dummy_clock:   7;   /**< bit[16:22] */
			uint32_t reserved1:     1;   /**< bit[23]    */
			uint32_t dummy_mode:    3;   /**< bit[24:26] */
			uint32_t reserved2:     3;   /**< bit[27:29] */
			uint32_t cmd_mode:      2;   /**< bit[30:31] */
		};
		uint32_t v;
	} cmd_b_cfg2;

	/* REG_0x10 */
	union {
		struct {
			uint32_t cmd5:    8; /**< bit[ 0: 7] */
			uint32_t cmd6:    8; /**< bit[ 8:15] */
			uint32_t cmd7:    8; /**< bit[16:23] */
			uint32_t cmd8:    8; /**< bit[24:31] */
		};
		uint32_t v;
	} cmd_c_l;

	/* REG_0x11 */
	union {
		struct {
			uint32_t cmd1:    8; /**< bit[ 0: 7] */
			uint32_t cmd2:    8; /**< bit[ 8:15] */
			uint32_t cmd3:    8; /**< bit[16:23] */
			uint32_t cmd4:    8; /**< bit[24:31] */
		};
		uint32_t v;
	} cmd_c_h;

	/* REG_0x12 */
	union {
		struct {
			uint32_t cmd1_line:    2;   /**< bit[ 0: 1] */
			uint32_t cmd2_line:    2;   /**< bit[ 2: 3] */
			uint32_t cmd3_line:    2;   /**< bit[ 4: 5] */
			uint32_t cmd4_line:    2;   /**< bit[ 6: 7] */
			uint32_t cmd5_line:    2;   /**< bit[ 8: 9] */
			uint32_t cmd6_line:    2;   /**< bit[10:11] */
			uint32_t cmd7_line:    2;   /**< bit[12:13] */
			uint32_t cmd8_line:    2;   /**< bit[14:15] */
			uint32_t reserved:     16;  /**< bit[16:31] */
		};
		uint32_t v;
	} cmd_c_cfg1;

	/* REG_0x13 */
	union {
		struct {
			uint32_t cmd_start:     1;   /**< bit[0] */
			uint32_t reserved:      1;   /**< bit[1] */
			uint32_t data_len:     10;   /**< bit[ 2:11] */
			uint32_t reserved1:     2;   /**< bit[12:13] */
			uint32_t data_line:     2;   /**< bit[14:15] */
			uint32_t dummy_clock:   7;   /**< bit[16:22] */
			uint32_t reserved2:     1;   /**< bit[23]    */
			uint32_t dummy_mode:    3;   /**< bit[24:26] */
			uint32_t reserved3:     5;   /**< bit[27:31] */
		};
		uint32_t v;
	} cmd_c_cfg2;


	/* REG_0x14 */
	union {
		struct {
			uint32_t cmd5:    8; /**< bit[ 0: 7] */
			uint32_t cmd6:    8; /**< bit[ 8:15] */
			uint32_t cmd7:    8; /**< bit[16:23] */
			uint32_t cmd8:    8; /**< bit[24:31] */
		};
		uint32_t v;
	} cmd_d_l;

	/* REG_0x15 */
	union {
		struct {
			uint32_t cmd1:    8; /**< bit[ 0: 7] */
			uint32_t cmd2:    8; /**< bit[ 8:15] */
			uint32_t cmd3:    8; /**< bit[16:23] */
			uint32_t cmd4:    8; /**< bit[24:31] */
		};
		uint32_t v;
	} cmd_d_h;

	/* REG_0x16 */
	union {
		struct {
			uint32_t cmd1_line:    2;   /**< bit[ 0: 1] */
			uint32_t cmd2_line:    2;   /**< bit[ 2: 3] */
			uint32_t cmd3_line:    2;   /**< bit[ 4: 5] */
			uint32_t cmd4_line:    2;   /**< bit[ 6: 7] */
			uint32_t cmd5_line:    2;   /**< bit[ 8: 9] */
			uint32_t cmd6_line:    2;   /**< bit[10:11] */
			uint32_t cmd7_line:    2;   /**< bit[12:13] */
			uint32_t cmd8_line:    2;   /**< bit[14:15] */
			uint32_t reserved:     16;  /**< bit[16:31] */
		};
		uint32_t v;
	} cmd_d_cfg1;

	/* REG_0x17 */
	union {
		struct {
			uint32_t cmd_start:     1;   /**< bit[0] */
			uint32_t reserved:      1;   /**< bit[1] */
			uint32_t data_len:     10;   /**< bit[ 2:11] */
			uint32_t reserved1:     2;   /**< bit[12:13] */
			uint32_t data_line:     2;   /**< bit[14:15] */
			uint32_t dummy_clock:   7;   /**< bit[16:22] */
			uint32_t reserved2:     1;   /**< bit[23]    */
			uint32_t dummy_mode:    3;   /**< bit[24:26] */
			uint32_t reserved3:     5;   /**< bit[27:31] */
		};
		uint32_t v;
	} cmd_d_cfg2;

	/* REG_0x18 */
	union {
		struct {
			uint32_t qspi_en:           1;   /**< bit[0] */
			uint32_t cpol:              1;   /**< bit[1] */
			uint32_t cpha:              1;   /**< bit[2] */
			uint32_t io2_io3_mode:      1;   /**< bit[3] */
			uint32_t io2:               1;   /**< bit[4] */
			uint32_t io3:               1;   /**< bit[5] */
			uint32_t force_spi_cs_low:  1;   /**< bit[6] */
			uint32_t nss_h_gen_sck:     1;   /**< bit[7] */
			uint32_t clk_rate:          8;   /**< bit[8:15] */
			uint32_t disable_cmd_sck:   1;   /**< bit[16] */
			uint32_t led_dahb_rd_bps:   1;   /**< bit[17] */
			uint32_t first_bit_mode:    1;   /**< bit[18] */
			uint32_t dahb_trans_type:   2;   /**< bit[19:20] */
			uint32_t tx_fifo_clr_sync_sck_bps:  1;  /**< bit[21] */
			uint32_t io_cpu_mem_sel:    1;   /**< bit[22] */
			uint32_t spi_rcv_4byte_mode:1;   /**< bit[23] */
			uint32_t spi_cs_h_wait:     8;   /**< bit[24:31] */
		};
		uint32_t v;
	} config;

	/* REG_0x19 */
	union {
		struct {
			uint32_t reserved:          4;   /**< bit[0:3] */
			uint32_t clk_man_sel:       1;   /**< bit[4] */
			uint32_t clk_man_en:        1;   /**< bit[5] */
			uint32_t fifo_io_wr:        1;   /**< bit[6]    */
			uint32_t reserved1:         25;  /**< bit[7:31] */
		};
		uint32_t v;
	} rst_cfg;

	/* REG_0x1A */
	union {
		struct {
			uint32_t int_rx_done:          1;   /**< bit[0]    */
			uint32_t int_tx_done:          1;   /**< bit[1]    */
			uint32_t int_cmd_start_done:   1;   /**< bit[2]    */
			uint32_t int_qspi_done:        1;   /**< bit[3]    */
			uint32_t int_cmd_start_fail:   1;   /**< bit[4]    */
			uint32_t int_addr_cnt:         1;   /**< bit[5]    */
			uint32_t reserved:            26;   /**< bit[6:31] */
		};
		uint32_t v;
	} int_en;

	/* REG_0x1B */
	union {
		struct {
			uint32_t clr_rx_done:          1;   /**< bit[0]    */
			uint32_t clr_tx_done:          1;   /**< bit[1]    */
			uint32_t clr_cmd_start_done:   1;   /**< bit[2]    */
			uint32_t clr_qspi_done:        1;   /**< bit[3]    */
			uint32_t clr_cmd_start_fail:   1;   /**< bit[4]    */
			uint32_t int_addr_cnt:         1;   /**< bit[5]    */
			uint32_t reserved:            26;   /**< bit[6:31] */
		};
		uint32_t v;
	} status_clr;

	/* REG_0x1C */
	union {
		struct {
			uint32_t rx_done:          1;   /**< bit[0]    */
			uint32_t tx_done:          1;   /**< bit[1]    */
			uint32_t cmd_start_done:   1;   /**< bit[2]    */
			uint32_t qspi_done:        1;   /**< bit[3]    */
			uint32_t cmd_start_fail:   1;   /**< bit[4]    */
			uint32_t reserved:         7;   /**< bit[ 5:11]*/
			uint32_t qspi_cs:          1;   /**< bit[12]   */
			uint32_t rx_busy:          1;   /**< bit[13]   */
			uint32_t tx_busy:          1;   /**< bit[14]   */
			uint32_t reserved1:        1;   /**< bit[15]   */
			uint32_t fifo_empty:       1;   /**< bit[16]   */
			uint32_t reserved2:        15;  /**< bit[17:31]*/
		};
		uint32_t v;
	} status;

	/* REG_0x1D */
	union {
		struct {
			uint32_t led_wr_continue_cmd:  24;   /**< bit[ 0:23]*/
			uint32_t reserved:             8;    /**< bit[24:31] */
		};
		uint32_t v;
	} led_cfg;

	/* REG_0x1E */
	union {
		struct {
			uint32_t cmd_last_addr:  24;   /**< bit[ 0:23]*/
			uint32_t reserved:       8;    /**< bit[24:31] */
		};
		uint32_t v;
	} reg30;

	/* REG_0x1F */
	uint32_t lcd_head_cmd0;

	/* REG_0x20 */
	uint32_t lcd_head_cmd1;

	/* REG_0x21 */
	uint32_t lcd_head_cmd2;

	/* REG_0x22 */
	uint32_t lcd_head_cmd3;

	/* REG_0x23 */
	union {
		struct {
			uint32_t lcd_head_hcnt:  16;   /**< bit[ 0:15]*/
			uint32_t lcd_head_vcnt:  16;   /**< bit[16:31] */
		};
		uint32_t v;
	} reg35;

	/* REG_0x24 */
	union {
		struct {
			uint32_t lcd_head_dly:  8;   /**< bit[ 0:7]*/
			uint32_t lcd_head_len:  6;   /**< bit[ 8:13]*/
			uint32_t lcd_head_sel:  1;   /**< bit[14]*/
			uint32_t lcd_head_clr:  1;   /**< bit[15]*/
			uint32_t reserved:      16;  /**< bit[16:31] */
		};
		uint32_t v;
	} reg36;

	/* REG_0x25 ~ REG_0x3F */
	uint32_t reserved2[27];

	/* REG_0x40 ~ REG_0x7C */
	uint32_t fifo_data[61];
} qspi_hw_t;

#ifdef __cplusplus
}
#endif

