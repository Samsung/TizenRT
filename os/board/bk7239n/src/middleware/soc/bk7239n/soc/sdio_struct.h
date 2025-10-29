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
			uint32_t  sd_cmd_start    : 1;  //0x0[0],sd host(Slave) start send command; Host:If SW set it to 1, sdio auto clear this bit and then start send command.; Slave:SW set it to 1, sdio will receive and response command if host send command, then sdio clear this bit; 0:If this bit is 0, sdio will no reponse command to host and reject command from host side.,0x0,RW/AutoC
			uint32_t  sd_cmd_rsp      : 1;  //0x0[1],Host Only; sd host need slave respond the command or not.; 1: need respond; 0: not need respond,0x0,RW
			uint32_t  sd_cmd_long     : 1;  //0x0[2],Host Only; sd host need slave respond a long command or short 1: need long response ; 0: need short response,0x0,RW
			uint32_t  sd_cmd_crc_check: 1;  //0x0[3],sd host(Slave?) need to check the slave respond command crc or not ; 1: need check crc  ; 0: not check crc,0x0,RW
			uint32_t  cmd_index       : 6;  //0x0[9:4],Host Only; sd host send command index value,0x0,RW
			uint32_t  reserved        :22;  //0x0[31:10],Reserved,0,R
		};
		uint32_t v;
	} sd_cmd_ctrl;

	/* REG_0x05 */
	uint32_t cmd_argument;

	/* REG_0x06 */
	uint32_t sd_cmd_timer;

	/* REG_0x07 */
	union {
		struct {
			uint32_t sd_data_en:      1;  /**< bit[0] */
			uint32_t sd_data_stop_en: 1;  /**< bit[1] */
			uint32_t sd_data_bus:     1;  /**< bit[2] */
			uint32_t sd_data_mul_blk: 1;  /**< bit[3] */
			uint32_t sd_data_blk:     12; /**< bit[4:15] */
			uint32_t sd_start_wr_en:  1;  /**< bit[16] */
			uint32_t sd_byte_sel:     1;  /**< bit[17] */
			uint32_t sd_tx_byte_sel:  1;  /**< bit[18] */
			uint32_t reserved:        13; /**< bit[19:31] */
		};
		uint32_t v;
	} sd_data_ctrl;

		/* REG_0x08 */
	uint32_t sd_data_timer;

	/* REG_0x09 */
	uint32_t sd_rsp_agument_0;

	/* REG_0x0a */
	uint32_t sd_rsp_agument_1;

	/* REG_0x0b */
	uint32_t sd_rsp_agument_2;

	/* REG_0x0c */
	uint32_t sd_rsp_agument_3;

	/* REG_0x0d */
	union {
		struct {
			uint32_t sd_cmd_send_no_rsp_end_int:  1; /**< bit[0] */
			uint32_t sd_cmd_send_rsp_end_int:     1; /**< bit[1] */
			uint32_t sd_cmd_send_rsp_timeout_int: 1; /**< bit[2] */
			uint32_t sd_data_rec_end_int:         1; /**< bit[3] */
			uint32_t sd_data_wr_end_int:          1; /**< bit[4] */
			uint32_t sd_data_time_out_int:        1; /**< bit[5] */
			uint32_t rx_fifo_need_read:           1; /**< bit[6] */
			uint32_t tx_fifo_need_write:          1; /**< bit[7] */
			uint32_t rx_overflow_int:             1; /**< bit[8] */
			uint32_t tx_fifo_empty_int:           1; /**< bit[9] */
			uint32_t sd_rsp_cmd_crc_ok:           1; /**< bit[10] */
			uint32_t sd_rsp_cmd_crc_fail:         1; /**< bit[11] */
			uint32_t sd_data_crc_ok:              1; /**< bit[12] */
			uint32_t sd_data_crc_fail:            1; /**< bit[13] */
			uint32_t sd_rsp_index:                6; /**< bit[14:19] */
			uint32_t wr_status:                   3; /**< bit[20:22] */
			uint32_t data_busy:                   1; /**< bit[23] */
			uint32_t cmd_s_res_end_int:           1; /**< bit[24] */
			uint32_t data_s_wr_wai_int:           1; /**< bit[25] */
			uint32_t data_s_rd_bus_int:           1; /**< bit[26] */

			uint32_t dat_wrsts_err_int:			  1; /**< bit[27] WC */
			uint32_t dat_crc_fail_int:			  1; /**< bit[28] WC */
			uint32_t dat_s_wr_blk_int:			  1; /**< bit[29] WC */
			uint32_t reserved:                    2; /**< bit[30:31] */
		};
		uint32_t v;
	} sd_cmd_rsp_int_sel;

	/* REG_0x0e */
	union {
		struct {
			uint32_t sd_cmd_send_no_rsp_end_mask:  1;  /**< bit[0] */
			uint32_t sd_cmd_send_rsp_end_mask:     1;  /**< bit[1] */
			uint32_t sd_cmd_send_rsp_timeout_mask: 1;  /**< bit[2] */
			uint32_t sd_data_rec_end_mask:         1;  /**< bit[3] */
			uint32_t sd_data_wr_end_mask:          1;  /**< bit[4] */
			uint32_t sd_data_time_out_mask:        1;  /**< bit[5] */
			uint32_t rx_fifo_need_read_mask:       1;  /**< bit[6] */
			uint32_t tx_fifo_need_write_mask:      1;  /**< bit[7] */
			uint32_t rx_overflow_mask:             1;  /**< bit[8] */
			uint32_t tx_fifo_empty_mask:           1;  /**< bit[9] */
			uint32_t cmd_s_res_end_int_mask:       1;  /**< bit[10] */
			uint32_t data_s_wr_wai_int_mask:       1;  /**< bit[11] */
			uint32_t data_s_rd_bus_int_mask:       1;  /**< bit[12] */
			uint32_t tx_fifo_need_write_mask_cg:   1;  /**< bit[13] */
			uint32_t write_wait_jump_sel:          1;  /**< bit[14] */
			uint32_t idle_stop_jump_sel:           1;  /**< bit[15] */

			uint32_t dat_wrsts_err_int_mask:	   1;  /**< bit[16] */
			uint32_t dat_crc_fail_int_mask:		   1;  /**< bit[17] */
			uint32_t dat_s_wr_blk_int_mask:		   1;  /**< bit[18] */
			uint32_t dat_s_rd_mul_sel:			   1;  /**< bit[19] */
			uint32_t dat_s_wr_mul_sel:			   1;  /**< bit[20] */
			uint32_t reserved:                     11; /**< bit[21:31] */
		};
		uint32_t v;
	} sd_cmd_rsp_int_mask;

	/* REG_0x0f */
	uint32_t tx_fifo_din;

	/* REG_0x10 */
	uint32_t rx_fifo_dout;

	/* REG_0x11 */
	union {
		struct {
			uint32_t rx_fifo_threshold              : 8;  //0xd[7:0],sd host fifo threshold for read,0x0,RW
			uint32_t tx_fifo_threshold              : 8;  //0xd[15:8],sd host fifo threshold for write,0x0,RW
			uint32_t rx_fifo_reset                  : 1;  //0xd[16],read fifo reset, high active; Host/Slave:Reset self state,0x1,R/W1C
			uint32_t tx_fifo_reset                  : 1;  //0xd[17],write fifo reset, high active; Host/Slave:Reset self state,0x1,R/W1C
			uint32_t rx_fifo_rd_ready               : 1;  //0xd[18],host/slave rx fifo not empty,0x0,R
			uint32_t tx_fifo_wr_ready               : 1;  //0xd[19],host/slave tx fifo not full(data is valid, not used for CMD),0x1,R
			uint32_t sd_state_reset                 : 1;  //0xd[20],sdcard command and data state reset, high active; Host/Slave:Reset self state; It's better to reset after one round transfer.,0x1,R/W1C
			uint32_t sd_clk_sel                     : 2;  //0xd[22:21],sdcard clock div sel, used or not depend on project; Invalid.,0x3,RW
			uint32_t sd_rd_wait_sel                 : 1;  //0xd[23],read data after command ?; Use default value.,0x1,RW
			uint32_t sd_wr_wait_sel                 : 1;  //0xd[24],write data after command ?; Use default value.,0x0,RW
			uint32_t clk_rec_sel                    : 1;  //0xd[25],write clock recovery selection; when write data, should be set to 1(always 1,SW not use it),0x0,RW
			uint32_t samp_sel                       : 1;  //0xd[26],sample egde of data 0：neg 1：pos
			uint32_t clk_gate_on                    : 1;  //0xd[27],module clockgate on control 1：always on
			uint32_t host_wr_blk_en                 : 1;  //0xd[28],new multi-block write method enable
			uint32_t host_rd_blk_en                 : 1;  //0xd[29],new multi-block read  method enable
			uint32_t reserved                       : 2;  //0xd[31:30],Reserved,0,R
		};
		uint32_t v;
	} sd_fifo_threshold;

	/* REG_0x12 */
	union {
		struct {
			uint32_t sd_slave:            1; /**< bit[0] */
			uint32_t data_s_rd_mul_block: 1; /**< bit[1] */
			uint32_t io_cur_sta_reg:      2; /**< bit[2:3] */
			uint32_t cmd_52_stop_clr:     1; /**< bit[4] */
			uint32_t cmd_keep_det:        1; /**< bit[5] */
			uint32_t reserved0:           2; /**< bit[6:7] */
			uint32_t fifo_send_cnt:       8; /**< bit[8:15] */
			uint32_t reserved1:          16; /**< bit[16:31] */
		};
		uint32_t v;
	} sd_slave_cfg;

	/* REG_0x13 */
	uint32_t sd_slave_rdat_0;

	/* REG_0x14 */
	uint32_t sd_slave_rdat_1;

	/* REG_0x15 */
	uint32_t sd_slave_wdat_0;

	/* REG_0x16 */
	uint32_t sd_slave_wdat_1;

	/* REG_0x17 */
	union {
		struct {
			uint32_t cmd_s_res_dat_rd               : 1;  //0x15[0],cmd_s_res_dat_rd  //0; ignore this bit ,0x0,R
			uint32_t cmd_s_res_dat_wr               : 1;  //0x15[1],cmd_s_res_dat_wr ,//1 ; ignore this bit ,0x1,R
			uint32_t cmd_s_rec_bb_cnt               : 9;  //0x15[10:2],cmd_s_rec_bb_cnt ,//10:2 ; The length which host write/read.,0x0,R
			uint32_t cmd_s_rec_op_code              : 1;  //0x15[11],cmd_s_rec_op_code,//11 ; Invalid,0x0,R
			uint32_t cmd_s_rec_blk_mod              : 1;  //0x15[12],cmd_s_rec_blk_mod,//12,0x0,R
			uint32_t sd_start_wr_en_r3              : 1;  //0x15[13],sd_start_wr_en_r3,//13; slave write data to line need set SD_START_WR_EN, before set SD_START_WR_EN, ensure this bit is 0, if 1, wait till 0,0x0,R
			uint32_t dat_s_rd_bus_4rd               : 1;  //0x15[14],dat_s_rd_bus_4rd ,//14; Slave:1:Slave is in busy status,CPU should deal the received data.,0x0,R
			uint32_t cmd_s_res_end_4rd              : 1;  //0x15[15],cmd_s_res_end_4rd,//15,0x0,R
			uint32_t dat_s_wr_wai_4rd               : 1;  //0x15[16],dat_s_wr_wai_4rd, //16; wait slave write data finish; 1:write finish,0x0,R
			uint32_t reserved                       :15;  //0x15[31:17],Reserved,0,R
		};
		uint32_t v;
	} sd_slave_status;
} sdio_hw_t;

#ifdef __cplusplus
}
#endif

