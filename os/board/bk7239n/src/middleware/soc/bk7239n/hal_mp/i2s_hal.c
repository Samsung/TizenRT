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


#include "i2s_hw.h"
#include <driver/i2s_types.h>
#include "i2s_hal.h"

static uint8_t i2s_cfg_index = 0;

bk_err_t i2s_hal_set_cfg_index(uint8_t idx)
{
	i2s_cfg_index = 0;
	return BK_OK;
}

uint8_t i2s_hal_get_cfg_index(void)
{
	return i2s_cfg_index;
}

bk_err_t i2s_hal_soft_reset(void)
{
	i2s_ll_set_smb_clkrst_value(I2S_SOFT_RESET_MASK << I2S_SOFT_RESET_POS, i2s_cfg_index);
	return BK_OK;
}

bk_err_t i2s_hal_config(const i2s_cfg_t *config)
{
	i2s_ll_set_pcm_cfg_i2spcmen(config->i2s_en, i2s_cfg_index);
	i2s_ll_set_pcm_cfg_msten(config->role, i2s_cfg_index);
	i2s_ll_set_pcm_cfg_modesel(config->work_mode, i2s_cfg_index);
	i2s_ll_set_pcm_cfg_lrckrp(config->lrck_invert, i2s_cfg_index);
	i2s_ll_set_pcm_cfg_sclkinv(config->sck_invert, i2s_cfg_index);
	i2s_ll_set_pcm_cfg_lsbfirst(config->lsb_first_en, i2s_cfg_index);
	i2s_ll_set_pcm_cfg_synclen(config->sync_length, i2s_cfg_index);
	i2s_ll_set_pcm_cfg_datalen(config->data_length - 1, i2s_cfg_index);

	i2s_ll_set_pcm_cfg_pcm_dlen(config->pcm_dlength, i2s_cfg_index);
	i2s_ll_set_pcm_cfg_smpratio(config->sample_ratio, i2s_cfg_index);
	i2s_ll_set_pcm_cfg_bitratio(config->sck_ratio, i2s_cfg_index);

	i2s_ll_set_pcm_cn_parallel_en(config->parallel_en, i2s_cfg_index);
	i2s_ll_set_pcm_cn_lrcom_store(config->store_mode, i2s_cfg_index);
	i2s_ll_set_pcm_cn_bitratio_h4b(config->sck_ratio_h4b, i2s_cfg_index);
	i2s_ll_set_pcm_cn_smpratio_h2b(config->sample_ratio_h2b, i2s_cfg_index);
	i2s_ll_set_pcm_cn_txint_level(config->txint_level, i2s_cfg_index);
	i2s_ll_set_pcm_cn_rxint_level(config->rxint_level, i2s_cfg_index);

	return BK_OK;
}

bk_err_t i2s_hal_deconfig(void)
{
	i2s_ll_set_pcm_cfg_value(0, i2s_cfg_index);
	i2s_ll_set_pcm_cn_value(0, i2s_cfg_index);
	i2s_ll_set_pcm_dat_value(0, i2s_cfg_index);

	return BK_OK;
}

bk_err_t i2s_hal_int_status_get(i2s_int_status_t *int_status)
{
	switch(int_status->channel_id)
	{
		case I2S_CHANNEL_1:
			int_status->tx_udf = (bool)i2s_ll_get_pcm_stat_txudf(i2s_cfg_index);
			int_status->rx_ovf = (bool)i2s_ll_get_pcm_stat_rxovf(i2s_cfg_index);
			int_status->tx_int = (bool)i2s_ll_get_pcm_stat_txint(i2s_cfg_index);
			int_status->rx_int = (bool)i2s_ll_get_pcm_stat_rxint(i2s_cfg_index);
			break;
		default:
			break;
	}

	return BK_OK;
}

bk_err_t i2s_hal_read_ready_get(uint32_t *read_flag)
{
	*read_flag = i2s_ll_get_pcm_stat_rxfifo_rd_ready(i2s_cfg_index);
	return BK_OK;
}

bk_err_t i2s_hal_write_ready_get(uint32_t *write_flag)
{
	*write_flag = i2s_ll_get_pcm_stat_txfifo_wr_ready(i2s_cfg_index);
	return BK_OK;
}

bk_err_t i2s_hal_en_set(uint32_t value)
{
	i2s_ll_set_pcm_cfg_i2spcmen(value, i2s_cfg_index);
	return BK_OK;
}

bk_err_t i2s_hal_int_set(i2s_isr_id_t int_id, uint32_t value)
{
	switch (int_id) {
		case I2S_ISR_CHL1_TXUDF:
			i2s_ll_set_pcm_cn_txudf_en(value, i2s_cfg_index);
			break;
		case I2S_ISR_CHL1_RXOVF:
			i2s_ll_set_pcm_cn_rxovf_en(value, i2s_cfg_index);
			break;

		case I2S_ISR_CHL1_TXINT:
			i2s_ll_set_pcm_cn_txint_en(value, i2s_cfg_index);
			break;
		case I2S_ISR_CHL1_RXINT:
			i2s_ll_set_pcm_cn_rxint_en(value, i2s_cfg_index);
			break;

		default:
			break;
	}

	return BK_OK;
}

bk_err_t i2s_hal_role_set(uint32_t value)
{
	i2s_ll_set_pcm_cfg_msten(value, i2s_cfg_index);
	return BK_OK;
}

bk_err_t i2s_hal_work_mode_set(uint32_t value)
{
	i2s_ll_set_pcm_cfg_modesel(value, i2s_cfg_index);
	return BK_OK;
}

bk_err_t i2s_hal_lrck_invert_set(uint32_t value)
{
	i2s_ll_set_pcm_cfg_lrckrp(value, i2s_cfg_index);
	return BK_OK;
}

bk_err_t i2s_hal_sck_invert_set(uint32_t value)
{
	i2s_ll_set_pcm_cfg_sclkinv(value, i2s_cfg_index);
	return BK_OK;
}

bk_err_t i2s_hal_lsb_first_set(uint32_t value)
{
	i2s_ll_set_pcm_cfg_lsbfirst(value, i2s_cfg_index);
	return BK_OK;
}

bk_err_t i2s_hal_sync_len_set(uint32_t value)
{
	i2s_ll_set_pcm_cfg_synclen(value, i2s_cfg_index);
	return BK_OK;
}

bk_err_t i2s_hal_data_len_set(uint32_t value)
{
	i2s_ll_set_pcm_cfg_datalen(value, i2s_cfg_index);
	return BK_OK;
}

bk_err_t i2s_hal_pcm_dlen_set(uint32_t value)
{
	i2s_ll_set_pcm_cfg_pcm_dlen(value, i2s_cfg_index);
	return BK_OK;
}

bk_err_t i2s_hal_store_mode_set(uint32_t value)
{
	i2s_ll_set_pcm_cn_lrcom_store(value, i2s_cfg_index);
	return BK_OK;
}

bk_err_t i2s_hal_rxfifo_clear(void)
{
	i2s_ll_set_pcm_cn_rxfifo_clr(1, i2s_cfg_index);
	return BK_OK;
}

bk_err_t i2s_hal_txfifo_clear(void)
{
	i2s_ll_set_pcm_cn_txfifo_clr(1, i2s_cfg_index);
	return BK_OK;
}

bk_err_t i2s_hal_txudf_int_clear(i2s_channel_id_t channel_id)
{
	switch (channel_id) {
		case I2S_CHANNEL_1:
			i2s_ll_clear_pcm_stat_txudf(i2s_cfg_index);
			break;
		default:
			break;
	}

	return BK_OK;
}

bk_err_t i2s_hal_rxovf_int_clear(i2s_channel_id_t channel_id)
{
	switch (channel_id) {
		case I2S_CHANNEL_1:
			i2s_ll_clear_pcm_stat_rxovf(i2s_cfg_index);
			break;
		default:
			break;
	}

	return BK_OK;
}

bk_err_t i2s_hal_txint_level_set(uint32_t value)
{
	i2s_ll_set_pcm_cn_txint_level(value, i2s_cfg_index);
	return BK_OK;
}

bk_err_t i2s_hal_rxint_level_set(uint32_t value)
{
	i2s_ll_set_pcm_cn_rxint_level(value, i2s_cfg_index);
	return BK_OK;
}

bk_err_t i2s_hal_data_write(uint32_t channel_id, uint32_t value)
{
	switch (channel_id) {
		case I2S_CHANNEL_1:
			i2s_ll_set_pcm_dat_value(value, i2s_cfg_index);
			break;
		default:
			break;
	}

	return BK_OK;
}

bk_err_t i2s_hal_data_read(uint32_t *value)
{
	*value = i2s_ll_get_pcm_dat_i2s_dat(i2s_cfg_index);
	return BK_OK;
}

bk_err_t i2s_hal_sample_ratio_set(uint32_t value)
{
	i2s_ll_set_pcm_cfg_smpratio(value, i2s_cfg_index);
	return BK_OK;
}

bk_err_t i2s_hal_sck_ratio_set(uint32_t value)
{
	i2s_ll_set_pcm_cfg_bitratio(value, i2s_cfg_index);
	return BK_OK;
}

bk_err_t i2s_hal_sample_ratio_h2b_set(uint32_t value)
{
	i2s_ll_set_pcm_cn_smpratio_h2b(value, i2s_cfg_index);
	return BK_OK;
}

bk_err_t i2s_hal_sck_ratio_h4b_set(uint32_t value)
{
	i2s_ll_set_pcm_cn_bitratio_h4b(value, i2s_cfg_index);
	return BK_OK;
}

