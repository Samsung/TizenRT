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

#include <common/bk_err.h>
#include <driver/video_common_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BK_ERR_JPEG_NOT_INIT    (BK_ERR_JPEG_BASE - 1) /**< JPEG driver not init */
typedef uint8_t jpeg_unit_t; /**< jpeg uint id */

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
	yuv_mode_t mode;        /**< jpeg module work mode:0/1:jpegencoder/yuv */
	mclk_freq_t  clk;         /**< jpeg module work clk config */
	sync_level_t vsync; /**< sensor vsync active level  */
	sync_level_t hsync; /**< sensor hsync active level  */
	yuv_format_t sensor_fmt; /**< sensor input data format */
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

