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


#include <common/bk_include.h>
#include "sbc_ll_macro_def.h"
#include <driver/sbc_types.h>


void sbc_decoder_hal_sbc_config(const sbc_config_t *config)
{
	sbc_ll_set_sbc_ctrl_sbc_channel(config->channel_num);
	sbc_ll_set_sbc_ctrl_sbc_subband(config->subbands);
	sbc_ll_set_sbc_ctrl_sbc_chn_comb(config->chn_comb);
	sbc_ll_set_sbc_ctrl_sbc_blocks(config->blocks);
}

void sbc_decoder_hal_sbc_enable(void)
{
	sbc_ll_set_sbc_ctrl_sbc_en(0x1);
}

uint32_t sbc_decoder_hal_get_sbc_status_value(void)
{
	return sbc_ll_get_sbc_ctrl_sbc_en();
}

void sbc_decoder_hal_sbc_support_msbc(bool enable)
{
	if(enable) {
		sbc_ll_set_sbc_ctrl_msbc_support(0x1);
	}else {
		sbc_ll_set_sbc_ctrl_msbc_support(0x0);
	}
}

uint32_t sbc_decoder_hal_get_sbc_ctrl_value(void)
{
	return sbc_ll_get_sbc_ctrl_value();
}

void sbc_decoder_hal_sbc_clear_interrupt_status(void)
{
	sbc_ll_set_sbc_state_sbc_int(0x1);
}

uint32_t sbc_decoder_hal_get_sbc_interrupt_status(void)
{
	return sbc_ll_get_sbc_state_sbc_int();
}

uint32_t sbc_decoder_hal_get_sbc_idle_status(void)
{
	return sbc_ll_get_sbc_state_sbc_idle();
}

void sbc_decoder_hal_set_res_bytel_value(uint32_t bytel_value)
{
	sbc_ll_set_res_bytel_value(bytel_value);
}

void sbc_decoder_hal_set_res_bytem_value(uint32_t bytem_value)
{
	sbc_ll_set_res_bytem_value(bytem_value);
}

void sbc_decoder_hal_set_res_byteh_value(uint32_t byteh_value)
{
	sbc_ll_set_res_byteh_value(byteh_value);
}

void sbc_decoder_hal_set_sbc_bit_num(uint32_t value)
{
	sbc_ll_set_bit_num_sbc_bitnum(value);
}

void sbc_decoder_hal_set_scale_factor(uint32_t value)
{
	sbc_ll_set_sf_sbc_scale_factor(value);
}

void sbc_decoder_hal_set_sbc_level(uint32_t value)
{
	sbc_ll_set_level_sbc_level(value);
}

void sbc_decoder_hal_set_sbc_res_bit(uint32_t value)
{
	sbc_ll_set_res_bit_sbc_res_bit(value);
}

void sbc_decoder_hal_decode_enable(void)
{
	sbc_ll_set_dec_en_value(0x1);
}

uint32_t sbc_decoder_hal_get_decode_enable_value(void)
{
	return sbc_ll_get_dec_en_value();
}

uint32_t sbc_decoder_hal_get_pcm_data(void)
{
	uint32_t pcm_data = 0;
	pcm_data = sbc_ll_get_pcm_data_value();
	return pcm_data;
}

void sbc_decoder_hal_mem_init(void)
{
	uint32_t i = 0;
	uint32_t j = 0;
	uint32_t *addr_mem0 = (uint32_t *)SBC_SBC_MEM0_ADDR;
	uint32_t *addr_mem1 = (uint32_t *)SBC_SBC_MEM1_ADDR;

	for(i = 0; i < 256; i++)
	{
		addr_mem0[i] = 0;
	}

	for(j = 0; j < 320; j++)
	{
		addr_mem1[j] = 0;
	}
}

void sbc_decoder_hal_soft_reset(void)
{
    sbc_ll_set_clk_control_soft_reset(1);
}

