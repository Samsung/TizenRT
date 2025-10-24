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
#include "driver/jpeg_dec_types.h"
#include <driver/media_types.h>


#ifdef __cplusplus
extern "C" {
#endif

 /* @brief Overview about jpeg dec API header
  *
  */

 /**
  * @brief HW JPEG DEC API
  * @{
  */


 /**
 * @brief     this api open the jpeg decode clk
 *
 */
bk_err_t bk_jpeg_dec_driver_init(void);


/**
* @brief	 this api close the jpeg decode clk
*          - disable jpeg decode
*/
bk_err_t bk_jpeg_dec_driver_deinit(void);


/**
* @brief	 this api used for jpeg decode config
*
* @param     xpixel jpeg decode picture x pixel
*		  - input_buf  decode input data addr
*        - output_buf decode output data addr
* @return
*	  - BK_OK: succeed
*	  - others: other errors.

* @return
*/
//bk_err_t bk_jpeg_dec_hw_init(uint16_t xpixel, uint16_t ypixel, uint32_t length, unsigned char *input_buf, unsigned char * output_buf);

/**
* @brief	 this api used for jpeg decode line interrupt config
*            decode can select a frame complete done isr(defult) , or this decode lin_num enter int isr
* @param     lin_num decode line num ,should multiple of 8,
*
* Usage example:
*
*		 if x pixel 320, you set bk_jpeg_dec_line_int_en(32), every 32 line will enter isr, in all ten times
*		 if x pixel 240, you set bk_jpeg_dec_line_int_en(24), every 24 line will enter isr, in all ten times
* @return
*	  - BK_OK: succeed
*	  - others: other errors.

* @return
*/
//bk_err_t bk_jpeg_dec_line_int_en(uint32_t line_num);

/**
* @brief	 this api used for disable jpeg decode line num int
*
* @return
*	  - BK_OK: succeed
*	  - others: other errors.
*/
bk_err_t bk_jpeg_dec_line_int_dis(void);

/**
* @brief	 this api start jpeg decode
*
* @param     length jpeg length
*           - input_buf jpeg data src addr
*           - output_buf jpeg decode output addr
*
* @return
*         0: jpeg decode ok;
*        others: error
*/
bk_err_t bk_jpeg_dec_hw_start(uint32_t length, unsigned char *input_buf, unsigned char * output_buf);

/**
* @brief	 this api start jpeg decode
*
* @param     length jpeg length
*           - input_buf jpeg data src addr
*           - output_buf jpeg decode output addr
*
* @return
*         0: jpeg decode ok;
*        others: error
*/
bk_err_t bk_jpeg_dec_dma_start(uint32_t length, unsigned char *input_buf, unsigned char * output_buf);


/**
* @brief	 stop jpeg decode
*
* @return
*         0: jpeg decode stop ok;
*        others: error
*/
bk_err_t bk_jpeg_dec_stop(void);

/**
* @brief	set jpeg decode out format, useful in bk7258
*
* @param pixel_fmt select, PIXEL_FMT_YUYV(default), PIXEL_FMT_VUYY, PIXEL_FMT_VYUY (not commonly used),PIXEL_FMT_YYUV(not commonly used)
*
* @return
*         BK_OK
*/
bk_err_t bk_jpeg_dec_out_format(pixel_format_t pixel_fmt);

/**
* @brief	when set decode by line, use this api config decode by how many lines
* @param select according to line_num_t enum,like 8/16/24/32....etc. multiple of 8 line
*/
void bk_jpeg_dec_line_num_set(line_num_t line_num);

/**
* @brief	start decode in every line decode isr
*/
void bk_jpeg_dec_by_line_start(void);

/**
* @brief set decode output addr,this api is changed by line decode mode isr
*/
bk_err_t jpeg_dec_set_dst_addr(unsigned char * output_buf);


/**
* @brief check decode by line is decode last line complete
* @return return true: last line complete
*/
bool bk_jpeg_dec_by_line_is_last_line_complete(void);

#if (USE_JPEG_DEC_COMPLETE_CALLBACKS == 1)

/**
* @brief	 this api used for register jpeg decode isr
*
* @param   isr_id  select from DEC_END_OF_FRAME or DEC_END_OF_LINE_NUM
*	     - cb_isr
*
* @return
*         0: jpeg decode ok;
*        others: error
*/
bk_err_t bk_jpeg_dec_isr_register(jpeg_dec_isr_type_t isr_id, jpeg_dec_isr_cb_t cb_isr);

#else
/**

 * @brief    This API is direct registerjpeg  dec cpu isr
 *
 * @param   isr: isr function
 *
 * @return
 *     - BK_OK: succeed
 *     - others: other errors.
 */
bk_err_t  bk_jpeg_dec_isr_register(jpeg_dec_isr_t jpeg_dec_isr);
#endif

/**
  * @}
  */


#ifdef __cplusplus
}
#endif









