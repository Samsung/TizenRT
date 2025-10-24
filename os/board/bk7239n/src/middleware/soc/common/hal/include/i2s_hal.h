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

#include "hal_config.h"
#include <soc/soc.h>
#include "i2s_ll_macro_def.h"
#include <driver/i2s_types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	I2S_TXRX_STATE_NULL = 0,
	I2S_TXRX_STATE_IDLE,
	I2S_TXRX_STATE_START,
	I2S_TXRX_STATE_STOP,
} i2s_txrx_state_t;

typedef struct {
	i2s_channel_id_t channel_id;
	i2s_int_en_t tx_udf_en;
	i2s_int_en_t rx_ovf_en;
	i2s_int_en_t tx_int_en;
	i2s_int_en_t rx_int_en;
} i2s_int_en_config_t;

typedef struct {
	i2s_channel_id_t channel_id;
	bool tx_udf;
	bool rx_ovf;
	bool tx_int;
	bool rx_int;
} i2s_int_status_t;

typedef struct {
	i2s_isr_t callback;
	void *param;
} i2s_callback_t;

typedef struct {
	i2s_data_width_t datawidth;
	i2s_samp_rate_t samp_rate;
} i2s_rate_t;

typedef struct {
	i2s_samp_rate_t samp_rate;
	i2s_data_width_t datawidth;
	uint32_t sys_clk;
	uint32_t smp_ratio;
	uint32_t bit_ratio;
} i2s_rate_table_node_t;

typedef struct {
	i2s_en_t i2s_en;
	i2s_role_t role;
	i2s_work_mode_t work_mode;
	i2s_lrck_invert_en_t lrck_invert;
	i2s_sck_invert_en_t sck_invert;
	i2s_lsb_first_en_t lsb_first_en;
	uint32_t sync_length;
	uint32_t data_length;
	uint32_t pcm_dlength;
	uint32_t sample_ratio;
	uint32_t sck_ratio;
	i2s_parallel_en_t parallel_en;
	i2s_lrcom_store_mode_t store_mode;
	uint32_t sck_ratio_h4b;
	uint32_t sample_ratio_h2b;
	i2s_txint_level_t txint_level;
	i2s_rxint_level_t rxint_level;
	i2s_samp_rate_t samp_rate;
	uint8_t pcm_chl_num;					//the channel number need to transfer in pcm mode
} i2s_cfg_t;


bk_err_t i2s_hal_set_cfg_index(uint8_t idx);
uint8_t i2s_hal_get_cfg_index(void);
bk_err_t i2s_hal_soft_reset(void);
bk_err_t i2s_hal_config(const i2s_cfg_t *config);
bk_err_t i2s_hal_deconfig(void);
bk_err_t i2s_hal_int_status_get(i2s_int_status_t *int_status);
bk_err_t i2s_hal_read_ready_get(uint32_t *read_flag);
bk_err_t i2s_hal_write_ready_get(uint32_t *write_flag);
bk_err_t i2s_hal_en_set(uint32_t value);
bk_err_t i2s_hal_int_set(i2s_isr_id_t int_id, uint32_t value);
bk_err_t i2s_hal_role_set(uint32_t value);
bk_err_t i2s_hal_work_mode_set(uint32_t value);
bk_err_t i2s_hal_lrck_invert_set(uint32_t value);
bk_err_t i2s_hal_sck_invert_set(uint32_t value);
bk_err_t i2s_hal_lsb_first_set(uint32_t value);
bk_err_t i2s_hal_sync_len_set(uint32_t value);
bk_err_t i2s_hal_data_len_set(uint32_t value);
bk_err_t i2s_hal_pcm_dlen_set(uint32_t value);
bk_err_t i2s_hal_store_mode_set(uint32_t value);
bk_err_t i2s_hal_rxfifo_clear(void);
bk_err_t i2s_hal_txfifo_clear(void);
bk_err_t i2s_hal_txudf_int_clear(i2s_channel_id_t channel_id);
bk_err_t i2s_hal_rxovf_int_clear(i2s_channel_id_t channel_id);
bk_err_t i2s_hal_txint_level_set(uint32_t value);
bk_err_t i2s_hal_rxint_level_set(uint32_t value);
bk_err_t i2s_hal_data_write(uint32_t channel_id, uint32_t value);
bk_err_t i2s_hal_data_read(uint32_t *value);
bk_err_t i2s_hal_sample_ratio_set(uint32_t value);
bk_err_t i2s_hal_sck_ratio_set(uint32_t value);
bk_err_t i2s_hal_sample_ratio_h2b_set(uint32_t value);
bk_err_t i2s_hal_sck_ratio_h4b_set(uint32_t value);

#if CFG_HAL_DEBUG_I2S
void i2s_struct_dump(void);
#else
#define i2s_struct_dump()
#endif


#ifdef __cplusplus
}
#endif
