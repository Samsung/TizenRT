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

#ifdef __cplusplus
extern "C" {
#endif

#include <driver/media_types.h>

#define BK_ERR_HW_SCALE_NOT_INIT    (BK_ERR_LCD_BASE - 3) /**< hw scale init err */

/**
 * @brief jpeg isr callback relay jpeg driver
 *
 * @param id: only 0 useful, set 0 default
 * @param param: NULL default
 */
typedef void (*hw_scale_isr_t)(void *param);



typedef enum{
	HW_SCALE0 = 0,
	HW_SCALE1,
	HW_SCALE_MAX,
}scale_id_t;

typedef enum{
	FRAME_SCALE = 0,
	BLOCK_SCALE,
	DVP_DIRECT_YUV_SCALE,
	DVP_H264_SCALE,
	JPEGDEC_YUV_SCALE
}scale_mode_t;

typedef struct{
	uint16_t line_index;
	uint16_t line_count;
	uint8_t *data;
	void *args;
}scale_block_t;

typedef struct{

	uint16_t complete_block_count;
	uint16_t current_block_line;
	uint16_t next_block_line;

	uint16_t complete_frame_count;
	uint16_t current_frame_line;
	uint16_t next_frame_line;
}scale_result_t;


typedef struct {
	uint8_t line_cycle;
	uint16_t line_mask;
	uint16_t src_width;            /**< src image width */
	uint16_t src_height;           /**< src image height */
	uint16_t dst_width;            /**< dst image width */
	uint16_t dst_height;           /**< dst image height */
	scale_mode_t scale_mode;
	pixel_format_t pixel_fmt;          /**< scale module input data format , only select YUYV/RGB888/RGB565*/
	uint8_t *src_addr;                /**< image src addr  */
	uint8_t *dst_addr;                /**< image dst addr */
	void (*frame_complete)(scale_result_t *src, scale_result_t *dst, scale_block_t *src_block, scale_block_t *dst_block);
	void (*source_block_complete)(scale_result_t *result, scale_block_t *block);
	void (*dest_block_complete)(scale_result_t *result, scale_block_t *block);
	void (*scale_block_result)(scale_block_t *src_block, scale_block_t *dst_block);
}scale_drv_config_t;

typedef struct {
	scale_block_t src_block;
	scale_block_t dst_block;
	uint32_t dst_line_count;
	scale_result_t src_result;
	scale_result_t dst_result;
	uint8_t remain_data[1280 * 2];
	uint16_t remain_size;
	uint16_t fill_count;
	uint16_t complete_count;
    uint16_t h_index_coef[1280];
    uint16_t s_hloc_scale[1280];
}scale_drv_record_t;


typedef struct {
	scale_drv_config_t *config;
	scale_drv_record_t *record;
	uint8_t is_init;
	uint8_t is_config : 1;
}scale_drv_handle_t;



#ifdef __cplusplus
}
#endif

