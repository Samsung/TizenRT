// Copyright 2023-2024 Beken
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

#define JPEG_CRC_SIZE     (5)
#define H264_GOP_FRAME_CNT    (CONFIG_H264_P_FRAME_CNT + 1)
#define H265_GOP_FRAME_CNT    (H264_GOP_FRAME_CNT)

typedef enum {
	YUV_FORMAT_YUYV = 0,
	YUV_FORMAT_UYVY,
	YUV_FORMAT_YYUV,
	YUV_FORMAT_UVYY,
} yuv_format_t;

typedef enum
{
	SYNC_LOW_LEVEL,
	SYNC_HIGH_LEVEL,
} sync_level_t;

typedef enum {
	VIDEO_SYS_CLK_320M = 0,
	VIDEO_SYS_CLK_480M,
} video_sys_clk_t;

typedef enum {
	YUV_MCLK_DIV_4 = 0,
	YUV_MCLK_DIV_6 = 1,
	YUV_MCLK_DIV_2 = 2,
	YUV_MCLK_DIV_3 = 3,
} mclk_div_t;

typedef enum
{
	MCLK_15M,
	MCLK_16M,
	MCLK_20M,
	MCLK_24M,
	MCLK_30M,
	MCLK_32M,
	MCLK_40M,
	MCLK_48M,
	MCLK_UNKNOW,
} mclk_freq_t;

typedef enum
{
	DVP_GPIO_CLK = 0,
	DVP_GPIO_DATA,
	DVP_GPIO_HSYNC_DATA,
	DVP_GPIO_ALL,
} dvp_gpio_mode_t;

typedef enum
{
	UNKNOW_MODE = 0,
	YUV_MODE,
	GRAY_MODE,
	JPEG_MODE,
	H264_MODE,
	H265_MODE,
	JPEG_YUV_MODE,
	H264_YUV_MODE,
} yuv_mode_t;

typedef enum {
    H264_NAL_UNSPECIFIED     = 0,
    H264_NAL_SLICE           = 1,
    H264_NAL_DPA             = 2,
    H264_NAL_DPB             = 3,
    H264_NAL_DPC             = 4,
    H264_NAL_IDR_SLICE       = 5,
    H264_NAL_SEI             = 6,
    H264_NAL_SPS             = 7,
    H264_NAL_PPS             = 8,
    H264_NAL_AUD             = 9,
    H264_NAL_END_SEQUENCE    = 10,
    H264_NAL_END_STREAM      = 11,
    H264_NAL_FILLER_DATA     = 12,
    H264_NAL_SPS_EXT         = 13,
    H264_NAL_PREFIX          = 14,
    H264_NAL_SUB_SPS         = 15,
    H264_NAL_DPS             = 16,
    H264_NAL_RESERVED17      = 17,
    H264_NAL_RESERVED18      = 18,
    H264_NAL_AUXILIARY_SLICE = 19,
    H264_NAL_EXTEN_SLICE     = 20,
    H264_NAL_DEPTH_EXTEN_SLICE = 21,
    H264_NAL_B_FRAME         = 22,
    H264_NAL_P_FRAME         = 23,
    H264_NAL_I_FRAME         = 24,
    H264_NAL_UNSPECIFIED25   = 25,
    H264_NAL_UNSPECIFIED26   = 26,
    H264_NAL_UNSPECIFIED27   = 27,
    H264_NAL_UNSPECIFIED28   = 28,
    H264_NAL_UNSPECIFIED29   = 29,
    H264_NAL_UNSPECIFIED30   = 30,
    H264_NAL_UNSPECIFIED31   = 31,
} h264_type_t;

#ifdef __cplusplus
}
#endif

