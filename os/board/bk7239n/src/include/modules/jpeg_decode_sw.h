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

#include <common/bk_include.h>
#include <modules/jpeg_decode_sw_types.h>
#include <modules/tjpgd.h>

#ifdef __cplusplus
extern "C" {
#endif

/* @brief Overview about this API header
 *
 */

/**
 * @brief JPEG_DEC APIs Version 1.0
 * @{
 */

/**
 * @brief     Init the jpeg_dec
 *
 * This API init the jpeg_dec
 *  - config jpeg_dec image resoult, and simple_rate
 *  - malloc buffer for jpeg_dec
 *
 * @param jd_in inter buffer
 * @param len buffer length
 *
 * @attention 1. only for software jpeg_dec 
 *
 * @return
 *    - BK_OK: succeed
 *    - other: errors.
 */
bk_err_t bk_jpeg_dec_sw_init(void* jd_in, uint32_t len);

/**
 * @brief     Deinit the jpeg_dec
 *
 * This API deinit the jpeg_dec
 *  - clear all configure for jpeg_dec
 *  - free buffer for jpeg_dec
 *
 * @attention 1. only for software jpeg_dec 
 *
 * @return
 *    - BK_OK: succeed
 *    - other: errors.
 */
bk_err_t bk_jpeg_dec_sw_deinit(void);

/**
 * @brief     jpeg_decoder_fun
 *
 * This API jpeg_decoder_fun
 *  - jpeg_prepare for jpeg_dec
 *  - start jpeg_dec
 *
 * @param decode_type 0:decode by line, one time decode 16line; 1:decode by frame
 * @param src_buf jpeg image address
 * @param dst_buf decode output buffer address
 * @param outbuf_size decode output buffer size
 * @param result decode image info
 *
 * @attention 1. only for software jpeg_dec 
 * @attention 2. outbuf_size only work for decode_type=0, decode by frame, and outbuf size must 2 times for decode 16line
 *
 * @return
 *    - BK_OK: succeed
 *    - other: errors.
 */
bk_err_t bk_jpeg_dec_sw_start(uint8_t decode_type, uint8_t *src_buf, uint8_t *dst_buf, uint32_t jpeg_size, uint32_t outbuf_size, sw_jpeg_dec_res_t *result);

/**
 * @brief     start line jpeg decode sw
 *
 * This API is used to decode go on
 *
 * @attention 1. only for software jpeg_dec by line
 *
 * @return
 *    - BK_OK: succeed
 *    - other: errors.
 */
bk_err_t bk_jpeg_dec_sw_start_line(void);

/**
 * @brief     jpeg_decoder_fun
 *
 * This API is used to get img width and size
 *
 * @param frame_size input image size
 * @param src_buf jpeg image address
 * @param result decode image info
 *
 * @attention 1. only for software jpeg_dec 
 *
 * @return
 *    - BK_OK: succeed
 *    - other: errors.
 */
bk_err_t bk_jpeg_get_img_info(uint32_t frame_size, uint8_t *src_buf, sw_jpeg_dec_res_t *result);

/**
 * @brief     jpegdec register finish icallback_func
 *
 * This API jpeg_decoder_fun, the callback function wiil execute when decode success
 *
 * @param cb finish jpeg_dec callback function
 *
 * @attention 1. only for software jpeg_dec 
 *
 * @return
 */
void bk_jpeg_dec_sw_register_finish_callback(void *cb);

/**
 * @brief     jpeg_decoder_fun
 *
 * This API jpeg_decoder_fun
 *  - jpeg_prepare for jpeg_dec
 *  - start jpeg_dec
 *
 * @param decode_type 0:decode by line, one time decode 16line; 1:decode by frame
 * @param src_buf jpeg image address
 * @param dst_buf decode output buffer address
 * @param outbuf_size decode output buffer size
 * @param result decode image info
 * @param scaling of decode image
 * @param format of decode image
 * @param rotate_angle of decode image
 * @param work_buffer inter workbuffer
 * @param rotate_buffer rotate need buffer
 *
 * @attention 1. only for software jpeg_dec 
 * @attention 2. outbuf_size only work for decode_type=0, decode by frame, and outbuf size must 2 times for decode 16line
 *
 * @return
 *    - BK_OK: succeed
 *    - other: errors.
 */
bk_err_t bk_jpeg_dec_sw_start_one_time(
		uint8_t decode_type,
		uint8_t *src_buf,
		uint8_t *dst_buf,
		uint32_t jpeg_size,
		uint32_t outbuf_size,
		sw_jpeg_dec_res_t *result,
		uint8_t scale,
		JD_FORMAT_OUTPUT format,
		media_rotate_t rotate_angle,
		uint8_t *work_buffer,
		uint8_t *rotate_buffer);

/**
 * @}
 */


#ifdef __cplusplus
}
#endif
