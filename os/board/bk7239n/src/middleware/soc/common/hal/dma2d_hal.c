// Copyright 2021-2025 Beken
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
#include <soc/soc.h>
#include <driver/dma2d_types.h>
//#include "dma2d_ll.h"
#include "dma2d_hal.h"
#include "dma2d_ll_macro_def.h"
#include "hal.h"
#include "bk_misc.h"

bk_err_t dma2d_hal_soft_reset(void)
{
#if defined(CONFIG_SOC_BK7236XX)
	dma2d_ll_set_module_control_soft_reset(1);
	delay_us(10);
	dma2d_ll_set_module_control_soft_reset(0);
#endif
	return BK_OK;
}

void dma2d_hal_transfes_ability(dma2d_trans_ability_t trans_ability)
{
#if defined(CONFIG_SOC_BK7236XX)
	dma2d_ll_set_dma2d_control_reg_dma2d_ahb_burst_len(trans_ability);
#endif
}
bk_err_t dma2d_hal_init(dma2d_config_t *dma2d)
{
	// 000:m2m; 001:m2m pixel convert with fg; 010:m2m blend; 011:r2m.only with output; 100: m2m blend fix fg; 101:m2m blend fix bg;
	dma2d_ll_set_dma2d_control_reg_mode(dma2d->init.mode);
#if defined(CONFIG_SOC_BK7256XX)
	dma2d_ll_set_dma2d_control_reg_master_tran_length(dma2d->init.trans_ability);
#elif defined(CONFIG_SOC_BK7236XX)
	//dma2d_ll_set_dma2d_control_reg_dma2d_ahb_burst_len(dma2d->init.trans_ability);
	dma2d_ll_set_module_control_clk_gate(1);
#endif

	dma2d_ll_set_dma2d_control_reg_out_byte_revese(dma2d->init.out_byte_by_byte_reverse);
	dma2d_ll_set_dma2d_control_reg_line_offset_mode(dma2d->init.line_offset_mode);

	//0:argb888; 1:rgb888; 010:rgb565; 011:argb1555; 100:argb444; 101 YUYV
	if (dma2d->init.mode == DMA2D_M2M)
		dma2d_ll_set_out_pfc_contrl_out_color_mode(0);
	else
		dma2d_ll_set_out_pfc_contrl_out_color_mode(dma2d->init.color_mode);
#if defined(CONFIG_SOC_BK7236XX)
		dma2d_ll_set_out_pfc_contrl_out_r2y_yuv_fmt( dma2d->init.color_mode  - DMA2D_OUTPUT_YUYV);
#endif
	dma2d_ll_set_out_pfc_contrl_out_alpha_invert(dma2d->init.alpha_inverted);
	dma2d_ll_set_output_offset_out_line_offset(dma2d->init.output_offset);
	dma2d_ll_set_dma2d_fg_pfc_ctrl_fg_rb_swap(dma2d->init.red_blue_swap);
	return BK_OK;
}

bk_err_t dma2d_hal_deinit(void)
{
	// 000:m2m; 001:m2m pixel convert with fg; 010:m2m blend; 011:r2m.only with output; 100: m2m blend fix fg; 101:m2m blend fix bg;
	dma2d_ll_set_dma2d_control_reg_value(0);
	dma2d_ll_set_dma2d_int_clear_value(0x3f);
	//0:argb888; 1:rgb888; 010:rgb565; 011:argb1555; 100:argb444
	dma2d_ll_set_out_pfc_contrl_value(0);
	dma2d_ll_set_output_offset_value(0);
	dma2d_ll_set_dma2d_fg_address_value(0);
	dma2d_ll_set_dma2d_bg_address_value(0);
	dma2d_ll_set_dma2d_fg_pfc_ctrl_value(0);
	dma2d_ll_set_dma2d_fg_pfc_ctrl_value(0);
	dma2d_ll_set_module_control_clk_gate(0);

	return BK_OK;
}


/**
  * @brief  Set the DMA2D transfer parameters.
  * @param  dma2d     Pointer to a dma2d_config_t structure that contains
  *                     the configuration information for the specified DMA2D.
  * @param  src_addr      The source memory Buffer address
  * @param  DstAddress The destination memory Buffer address
  * @param  Width      The width of data to be transferred from source to destination.(number of pixel every line)
  * @param  Height     The height of data to be transferred from source to destination.(number of line)
  * @retval HAL status, uint32_t src_addr, uint32_t dst_addr, uint32_t width, uint32_t height
  */
bk_err_t dma2d_hal_config(dma2d_config_t *dma2d, uint32_t src_addr, uint32_t dst_addr, uint32_t width, uint32_t height)
{

	dma2d_ll_set_dma2d_number_of_line_pexel_line(width);
	dma2d_ll_set_dma2d_number_of_line_number_line(height);
	dma2d_ll_set_dma2d_out_mem_address_out_mem_address(dst_addr);
//	HAL_LOGI("dma2d dst_addr = %p %d %d\n", dst_addr, width, height);
	if (dma2d->init.mode == DMA2D_R2M) {
		#if(0)
		uint32_t tmp;
		uint32_t tmp1;
		uint32_t tmp2;
		uint32_t tmp3;
		uint32_t tmp4;
		tmp1 = src_addr & DMA2D_OCOLR_ALPHA_1;
		tmp2 = src_addr & DMA2D_OCOLR_RED_1;
		tmp3 = src_addr & DMA2D_OCOLR_GREEN_1;
		tmp4 = src_addr & DMA2D_OCOLR_BLUE_1;
		/* Prepare the value to be written to the OCOLR register according to the color mode */
		if (dma2d->init.ColorMode == DMA2D_OUTPUT_ARGB8888) {
			tmp = (tmp3 | tmp2 | tmp1| tmp4);
		}
		else if (dma2d->init.ColorMode == DMA2D_OUTPUT_RGB888) {
			tmp = (tmp3 | tmp2 | tmp4);
		}
		else if (dma2d->init.ColorMode == DMA2D_OUTPUT_RGB565) {
			tmp2 = (tmp2 >> 19U);
			tmp3 = (tmp3 >> 10U);
			tmp4 = (tmp4 >> 3U );
			tmp  = ((tmp3 << 5U) | (tmp2 << 11U) | tmp4);
		}
		else if (dma2d->init.ColorMode == DMA2D_OUTPUT_ARGB1555) {
			tmp1 = (tmp1 >> 31U);
			tmp2 = (tmp2 >> 19U);
			tmp3 = (tmp3 >> 11U);
			tmp4 = (tmp4 >> 3U );
			tmp  = ((tmp3 << 5U) | (tmp2 << 10U) | (tmp1 << 15U) | tmp4);
		}
		else /* Ddma2d->Init.ColorMode = DMA2D_OUTPUT_ARGB4444 */ {
			tmp1 = (tmp1 >> 28U);
			tmp2 = (tmp2 >> 20U);
			tmp3 = (tmp3 >> 12U);
			tmp4 = (tmp4 >> 4U );
			tmp  = ((tmp3 << 4U) | (tmp2 << 8U) | (tmp1 << 12U) | tmp4);
		}
		#endif

		dma2d_ll_set_out_color_reg_output_clor_reg(src_addr);
	}
	else if (dma2d->init.mode == DMA2D_M2M_BLEND_FG) /*M2M_blending with fixed color FG DMA2D Mode selected*/ {
		dma2d_ll_set_dma2d_bg_address_value(src_addr);
	}

	else /* M2M, M2M_PFC,M2M_Blending or M2M_blending with fixed color BG DMA2D Mode */ {
		/* Configure DMA2D source address */
		dma2d_ll_set_dma2d_fg_address_value(src_addr);
	}

	return BK_OK;
}


bk_err_t dma2d_hal_start_transfer(bool start_transfer)
{
	dma2d_ll_set_dma2d_control_reg_tran_start(start_transfer);
	return BK_OK;
}

bool dma2d_hal_is_transfer_done(void)
{
	return dma2d_ll_get_dma2d_control_reg_tran_start();
}

/**
  * @brief  Start the multi-source DMA2D Transfer with interrupt enabled.
  * @param  dma2d     Pointer to a dma2d_config_t structure that contains
  *                     the configuration information for the DMA2D.
  * @param  src_addr1 The source memory Buffer address for the foreground layer.
  * @param  src_addr2 The source memory Buffer address for the background layer.
  * @param  dst_addr  The destination memory Buffer address.
  * @param  Width       The width of data to be transferred from source to destination (expressed in number of pixels per line).
  * @param  Height      The height of data to be transferred from source to destination (expressed in number of lines).
  * @retval HAL status
  */
bk_err_t dma2d_hal_blending_start(dma2d_config_t *dma2d, uint32_t src_addr1, uint32_t src_addr2, uint32_t dst_addr, uint32_t Width,  uint32_t Height)
{
	if(dma2d->init.mode == DMA2D_M2M_BLEND_FG)
	{
		/*blending & fixed FG*/
		dma2d_ll_set_dma2d_fg_color_reg_value(src_addr1);
		/* Configure the source, destination address and the data size */
		dma2d_hal_config(dma2d, src_addr2, dst_addr, Width, Height);
	}
	else if (dma2d->init.mode == DMA2D_M2M_BLEND_BG)
	{
		/*blending & fixed BG*/
		dma2d_ll_set_dma2d_bg_color_reg_value(src_addr2);
		/* Configure the source, destination address and the data size */
		dma2d_hal_config(dma2d, src_addr1, dst_addr, Width, Height);
	}
	else
	{
		dma2d_ll_set_dma2d_bg_address_bg_address(src_addr2);
		/* Configure the source, destination address and the data size */
		dma2d_hal_config(dma2d, src_addr1, dst_addr, Width, Height);
	}
//	dma2d_ll_set_dma2d_control_reg_tran_start(1);
	return BK_OK;
}


bk_err_t dma2d_hal_suspend(bool suspend)
{
	dma2d_ll_set_dma2d_control_reg_tran_suspend(suspend);
	return BK_OK;
}

/**
  * @brief  Configure the DMA2D CLUT Transfer.
  * @param  dma2d   Pointer to a dma2d_config_t structure that contains
  *                   the configuration information for the DMA2D.
  * @param  CLUTCfg  Pointer to a DMA2D_CLUTCfgTypeDef structure that contains
  *                   the configuration information for the color look up table.
  * @param  LayerIdx DMA2D Layer index.
  *                   This parameter can be one of the following values:
  *                   DMA2D_BACKGROUND_LAYER(0) / DMA2D_FOREGROUND_LAYER(1)
  * @note API obsolete and maintained for compatibility with legacy. User is invited
  *      to resort to HAL_DMA2D_CLUTStartLoad() instead to benefit from code compactness,
  *      code size and improved heap usage.
  * @retval HAL status
  */
bk_err_t dma2d_hal_clut_config(dma2d_clut_cfg_t clut_cfg, uint32_t layer_idx)
{
	if(layer_idx == DMA2D_BACKGROUND_LAYER)
	{
		/* Write background CLUT memory address */
		dma2d_ll_set_bg_clut_mem_address_value((uint32_t)clut_cfg.pclut);
		/* Write background CLUT size and CLUT color mode */
		dma2d_ll_set_dma2d_bg_pfc_ctrl_bg_clut_color_mode(clut_cfg.club_color_mode);
//		dma2d_ll_set_dma2d_bg_pfc_ctrl_bg_start_clut();
		dma2d_ll_set_dma2d_bg_pfc_ctrl_bg_clut_size(clut_cfg.size);
	}
	/* Configure the CLUT of the foreground DMA2D layer */
	else
	{
		/* Write foreground CLUT memory address */
		dma2d_ll_set_fg_clut_mem_address_value( (uint32_t)clut_cfg.pclut);
		/* Write foreground CLUT size and CLUT color mode */
		dma2d_ll_set_dma2d_fg_pfc_ctrl_fg_color_mode(clut_cfg.club_color_mode);
		dma2d_ll_set_dma2d_fg_pfc_ctrl_fg_clut_size(clut_cfg.size);
	}
	return BK_OK;
}

/**
  * @brief  Configure the DMA2D Layer according to the specified
  *         parameters in the dma2d_config_t.
  * @param  dma2d Pointer to a dma2d_config_t structure that contains
  *                 the configuration information for the DMA2D.
  * @param  LayerIdx DMA2D Layer index.
  *                   This parameter can be one of the following values:
  *                   DMA2D_BACKGROUND_LAYER(0) / DMA2D_FOREGROUND_LAYER(1)
  * @retval HAL status
  */
bk_err_t dma2d_hal_layer_config(dma2d_config_t *dma2d, uint32_t LayerIdx)
{
	dma2d_layer_cfg_t *pLayerCfg;

	pLayerCfg = &dma2d->layer_cfg[LayerIdx];

	/* Configure the background DMA2D layer */
	if (LayerIdx == DMA2D_BACKGROUND_LAYER) {
		 /* Write DMA2D BGPFCCR register */
	
#if defined(CONFIG_SOC_BK7236XX)
		if(pLayerCfg->input_color_mode > DMA2D_INPUT_A4)
		{
			dma2d_ll_set_dma2d_bg_pfc_ctrl_bg_color_mode(DMA2D_INPUT_YUYV);
			dma2d_ll_set_dma2d_bg_pfc_ctrl_bg_y2r_yuv_fmt(4 - (DMA2D_INPUT_VUYY -  pLayerCfg->input_color_mode));
		}
		else
#endif
		{
			dma2d_ll_set_dma2d_bg_pfc_ctrl_bg_color_mode(pLayerCfg->input_color_mode);
		}
		dma2d_ll_set_dma2d_bg_pfc_ctrl_bg_alpha_mode(pLayerCfg->alpha_mode);
		dma2d_ll_set_dma2d_bg_pfc_ctrl_alpha_invert(pLayerCfg->alpha_inverted);
		dma2d_ll_set_dma2d_bg_pfc_ctrl_bg_rb_swap(pLayerCfg->red_blue_swap);

		dma2d_ll_set_dma2d_bg_pfc_ctrl_bg_alpha(pLayerCfg->input_alpha);
		/* DMA2D BGOR register configuration -------------------------------------*/
		dma2d_ll_set_dma2d_bg_offset_bg_line_offset(pLayerCfg->input_offset);
#if defined(CONFIG_SOC_BK7236XX)
	if (pLayerCfg->input_data_reverse == BYTE_BY_BYTE_REVERSE)
	{
		dma2d_ll_set_dma2d_bg_pfc_ctrl_bg_y2r_byte_reve(1);
		dma2d_ll_set_dma2d_bg_pfc_ctrl_bg_y2r_hword_reve(0);
	}
	else if (pLayerCfg->input_data_reverse == HFWORD_BY_HFWORD_REVERSE)
	{
		dma2d_ll_set_dma2d_bg_pfc_ctrl_bg_y2r_byte_reve(0);
		dma2d_ll_set_dma2d_bg_pfc_ctrl_bg_y2r_hword_reve(1);
	}
	else
	{
		dma2d_ll_set_dma2d_bg_pfc_ctrl_bg_y2r_byte_reve(0);
		dma2d_ll_set_dma2d_bg_pfc_ctrl_bg_y2r_hword_reve(0);
	}
#endif
		/* DMA2D BGCOLR register configuration -------------------------------------*/
		if ((pLayerCfg->input_color_mode == DMA2D_INPUT_A4) || (pLayerCfg->input_color_mode == DMA2D_INPUT_A8))
		{
			dma2d_ll_set_dma2d_bg_color_reg_value(pLayerCfg->input_color);
		}
	}
	 /* Configure the foreground DMA2D layer */
	else {
		/* Write DMA2D FGPFCCR register */
#if defined(CONFIG_SOC_BK7236XX)
		if(pLayerCfg->input_color_mode > DMA2D_INPUT_A4)
		{
			dma2d_ll_set_dma2d_fg_pfc_ctrl_fg_color_mode(DMA2D_INPUT_YUYV);
			dma2d_ll_set_dma2d_fg_pfc_ctrl_fg_y2r_yuv_fmt(4 - (DMA2D_INPUT_VUYY -  pLayerCfg->input_color_mode));
		}
		else
#endif
			dma2d_ll_set_dma2d_fg_pfc_ctrl_fg_color_mode(pLayerCfg->input_color_mode);

		dma2d_ll_set_dma2d_fg_pfc_ctrl_fg_alpha_mode(pLayerCfg->alpha_mode);
		dma2d_ll_set_dma2d_fg_pfc_ctrl_alpha_invert(pLayerCfg->alpha_inverted);
		dma2d_ll_set_dma2d_fg_pfc_ctrl_fg_rb_swap(pLayerCfg->red_blue_swap);
		dma2d_ll_set_dma2d_fg_pfc_ctrl_fg_alpha(pLayerCfg->input_alpha);


		/* DMA2D FGOR register configuration -------------------------------------*/
		dma2d_ll_set_dma2d_fg_offset_fg_line_offset(pLayerCfg->input_offset);
#if defined(CONFIG_SOC_BK7236XX)
		if (pLayerCfg->input_data_reverse == BYTE_BY_BYTE_REVERSE)
		{
			dma2d_ll_set_dma2d_fg_pfc_ctrl_fg_y2r_byte_reve(1);
			dma2d_ll_set_dma2d_fg_pfc_ctrl_fg_y2r_hword_reve(0);
		}
		else if (pLayerCfg->input_data_reverse == HFWORD_BY_HFWORD_REVERSE)
		{
			dma2d_ll_set_dma2d_fg_pfc_ctrl_fg_y2r_byte_reve(0);
			dma2d_ll_set_dma2d_fg_pfc_ctrl_fg_y2r_hword_reve(1);
		}
		else
		{
			dma2d_ll_set_dma2d_fg_pfc_ctrl_fg_y2r_byte_reve(0);
			dma2d_ll_set_dma2d_fg_pfc_ctrl_fg_y2r_hword_reve(0);
		}
#endif

		/* DMA2D FGCOLR register configuration -------------------------------------*/
		if ((pLayerCfg->input_color_mode == DMA2D_INPUT_A4) || (pLayerCfg->input_color_mode == DMA2D_INPUT_A8)) {
			dma2d_ll_set_dma2d_bg_color_reg_value(pLayerCfg->input_color);
		}
	}
	return BK_OK;
}


bk_err_t dma2d_hal_abort(bool abort)
{
	dma2d_ll_set_dma2d_control_reg_tran_abort(abort);
	return BK_OK;
}


bk_err_t dma2d_hal_int_config(dma2d_int_type_t int_type, bool enable)
{
	uint32_t int_value;

	int_value = dma2d_ll_get_dma2d_control_reg_value();

	if (1 == enable) {
		dma2d_ll_set_dma2d_control_reg_value(int_value | int_type);
	}
	else {
		dma2d_ll_set_dma2d_control_reg_value(int_value & ~(int_type));
	}

	return BK_OK;
}

bk_err_t dma2d_hal_int_status_get(void)
{
	return dma2d_ll_get_dma2d_int_status_value();
}

bk_err_t dma2d_hal_int_status_clear(dma2d_int_status_t int_status)
{
	dma2d_ll_set_dma2d_int_clear_value(int_status);
	return BK_OK;
}

/**
  * @brief  Configure the line watermark.
  * @param  Line   Line Watermark configuration (maximum 16-bit long value expected).
  * @note   dma2d_hal_line_Watermar_cfg() API used with the transfer watermark interrupt.
  * @note   The transfer watermark interrupt is disabled once it has occurred.
  * @retval HAL status
  */
bk_err_t dma2d_hal_line_Watermar_cfg(uint32_t Line)
{
	if (Line > 0xFFFF) {
		return BK_FAIL;
	}
	else {
		dma2d_ll_set_dma2d_line_watermark_line_watermark(Line);
	}
	return BK_OK;
}

/**
  * @brief  Start the multi-source DMA2D Transfer.
  * @param  dma2d      Pointer to a dma2d_config_t structure that contains
  *                      the configuration information for the DMA2D.
  * @param  SrcAddress1 The source memory Buffer address for the foreground layer.
  * @param  SrcAddress2 The source memory Buffer address for the background layer.
  * @param  DstAddress  The destination memory Buffer address.
  * @param  Width       The width of data to be transferred from source to destination (expressed in number of pixels per line).
  * @param  Height      The height of data to be transferred from source to destination (expressed in number of lines).
  * @retval HAL status
  */
bk_err_t dma2d_hal_blending_config(uint32_t SrcAddress1, uint32_t SrcAddress2, uint32_t DstAddress, uint32_t Width, uint32_t Height)
{
	return dma2d_ll_get_dma2d_int_status_value();
	return BK_OK;
}

