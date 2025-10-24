// Copyright 2022-2023 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//	 http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "h264_ll.h"
#include "sys_ll.h"
#include "h264_hal.h"
#include <driver/hal/hal_h264_types.h>
#include "hal_port.h"

bk_err_t h264_hal_init(h264_hal_t *hal)
{
	hal->hw=(h264_hw_t *)H264_LL_REG_BASE(0);
	return BK_OK;
}

bk_err_t h264_hal_set_img_width(h264_hal_t *hal,uint16_t media_width, uint16 media_height)
{
	uint32_t h264_lines = (media_height / 16) - 1;
	h264_ll_set_img_width(hal->hw,media_width);
	h264_ll_set_img_height(hal->hw,media_height);
	h264_ll_set_mb_lines(hal->hw,h264_lines);
	return BK_OK;
}

void h264_hal_gop_config(h264_hal_t *hal,const h264_config_t *config)
{
	h264_ll_set_num_pframe(hal->hw,config->num_pframe);
	h264_ll_set_skip_frame_mode(hal->hw,config->skip_mode);
}

// to do : yuv buffer addr setting

void h264_hal_qp_config(h264_hal_t *hal,const h264_config_t *config)
{
	h264_ll_set_pframe_max_qp(hal->hw,config->pframe_max_qp);
	h264_ll_set_pframe_min_qp(hal->hw,config->pframe_min_qp);

	h264_ll_set_iframe_max_qp(hal->hw,config->iframe_max_qp);
	h264_ll_set_iframe_min_qp(hal->hw,config->iframe_min_qp);
	h264_ll_set_qp(hal->hw,config->qp);
	h264_ll_set_cqp_offset(hal->hw,config->cqp_off);
	h264_ll_set_qp_target(hal->hw,config->tqp);
}

void h264_hal_frame_cbr_config(h264_hal_t *hal,const h264_config_t *config)
{
	/*iframe cbr*/
	h264_ll_set_iframe_I_gain(hal->hw,config->iframe_igain);
	h264_ll_set_iframe_P_gain(hal->hw,config->iframe_pgain);
	h264_ll_set_iframe_D_gain(hal->hw,config->iframe_dgain);
	h264_ll_set_iframe_min_qp(hal->hw,config->iframe_min_qp);
	h264_ll_set_iframe_max_qp(hal->hw,config->iframe_max_qp);

	/*iframe cbr*/
	h264_ll_set_pframe_I_gain(hal->hw,config->pframe_igain);
	h264_ll_set_pframe_P_gain(hal->hw,config->pframe_pgain);
	h264_ll_set_pframe_D_gain(hal->hw,config->pframe_dgain);
	h264_ll_set_pframe_min_qp(hal->hw,config->pframe_min_qp);
	h264_ll_set_pframe_max_qp(hal->hw,config->pframe_max_qp);
}

void h264_hal_idc_config(h264_hal_t *hal,const h264_config_t *config)
{
	h264_ll_set_profile_idc(hal->hw,config->profile_idc);
	h264_ll_set_level_idc(hal->hw,config->level_idc);
}

bk_err_t h264_hal_filter_config(h264_hal_t *hal,const h264_config_t *config,uint32_t alpha_off,uint32_t beta_off)
{
	if(!config)
	{
		HAL_LOGI("h264 configuration is not initialized! \r\n");
		return -1;
	}
	if(config->filter_mode)
	{
		HAL_LOGI("deblocking filter is disabled! \r\n");
		return -1;
	}
	if(config->num_pframe!=0)
	{
		HAL_LOGI("deblocking filter is enbaled only when numpframe is 0! \r\n");
		return -1;
	}
	h264_ll_set_alpha_off(hal->hw,alpha_off);
	h264_ll_set_beta_off(hal->hw,beta_off);
	return BK_OK;
}

void h264_hal_mb_config(h264_hal_t *hal,const h264_config_t *config)
{
	h264_ll_set_num_imb_bits(hal->hw,config->imb_bits);
	h264_ll_set_num_pmb_bits(hal->hw,config->pmb_bits);
	h264_ll_set_nal_align_mode(hal->hw,config->nal_align_mode);
	h264_ll_set_cons_ipred_mode(hal->hw,config->cons_ipred_mode);
	h264_ll_set_num_ref_bits(hal->hw,config->imb_bits * 12 / 10);
	h264_ll_set_reset_i_state_mode(hal->hw,config->reset_i_state_mode);
}

void h264_hal_mb_bits_config(h264_hal_t *hal,const h264_config_t *config)
{
	h264_ll_set_num_imb_bits(hal->hw,config->imb_bits);
	h264_ll_set_num_pmb_bits(hal->hw,config->pmb_bits);
	h264_ll_set_num_ref_bits(hal->hw,config->imb_bits * 12 / 10);
}

void h264_hal_scale_factor_config(h264_hal_t *hal)
{
	h264_ll_set_Pbits_factor1(hal->hw,PBITS_FACT1);
	h264_ll_set_Pbits_factor2(hal->hw,PBITS_FACT2);
	h264_ll_set_Pbits_factor3(hal->hw,PBITS_FACT3);
	h264_ll_set_Pbits_factor4(hal->hw,PBITS_FACT4);
	h264_ll_set_Pbits_factor5(hal->hw,PBITS_FACT5);
}

void h264_hal_vui_config(h264_hal_t *hal,const h264_config_t *config)
{
	h264_ll_set_vui_time_scale_L(hal->hw,config->vui_time_scale_L);
	h264_ll_set_vui_time_scale_H(hal->hw,config->vui_time_scale_H);
	h264_ll_set_vui_num_u_tick_L(hal->hw,config->vui_num_u_tick_L);
	h264_ll_set_vui_num_u_tick_H(hal->hw,config->vui_num_u_tick_H);
	h264_ll_set_bitrate_scale(hal->hw,config->bitrate_scale);
	h264_ll_set_cpb_size_scale(hal->hw,config->cpb_size_scale);
	h264_ll_set_bitrate_value_less1(hal->hw,config->bitrate_value_less1);
	h264_ll_set_cpb_size_value_less1(hal->hw,config->cpb_size_value_less1);
}

void h264_hal_cpb_vui_config(h264_hal_t *hal,const h264_config_t *config)
{
	h264_ll_set_init_cpb_removal_delay(hal->hw,config->init_cpb_removal_delay);
	h264_ll_set_init_enc_buffer_level_L(hal->hw,config->init_enc_buffer_level_L);
	h264_ll_set_init_enc_buffer_level_H(hal->hw,config->init_enc_buffer_level_H);
	h264_ll_set_bits_per_frame_int_L(hal->hw,config->bits_per_frame_int_L);
	h264_ll_set_bits_per_frame_int_H(hal->hw,config->bits_per_frame_int_H);
	h264_ll_set_bits_per_frame_rem_L(hal->hw,config->bits_per_frame_rem_L);
	h264_ll_set_bits_per_frame_rem_H(hal->hw,config->bits_per_frame_rem_H);
	h264_ll_set_cpb_size_L(hal->hw,config->cpb_size_L);
	h264_ll_set_cpb_size_H(hal->hw,config->cpb_size_H);
}

void h264_hal_soft_reset_active(h264_hal_t *hal)
{
	h264_ll_set_soft_reset_mode(hal->hw,1);
}

void h264_hal_soft_reset_deactive(h264_hal_t *hal)
{
	h264_ll_set_soft_reset_mode(hal->hw,0);
}

void h264_hal_local_info(h264_hal_t *hal)
{
	uint32_t device_id,version_id;
	device_id = h264_ll_devid(hal->hw);
	version_id = h264_ll_verid(hal->hw);
	HAL_LOGI("device id is: %d , version id is: %d \r\n",device_id,version_id);
}

void h264_hal_int_config(h264_hal_t *hal,uint32_t int_mode)
{
	h264_ll_set_int_mode(hal->hw,int_mode);
}

void h264_hal_set_vui_fps(h264_hal_t *hal,uint32_t fps)
{
    uint32_t default_vui_num_u_tick_L = 0x3E8;
    uint32_t vui_time_scale_L = default_vui_num_u_tick_L * 2 * fps;
    h264_ll_set_vui_time_scale_L(hal->hw, vui_time_scale_L);
}