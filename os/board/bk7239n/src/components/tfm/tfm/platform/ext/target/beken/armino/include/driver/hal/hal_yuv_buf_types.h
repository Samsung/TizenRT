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

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	YUV_BUF_MODE_YUV = 0,      /**< camera work in yuv mode */
	YUV_BUF_MODE_JPEG,         /**< camera work in jpeg mode */
	YUV_BUF_MODE_H264,
} yuv_buf_work_mode_t;

typedef enum {
	YUV_FORMAT_YUYV = 0,
	YUV_FORMAT_UYVY,
	YUV_FORMAT_YYUV,
	YUV_FORMAT_UVYY,
} yuv_format_t;

typedef enum {
	YUV_MCLK_DIV_4 = 0,
	YUV_MCLK_DIV_6,
	YUV_MCLK_DIV_2,
	YUV_MCLK_DIV_3,
} yuv_buf_mclk_div_t;

typedef struct {
	yuv_format_t yuv_format;
} yuv_mode_cfg_t;

typedef struct {

} h264_enc_mode_cfg_t;

typedef struct {
	yuv_buf_work_mode_t work_mode;
	yuv_buf_mclk_div_t mclk_div;    /**< div yuv_buf module clock for MCLK and PCLK */
	uint32_t x_pixel;    /**< sensor data's resolution for width */
	uint32_t y_pixel;    /**< sensor data's resolution for height */
	yuv_mode_cfg_t yuv_mode_cfg;
	h264_enc_mode_cfg_t h264_enc_mode_cfg;
} yuv_buf_config_t;

typedef struct {
	uint32_t x_partial_offset_l;
	uint32_t x_partial_offset_r;
	uint32_t y_partial_offset_l;
	uint32_t y_partial_offset_r;
} yuv_buf_partial_offset_config_t;

#ifdef __cplusplus
}
#endif

