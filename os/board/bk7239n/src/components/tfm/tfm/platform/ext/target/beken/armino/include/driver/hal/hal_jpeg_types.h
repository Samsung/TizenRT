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

#include <driver/dma_types.h>
#include <common/bk_err.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BK_ERR_JPEG_NOT_INIT    (BK_ERR_JPEG_BASE - 1) /**< JPEG driver not init */
typedef uint8_t jpeg_unit_t; /**< jpeg uint id */


typedef enum {
	JPEG_ENC_MODE = 0,
	JPEG_YUV_MODE,
}jpeg_mode_t;

typedef enum {
	JPEG_SYS_CLK_320M = 0,
	JPEG_SYS_CLK_480M,
} jpeg_sys_clk_t;

typedef enum {
	JPEG_MCLK_DIV_4 = 0,
	JPEG_MCLK_DIV_6 = 1,
	JPEG_MCLK_DIV_2 = 2,
	JPEG_MCLK_DIV_3 = 3,
} jpeg_mclk_div_t;

/**
 * @brief jpeg module work clk and sensor work clk
 * @{
 */
typedef enum
{
	JPEG_96M_MCLK_16M = 0,   /**< jpeg module work in 96MHz and sensor work in 16MHz */
	JPEG_96M_MCLK_24M,
	JPEG_120M_MCLK_20M,
	JPEG_120M_MCLK_30M,
	JPEG_80M_MCLK_20M,
	JPEG_160M_MCLK_40M,
} jpeg_clk_t;

typedef enum
{
	JPEG_SYNC_LOW_LEVEL,
	JPEG_SYNC_HIGH_LEVEL,
} jpeg_sync_level_t;

/**
 * @brief jpeg_yuv_format_t when jpeg encode at yuv mode, the output yuv format type
 *
 * @attation 1. fllow type out base on input YUYV
 */
typedef enum {
	JPEG_YUYV = 0, /**< YUYV, bit[31-0]=[YUYV] */
	JPEG_UYVY = 1, /**< UYVY, bit[31-0]=[UYVY] */
	JPEG_YYUV = 2, /**< YYUV, bit[31-0]=[YYUV] */
	JPEG_UVYY = 3, /**< UVYY, bit[31-0]=[UVYY] */
}jpeg_yuv_format_t;

typedef enum {
	JPEG_EOY = 0,        /**< when work at yuv mode, transfer a complete frame will trigger this isr */
	JPEG_HEAD_OUTPUT,    /**< when work at jpeg encode mode, the head output complete will trigger this isr */
	JPEG_SOF,            /**< when work at jpeg encode mode, detect vsync rising edge after few cycle will trigger this isr */
	JPEG_EOF,            /**< when work at jpeg encode mode, transfer a complete frame will trigger this isr */
	JPEG_VSYNC_NEGEDGE,  /**< when detect vsync negedge will trigger this isr */
	JPEG_LINE_CLEAR,     /**< when used encode yuv data by 8line every time will trigger this isr */
	JPEG_FRAME_ERR,      /**< when detect jpeg encode error, will trigger this isr */
	JPEG_ISR_MAX,
}jpeg_isr_type_t;


typedef struct {
	uint8_t    *rx_buf;      /**< the address of receiving jpeg data */
	jpeg_mode_t mode;        /**< jpeg module work mode:0/1:jpegencoder/yuv */
	jpeg_clk_t  clk;         /**< jpeg module work clk config */
	jpeg_sync_level_t vsync; /**< sensor vsync active level  */
	jpeg_sync_level_t hsync; /**< sensor hsync active level  */
	jpeg_yuv_format_t sensor_fmt; /**< sensor input data format */
	uint32_t rx_buf_len;  /**< the length for receiving jpeg data */
	uint32_t node_len;    /**< dma transfer length */
	uint32_t x_pixel;     /**< jpeg encode image resolution for width */
	uint32_t y_pixel;     /**< jpeg encode image resolution for height */
} jpeg_config_t;

typedef struct {
	uint32_t x_partial_offset_l;
	uint32_t x_partial_offset_r;
	uint32_t y_partial_offset_l;
	uint32_t y_partial_offset_r;
} jpeg_partial_offset_config_t;

#ifdef __cplusplus
}
#endif

