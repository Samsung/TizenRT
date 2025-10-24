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

#define  USE_ROTT_REGISTER_CALLBACKS  1
#define BK_ERR_ROTT_NOT_INIT    (BK_ERR_LCD_BASE - 2) /**< rotate driver not init */

typedef void (*rott_isr_t)(void);

typedef enum {
	ROTATE_COMPLETE_INT =   1 << 0,      /**< rotate finish int enable */
	ROTATE_WARTERMARK_INT = 1 << 1,      /**<rotate watermark int enable. */
	ROTATE_CFG_ERR_INT =    1 << 2,      /**< rotate config error int enable */
} rott_int_type_t;

typedef enum {
	ROTT_MODE_UNKNOW = 0,
	ROTT_ONLY_YUV2RGB565, /**< bypass rotating, only use yuv2rgb565 pixel formart convert function */
	ROTT_ROTATE90,
	ROTT_ROTATE270,
} rott_mode_t;

typedef enum {
	ROTT_INPUT_NORMAL = 0,                     /**< input data not flip */
	ROTT_INPUT_REVESE_BYTE_BY_BYTE,            /**< convert input data revese byte by byte. */
	ROTT_INPUT_REVESE_HALFWORD_BY_HALFWORD,    /**< convert input data revese halfword by halfword */
} rott_input_data_flow_t;

typedef enum {
	ROTT_OUTPUT_NORMAL = 0,                   /**< output data not flip */
	ROTT_OUTPUT_REVESE_HALFWORD_BY_HALFWORD,  /**< convert output data reverse halfword by halfword*/
} rott_output_data_flow_t;

typedef struct {
	rott_mode_t rot_mode;                    /**< bypass rotating, only use yuv2rgb565 pixel formart convert function */
	void * input_addr;                       /**< input data addr*/
	void * output_addr;                      /**< output data addr*/
	pixel_format_t input_fmt;              /**< pixel format, output rgb565 always*/
	rott_input_data_flow_t input_flow;       /**< input data convert or not */
	rott_output_data_flow_t output_flow;     /**< output data convert or not */
	uint16_t picture_xpixel;                 /**< image width*/
	uint16_t picture_ypixel;                 /**< image height*/

	/** the following is optional, but should make sure rott_driver.c's TABLE_ROTT_BLOCK table is include used x/y pixel */
	uint16_t block_xpixel;                  /**< image width separated block width , must be devisible by picture_xpixel*/
	uint16_t block_ypixel;                  /**< image height separated block height, must be devisible by picture_ypixel */
	uint16_t block_cnt;                     /**< block counts, must be (picture_xpixel/block_xpixel)*(picture_ypixel/block_ypixel) cnt */
	uint16_t watermark_blk;                 /**< if enable watermark enable, you should config watermark_blk, normally defult one-half block cnt */
} rott_config_t;

#ifdef __cplusplus
}
#endif

