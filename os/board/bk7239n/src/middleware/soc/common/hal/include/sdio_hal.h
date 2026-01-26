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

#include "hal_config.h"
#include "sdio_hw.h"
#include "sdio_ll.h"
//#include <driver/hal/hal_sdio_types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	SDIO_HOST_MODE,
	SDIO_SLAVE_MODE,
}sdio_host_slave_mode_t;

void sdio_hal_slave_cmd_start(uint32_t value);
void sdio_hal_rx_set_sd_byte_sel(bool big_endian);

bool sdio_hal_slave_get_cmd_response_end_int(void);

void sdio_hal_slave_clear_cmd_response_end_int(void);

#if defined(CONFIG_SOC_BK7256XX)
/* REG_0x0a:reg0xa->TX_FIFO_NEED_WRITE_MASK_CG:0xa[13],1:sd host fifo memory need write mask for clk gate writing use only,RW*/
void sdio_hal_host_set_tx_fifo_need_write_mask_cg(uint32_t value);
#endif

uint32_t sdio_hal_get_read_ready(void);
uint32_t sdio_hal_get_write_ready(void);

uint32_t sdio_hal_slave_get_cmd_arg0(void);
uint32_t sdio_hal_slave_read_data(void);
void sdio_hal_slave_write_data(uint32_t value);

uint32_t sdio_hal_get_int_status(void);

uint32_t sdio_hal_slave_get_read_int_status(void);

void sdio_hal_slave_clear_read_int_status(void);

uint32_t sdio_hal_slave_get_rx_count(void);

	void sdio_hal_set_host_slave_mode(sdio_host_slave_mode_t mode);

	void sdio_hal_slave_notify_host_next_block(void);

	uint32 sdio_hal_slave_get_func_reg_value(void);

	void sdio_hal_slave_set_tx_length(uint32_t len);
	void sdio_hal_slave_clear_stop(void);
	void sdio_hal_slave_tx_transaction_en(void);
	void sdio_hal_slave_rx_clear_host_wait_write_data(void);

#if defined(CONFIG_SOC_BK7256XX) || defined(CONFIG_SOC_BK7236XX) || defined(CONFIG_SOC_BK7239XX) || defined(CONFIG_SOC_BK7286XX)
	/* REG_0x0d:reg0xd->SAMP_SEL:0xd[26],sample egde of data 0：neg 1：pos,RW*/
	uint32_t sdio_hal_slave_get_samp_sel(void);

	void sdio_hal_slave_set_samp_sel(uint32_t value);
#endif

#if defined(CONFIG_SDIO_SLAVE)
uint32_t sdio_hal_slave_get_wr_end_int(void);
void sdio_hal_slave_clear_wr_end_int(void);
uint32_t sdio_hal_get_tx_fifo_empty_int_status(void);
void sdio_hal_clear_tx_fifo_empty_int_status(void);
void sdio_hal_set_tx_fifo_empty_int(uint32_t value);
void sdio_hal_slave_set_cmd_res_end_int(uint32_t value);
void sdio_hal_slave_set_write_end_int(uint32_t value);
void sdio_hal_slave_set_read_end_int(uint32_t value);
void sdio_hal_slave_set_blk_size(uint32_t value);
uint32_t sdio_ll_get_sd_slave_wr_finish(void);
void sdio_hal_fifo_reset(void);
#endif

#if CONFIG_SDIO_V2P1
void sdio_hal_slave_clear_wr_blk_int_status(void);
uint32_t sdio_hal_slave_get_wr_blk_int_status(void);
void sdio_hal_slave_set_write_block_int_mask(uint32_t en);
void sdio_hal_slave_set_write_mul_sel(uint32_t en);
void sdio_hal_slave_clear_rd_blk_int_status(void);
uint32_t sdio_hal_slave_get_rd_blk_int_status(void);
void sdio_hal_slave_set_read_block_int_mask(uint32_t en);
void sdio_hal_slave_set_read_mul_sel(uint32_t en);
//register addr from host side is:0,1,2,3 bytes
void sdio_hal_slave_set_arg1_val(uint32_t v);
uint32_t sdio_hal_slave_get_arg1_val(void);
//register addr from host side is:4,5,6,7 bytes
void sdio_hal_slave_set_arg2_val(uint32_t v);
uint32_t sdio_hal_slave_get_arg2_val(void);
void sdio_hal_set_sd_cmd_crc_check(uint32_t value);
void sdio_hal_set_cmd_keep_det(uint32_t value);

void sdio_hal_cmd_sst_sel_en(uint32_t value);
void sdio_hal_dat1_int_en(uint32_t value);
void sdio_hal_cccr_intx_en(uint32_t value);
void sdio_hal_cccr_s4mi_en(uint32_t value);
void sdio_hal_set_reg0x3f_cmd_s_buf_cnt_clr(uint32_t value);

/* REG_0x3f:reg0x3f->cmd_s_buf_cnt_num:0x3f[3:1],设置cmd buffer 缓存深度，最大深度值为5; 缓存深度=cmd_s_buf_cnt_num+1,0x0,RW*/
void sdio_hal_set_reg0x3f_cmd_s_buf_cnt_num(uint32_t value);

/* REG_0x3f:reg0x3f->cmd_s_buf_cnt:0x3f[6:4],cmd buffer 缓存个数; 0：1个缓存的CMD; 1：2个缓存的CMD; 2：3个缓存的CMD; 3：4个缓存的CMD; 4：5个缓存的CMD ,0x0,R*/
uint32_t sdio_hal_get_reg0x3f_cmd_s_buf_cnt(void);

/* REG_0x3f:reg0x3f->cmd_s_buf_cover_clr:0x3f[7],cmd buffer 溢出标志位清零,0x0,RW*/
void sdio_hal_set_reg0x3f_cmd_s_buf_cover_clr(uint32_t value);

/* REG_0x3f:reg0x3f->cmd_s_buf_cover:0x3f[8],cmd buffer 溢出标志位,0x0,R*/
uint32_t sdio_hal_get_reg0x3f_cmd_s_buf_cover(void);

/* REG_0x3f:reg0x3f->cmd_s_buf_en:0x3f[9],cmd buffer enable; 注：先把cmd_s_buf_cnt_num深度值设置完成，再使能cmd_s_buf_en,0x0,RW*/
void sdio_hal_set_reg0x3f_cmd_s_buf_en(uint32_t value);

/* REG_0x3f:reg0x3f->cmd_s_cnt_clr:0x3f[10],cmd计数器清零,0x0,RW*/
void sdio_hal_set_reg0x3f_cmd_s_cnt_clr(uint32_t value);

uint32_t sdio_hal_get_reg0x40_value(void);	
uint32_t sdio_hal_get_reg0x41_cmd_s_rec_argument_buf0(void);	
uint32_t sdio_hal_get_reg0x42_cmd_s_rec_argument_buf1(void);	
uint32_t sdio_hal_get_reg0x43_cmd_s_rec_argument_buf2(void);
uint32_t sdio_hal_get_reg0x44_cmd_s_rec_argument_buf3(void);	
uint32_t sdio_hal_get_reg0x45_cmd_s_rec_argument_buf4(void);	
uint32_t sdio_hal_get_reg0x46_cmd_s_cnt(void);
#endif
#ifdef __cplusplus
}
#endif

