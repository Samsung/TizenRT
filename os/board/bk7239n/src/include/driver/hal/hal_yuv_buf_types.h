// Copyright 2022-2023 Beken
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

#include <common/bk_err.h>
#include <driver/video_common_types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	yuv_format_t yuv_format;
	sync_level_t vsync; /**< sensor vsync active level  */
	sync_level_t hsync; /**< sensor hsync active level  */
} yuv_mode_cfg_t;

typedef struct {

} h264_enc_mode_cfg_t;

typedef struct {
	yuv_mode_t work_mode;
	mclk_div_t mclk_div;    /**< div yuv_buf module clock for MCLK and PCLK */
	uint32_t x_pixel;    /**< sensor data's resolution for width */
	uint32_t y_pixel;    /**< sensor data's resolution for height */
	yuv_mode_cfg_t yuv_mode_cfg;
	h264_enc_mode_cfg_t h264_enc_mode_cfg;
	uint8_t *base_addr;  /**< used for save source yuv data base addr */
} yuv_buf_config_t;

typedef struct {
	uint32_t x_partial_offset_l;
	uint32_t x_partial_offset_r;
	uint32_t y_partial_offset_l;
	uint32_t y_partial_offset_r;
} yuv_buf_partial_offset_config_t;

typedef struct {
	uint32_t x_pixel_resize; /**< set yuv resize output width */
	uint32_t y_pixel_resize; /**< set yuv resize output height */
	uint8_t *emr_base_addr;  /**< used for read resize data base addr */
} yuv_buf_resize_config_t;

#ifdef __cplusplus
}
#endif

