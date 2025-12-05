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

#include "h264_hal.h"
#include "jpeg_hal.h"
#include "yuv_buf_hal.h"
#include "yuv_buf_ll.h"

static jpeg_hal_t s_jpeg_hal = {
	.hw = (jpeg_hw_t *)JPEG_LL_REG_BASE(0),
};

static h264_hal_t s_h264_hal = {
	.hw = (h264_hw_t *)H264_LL_REG_BASE(0),
};

static bk_err_t yuv_buf_hal_set_config_common(yuv_buf_hal_t *hal, const yuv_buf_config_t *config)
{
	yuv_buf_hal_set_err_mask(hal);
	yuv_buf_ll_set_mclk_div(hal->hw, config->mclk_div);
	yuv_buf_ll_set_x_pixel(hal->hw, config->x_pixel);
	yuv_buf_ll_set_y_pixel(hal->hw, config->y_pixel);
	yuv_buf_ll_set_frame_blk(hal->hw, config->x_pixel * config->y_pixel / 2);
	yuv_buf_ll_set_yuv_format(hal->hw, config->yuv_mode_cfg.yuv_format);

	yuv_buf_ll_set_x_pixel_resize(hal->hw, config->x_pixel);
	yuv_buf_ll_set_y_pixel_resize(hal->hw, config->y_pixel);

	yuv_buf_ll_enable_sync_edge_dect(hal->hw);
	yuv_buf_ll_set_encode_begin_hsync_posedge(hal->hw);
	yuv_buf_ll_disable_bps_cis(hal->hw);
	yuv_buf_ll_disable_memrev(hal->hw);
	yuv_buf_ll_enable_int(hal->hw);

	return BK_OK;
}

bk_err_t yuv_buf_hal_init(yuv_buf_hal_t *hal)
{
	hal->hw = (yuv_buf_hw_t *)YUV_BUF_LL_REG_BASE;
	return BK_OK;
}

bk_err_t yuv_buf_hal_set_yuv_mode_config(yuv_buf_hal_t *hal, const yuv_buf_config_t *config)
{
	yuv_buf_hal_set_config_common(hal, config);

	/* yuv_mode set em_base_addr as psram */
	yuv_buf_ll_set_em_base_addr(hal->hw, SOC_PSRAM_DATA_BASE);
	yuv_buf_ll_set_emr_base_addr(hal->hw, SOC_PSRAM_DATA_BASE);

	return BK_OK;
}

bk_err_t yuv_buf_hal_set_jpeg_mode_config(yuv_buf_hal_t *hal, const yuv_buf_config_t *config)
{
	yuv_buf_hal_set_config_common(hal, config);

	if (config->base_addr == NULL)
		return BK_FAIL;

	yuv_buf_ll_set_em_base_addr(hal->hw, (uint32_t)config->base_addr);
	yuv_buf_ll_set_emr_base_addr(hal->hw, (uint32_t)config->base_addr);

	return BK_OK;
}

bk_err_t yuv_buf_hal_set_h264_mode_config(yuv_buf_hal_t *hal, const yuv_buf_config_t *config)
{
	yuv_buf_hal_set_config_common(hal, config);

	if (config->base_addr == NULL)
		return BK_FAIL;

	yuv_buf_ll_set_em_base_addr(hal->hw, (uint32_t)config->base_addr);
	yuv_buf_ll_set_emr_base_addr(hal->hw, (uint32_t)config->base_addr);

	return BK_OK;
}

bk_err_t yuv_buf_hal_enable_nosensor_encode_mode(yuv_buf_hal_t *hal)
{
	yuv_buf_ll_disable_sync_edge_dect(hal->hw);
	yuv_buf_ll_set_encode_begin_hsync_negedge(hal->hw);
	yuv_buf_ll_enable_bps_cis(hal->hw);
	yuv_buf_ll_enable_memrev(hal->hw);
	yuv_buf_ll_disable_int(hal->hw);

	return BK_OK;
}

bk_err_t yuv_buf_hal_start_yuv_mode(yuv_buf_hal_t *hal)
{
	yuv_buf_ll_enable_yuv_buf_mode(hal->hw);
	yuv_buf_ll_disable_h264_encode_mode(hal->hw);
	return BK_OK;
}

bk_err_t yuv_buf_hal_stop_yuv_mode(yuv_buf_hal_t *hal)
{
	yuv_buf_ll_disable_yuv_buf_mode(hal->hw);
	return BK_OK;
}

bk_err_t yuv_buf_hal_start_jpeg_mode(yuv_buf_hal_t *hal)
{
	yuv_buf_ll_disable_yuv_buf_mode(hal->hw);
	yuv_buf_ll_disable_h264_encode_mode(hal->hw);
	jpeg_hal_enable_jpeg_mode(&s_jpeg_hal);
	return BK_OK;
}

bk_err_t yuv_buf_hal_stop_jpeg_mode(yuv_buf_hal_t *hal)
{
	jpeg_hal_disable_jpeg_mode(&s_jpeg_hal);
	return BK_OK;
}

bk_err_t yuv_buf_hal_start_h264_mode(yuv_buf_hal_t *hal)
{
	yuv_buf_ll_disable_yuv_buf_mode(hal->hw);
	yuv_buf_ll_enable_h264_encode_mode(hal->hw);
	return BK_OK;
}

bk_err_t yuv_buf_hal_stop_h264_mode(yuv_buf_hal_t *hal)
{
	yuv_buf_ll_disable_h264_encode_mode(hal->hw);
	return BK_OK;
}

bk_err_t yuv_buf_hal_set_partial_display_offset_config(yuv_buf_hal_t *hal, const yuv_buf_partial_offset_config_t *offset_config)
{
	yuv_buf_ll_set_x_partial_offset_l(hal->hw, offset_config->x_partial_offset_l);
	yuv_buf_ll_set_x_partial_offset_r(hal->hw, offset_config->x_partial_offset_r);
	yuv_buf_ll_set_y_partial_offset_l(hal->hw, offset_config->y_partial_offset_l);
	yuv_buf_ll_set_y_partial_offset_r(hal->hw, offset_config->y_partial_offset_r);

	return BK_OK;
}

uint32_t yuv_buf_hal_get_em_base_addr(yuv_buf_hal_t *hal)
{
	return yuv_buf_ll_get_em_base_addr(hal->hw);
}

bk_err_t yuv_buf_hal_set_resize_mode_config(yuv_buf_hal_t *hal, const yuv_buf_resize_config_t *resize_config)
{
	yuv_buf_ll_set_x_pixel_resize(hal->hw, (resize_config->x_pixel_resize >> 3));
	yuv_buf_ll_set_y_pixel_resize(hal->hw, (resize_config->y_pixel_resize >> 3));

	yuv_buf_ll_set_emr_base_addr(hal->hw, (uint32_t)resize_config->emr_base_addr);
	yuv_buf_ll_enable_bps_pingpong(hal->hw);

	return BK_OK;
}

uint32_t yuv_buf_hal_get_emr_base_addr(yuv_buf_hal_t *hal)
{
	return yuv_buf_ll_get_emr_base_addr(hal->hw);
}

bk_err_t yuv_buf_set_frame_resolution(yuv_buf_hal_t *hal, uint32_t width, uint32_t height)
{
	yuv_buf_ll_set_x_pixel(hal->hw, width >> 3);
	yuv_buf_ll_set_y_pixel(hal->hw, height >> 3);
	yuv_buf_ll_set_frame_blk(hal->hw, width * height >> 7);

	yuv_buf_ll_set_x_pixel_resize(hal->hw, width >> 3);
	yuv_buf_ll_set_y_pixel_resize(hal->hw, height >> 3);

	jpeg_hal_set_x_pixel(&s_jpeg_hal, width >> 3);
	jpeg_hal_set_x_pixel(&s_jpeg_hal, height >> 3);

	h264_hal_set_img_width(&s_h264_hal, width, height);
	return BK_OK;
}

bk_err_t yuv_buf_hal_set_err_mask(yuv_buf_hal_t *hal)
{
	yuv_buf_ll_int_vsync_nege_mask(hal->hw, 0);
	yuv_buf_ll_int_resl_err_mask(hal->hw, 1);
	yuv_buf_ll_int_sens_full_mask(hal->hw, 1);
	yuv_buf_ll_int_enc_slow_mask(hal->hw, 1);
	yuv_buf_ll_int_h264_err_mask(hal->hw, 1);
	return BK_OK;
}
