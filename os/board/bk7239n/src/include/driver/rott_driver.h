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

#include <driver/rott_types.h>

/**
* @brief This API used config sysclk
 *
 * @return
 *     - BK_OK: succeed
 *     - others: other errors.
 */
bk_err_t bk_rott_driver_init(void);

/**
* @brief This API used reset rotate
 *
 * @return
 *     - BK_OK: succeed
 *     - others: other errors.
 */

bk_err_t bk_rott_soft_reset(void);

/**
* @brief This API used disable rotate cpu int
 *
 * @return
 *     - BK_OK: succeed
 *     - others: other errors.
 */
bk_err_t bk_rott_driver_deinit(void);

/**
 * @brief This API used to config rotate int type and int enable
 *
 * @param 1:int_type include:
 *
 *            - ROTATE_COMPLETE_INT: rotate complete int
 *            - ROTATE_WARTERMARK_INT: if enable, wtmk_block need config, and not set 0, you can set wtmk_block by api bk_rott_wartermark_block_config config or rott_config
 *            - ROTATE_CFG_ERR_INT, the following will trigger cfg err int,like:
 *                             - picture_xpixel & picture_ypixel be zero while working.
 *                             - ROTATE_WARTERMARK_INT=1, but wtmk_block=0
 *                             - block_cnt=0
 *                             - block_xpixel > picture_xpixel,  block_ypixel > picture_ypixel
 *     2: en enable or disable
 *
 * @return
 *     - BK_OK: succeed
 *     - others: other errors.
 */
bk_err_t bk_rott_int_enable(rott_int_type_t int_type, bool en);


/**
* @brief This API used config rotate delay cycle in block and block interval
 *
 * @param 
 *      - en,enable the delay function
 *      - delay_hclk_count1 (range 0 ~ 0x7fff), the delay pclk cycle is calculate by (delay_hclk_count1 * delay_hclk_count2)
 *      - delay_hclk_count2 (range 0 ~ 0xffff), the delay pclk cycle is calculate by (delay_hclk_count1 * delay_hclk_count2)
 *
 * @return
 *     - BK_OK: succeed
 *     - others: other errors.
 */
bk_err_t bk_rott_block_delay_config(bool en, uint16_t delay_hclk_count1, uint16_t delay_hclk_count2);

/**
* @brief This API used config rotate mode
 *
 * @param struct rott_mode_t
 *      - ROTT_ONLY_YUV2RGB565, only use yuv data pixel convert to rgb565 pixel, no rotate
 *      - ROTT_ROTATE90, if rot_input_fmt = RGB565, just rotate; if rot_input_fmt = yuv, not only rotate but convert to rgb565.
 *      - ROTT_ROTATE170,if rot_input_fmt = RGB565, just rotate; if rot_input_fmt = yuv, not only rotate but convert to rgb565.
 *
 * @return
 *     - BK_OK: succeed
 *     - others: other errors.
 */
bk_err_t bk_rott_mode_config(media_rotate_t    rot_mode);

/**
* @brief This API used config input data format, output data format is default rgb565
 *
 * @param struct rott_input_fmt_t
 *      - RGB565, input format is rgb565 be
 *      - RGB565le, input format is rgb565 le
 *      - ROTT_FMT_YUYV
 *      - ROTT_FMT_VUYY
 *      - others
 *
 *
 * @attention if your data rotate or pixel convert not display normally,you can use api  bk_rott_data_reverse modify input/output data flow to see see
 *
 * @return
 *     - BK_OK: succeed
 *     - others: other errors.
 */
bk_err_t bk_rott_input_data_format(pixel_format_t rot_input_fmt);


/**
* @brief This API used config input data or output data flip(reverse)
 *
 * @param struct rott_input_data_flow_t
 *       - ROTT_INPUT_NORMAL just input data, no flip
 *       - ROTT_INPUT_REVESE_BYTE_BY_BYTE, input data  input convert or flip byte by byte
 *       - ROTT_INPUT_REVESE_HALFWORD_BY_HALFWORD, the  input convert or flip halfword by halfword
 *
 * @param  struct rott_output_data_flow_t
 *       - ROTT_OUTPUT_NORMAL  just output data, no flip outpuy
 *       - ROTT_OUTPUT_REVESE_HALFWORD_BY_HALFWORD,  convert output data reversed(flip) halfword by halfword
 *
 * @attention if your data rotate or pixel convert not display normally,you can use api  bk_rott_data_reverse modify input/output data flow to see see
 *
 * @return
 *     - BK_OK: succeed
 *     - others: other errors.
 */
bk_err_t bk_rott_data_reverse(rott_input_data_flow_t input_flow,  rott_output_data_flow_t output_flow);

/**
 * @brief This API used config rotate param, rotate is by block, a block is max 4800 pixel,so (block_xpixel * block_ypixel) not over then 4800
 *
 * @param
 *
 *      - picture_xpixel:the rotate src picture width (x pixel)
 *      - picture_ypixel: the rotate src picture height(y pixel)
 *      - block_xpixel: should be divisible by picture_xpixel
 *      - block_ypixel: should be divisible by picture_xpixel
 *      - block_cnt: should be (picture_xpixel/block_xpixel) * (picture_ypixel/block_ypixel)
 *
 * Usage example:
 *
 *              A 640X480 picture, the param can set [640,480,80,60,64]
 *              A 800X480 picture, the param can set [800,480,80,60,80]
 *              A 480X800 picture, the param can set [480,800,60,80,80]
 *
 * @return
 *     - BK_OK: succeed
 *     - others: other errors.
 */
bk_err_t bk_rott_block_rotate_config(uint16_t picture_xpixel, uint16_t picture_ypixel, uint16_t block_xpixel, uint16_t block_ypixel, uint16_t block_cnt);

/**
 * @brief This API used config rotate src/dst addr
 *
 * @param
 *
 *      - rd_base_addr:the rotate src picture addr
 *      - wr_base_addr: the rotate dst picture addr
 *
 * @return
 *     - BK_OK: succeed
 *     - others: other errors.
 */
bk_err_t bk_rott_wr_addr_config(uint32_t rd_base_addr, uint32_t wr_base_addr);

/**
 * @brief This API used config block threshold value,must make sure you enable ROTATE_WARTERMARK_INT int
 *
 * @param  wtmk_block: when rotate block is wtmk_block, will trigger ROTATE_WARTERMARK_INT
 *
 * Usage example:
 *
 *          A 640X480 picture, the param can set [640,480,80,60,64], you can set wtmk_block 32 or 16 or 48 or other less then block_cnt value
 *
 * @return
 *     - BK_OK: succeed
 *     - others: other errors.
 */
bk_err_t bk_rott_wartermark_block_config(uint16_t wtmk_block);

/**
 * @brief This API used enable rotate, when rotate complete once, you should enable again to rotate
 *
 * @return
 *     - BK_OK: succeed
 *     - others: other errors.
 */
bk_err_t bk_rott_enable(void);

/**
* @brief This API used reset ROTATE hardware logic
 *
 * @return
 *     - BK_OK: succeed
 *     - others: other errors.
 */
bk_err_t bk_rott_soft_rst(void);

#if (USE_ROTT_REGISTER_CALLBACKS == 1)
/**
 * @brief This API register  rotate isr
 *
 * @param
 *
 *     - int_type
 *     - isr: isr function
 *
  * @return
 *     - BK_OK: succeed
 *     - others: other errors.
 */
bk_err_t  bk_rott_isr_register(rott_int_type_t int_type, rott_isr_t isr);
#else
/**
 * @brief This API register  rotate isr
 *
 * @param
 *
 *     - isr: isr function, you should check which int is triggerd internal
 *
  * @return
 *     - BK_OK: succeed
 *     - others: other errors.
 */
bk_err_t bk_rott_isr_register(rott_isr_t rott_isr);

/**
 * @brief This API used to clear int status
 *
 * @param
 *     - int type: you should used with bk_rott_int_status_get() api
 *
 * Usage example:
 *
 *       if ((bk_rott_int_status_get() & ROTATE_COMPLETE_INT))
 *        bk_rott_int_status_clear(ROTATE_COMPLETE_INT);
 *
 * @return
 *     - BK_OK: succeed
 *     - others: other errors.
 */
bk_err_t bk_rott_int_status_clear(rott_int_type_t int_type);
/**
 * @brief This API get int status
 *
 *
 * @return
 *     - the int status value
 */
uint32_t bk_rott_int_status_get(void);

#endif


/**
 * @brief This API is concentrate multiple api to realize rotate config
 *
 * @param struct rott_input_data_flow_t
 *      - those params is the same as every solo api
 *
 * Usage example:
 *
 *      - you can realize only yuv to  rgb565 no rotate mode like this:
 *          rott.rot_mode = ROTT_ONLY_YUV2RGB565
 *          rott.input_addr = (void*)&yuyv_640_480[0];
 *          rott.output_addr = (uint8_t *)0x60000000;
 *          rott.input_fmt = PIXEL_FMT_YUYV;
 *          rott.input_flow = ROTT_INPUT_NORMAL;
 *          rott.output_flow = ROTT_OUTPUT_REVESE_HALFWORD_BY_HALFWORD;
 *          rott.picture_xpixel = 640;
 *          rott.picture_ypixel = 480;
 *          rott.block_xpixel = 80;
 *          rott.block_ypixel = 60;
 *          rott.block_cnt = 64;
 *          rott.watermark_blk = 32;
 *          rott_config(&rott);

 *      - you can realize only yuyv to  rgb565 rotate 270 mode like this:
 *          rott.rot_mode = ROTT_ROTATE270;
 *          rott.input_addr = (void*)&yuyv_640_480[0];
 *          rott.output_addr = (uint8_t *)0x60000000;
 *          rott.input_fmt = PIXEL_FMT_YUYV;
 *          rott.input_flow = ROTT_INPUT_NORMAL;
 *          rott.output_flow = ROTT_OUTPUT_REVESE_HALFWORD_BY_HALFWORD;
 *          rott.picture_xpixel = ROTT_XPIXE;
 *          rott.picture_ypixel = ROTT_YPIXE;
 *          rott.block_xpixel = ROTT_XBLOCK;
 *          rott.block_ypixel = ROTT_YBLOCK;
 *          rott.block_cnt = ROTT_BLOCK_NUM;
 *          rott.watermark_blk = ROTT_BLOCK_NUM >> 1;
 *          rott_config(&rott);
 *

 *      - you can realize only  rgb565 rotate 270 mode like this:
 *          rott.rot_mode = ROTT_ROTATE270;
 *          rott.input_fmt = PIXEL_FMT_RGB565;
 *          rott.input_addr = XXXX;
 *          .................
 *
 * @return
 *      - BK_OK: succeed
 *      - others: other errors.
 */
bk_err_t rott_config(rott_config_t *rott);


/**
  * @brief  set ahb master busrt len. 
  * @param  0:64 word per busrt; 1:32;2:16; 3:8; 4:4
  */
bk_err_t bk_rott_transfer_ability(rott_trans_ability_t ability);


#ifdef __cplusplus
}
#endif
