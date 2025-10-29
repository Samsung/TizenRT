#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include "driver/media_types.h"
#include <modules/image_scale_types.h>


/* @brief Overview about image scale
 *
 */

/**
 * @brief according target image scale crop src image, and compress target image
 *	for exampe: src image is 1280*720P and target image is 320*480, you should crop the maximul picture to scale compress target picture.
 *	so this algorithns can crop w*h is 480*720 to scale 320*480p.
 */
int image_scale_crop_compress(const uint8_t* src_img, uint8_t* dst_img, 
		unsigned int src_width, unsigned int src_height,
		unsigned int dst_width, unsigned int dst_height);

/**
 * @brief according target image scale crop src image, and compress rotate 
 *	for exampe: src image is 1280*720P and target lcd is 320*480, so rotate is 480*320, 
 *  so you should crop the maximul picture to scale compress target picture.
 *	so this algorithns can crop w*h is 1080*720 to scale 480*320p.
 */
int image_scale_crop_compress_rotate(const uint8_t* src_img, uint8_t* dst_img, 
		unsigned int src_width, unsigned int src_height,
		unsigned int dst_width, unsigned int dst_height);

/**
 * @brief according target image scaling src image. 
 */
int image_16bit_scaling(const unsigned char* src_img, unsigned char* dst_img, 
		unsigned int src_width, unsigned int src_height, 
		unsigned int dst_width, unsigned int dst_height);


/**
 * @brief only crop src image to dst image.
 */
int image_center_crop(const uint8_t* src_img, uint8_t* dst_img, 
			unsigned int src_width, unsigned int src_height,
			unsigned int dst_width, unsigned int dst_height);


/**
 * @brief rgb565 clockwise rotate90
 */
int rgb565_rotate_degree90(unsigned char *src, unsigned char *dst, int img_width, int img_height);

/**
 * @brief rgb565 anticlockwise rotate270
 */
int rgb565_rotate_degree270(unsigned char *src, unsigned char *dst, int img_width, int img_height);

/**
 * @brief  argb888 32bit clockwise rotate90
 */
int argb8888_rotate_degree90(unsigned char *src, unsigned char *dst, int img_width, int img_height);

/**
 * @brief  argb888 32bit rotate270
 */
int argb8888_rotate_degree270(unsigned char *src, unsigned char *dst, int img_width, int img_height);

/**
 * @brief yuyv data clockwise rotate90, and output yuyv
 */
int yuyv_rotate_degree90_to_yuyv(unsigned char *yuyv, unsigned char *rotatedYuyv, int width, int height);

/**
 * @brief yuyv rotete 270 degree and output data format yuyv
 *
 * @param 1 src yuyv data
 *        2 rotate src yuyv 270 degrees to yuyv,the dst yuyyv
 *        3 src width
 *        4 src height
 * 
 * @return
 *     - BK_OK: succeed
 *     - others: other errors.
 */
int yuyv_rotate_degree270_to_yuyv(unsigned char *yuyv, unsigned char *rotatedYuyv, int width, int height);

/**
 * @brief yuyv rotete 180 degree and output data format yuyv
 *
 * @param 1 src yuyv data
 *        2 rotate src yuyv 180 degrees to yuyv,the dst yuyyv
 *        3 src width
 *        4 src height
 * 
 * @return
 *     - BK_OK: succeed
 *     - others: other errors.
 */
int yuyv_rotate_degree180_to_yuyv(unsigned char *yuyv, unsigned char *rotatedyuyv, int width, int height);

/**
 * @brief yuyv data rotate270, and output vuyy
 */
int yuyv_rotate_degree270_to_vuyy(unsigned char *src_yuyv, unsigned char *rotated_to_vuyy, int width, int height);

/**
 * @brief vuyy clockwise rotate90, and output yuyv
 */
int vuyy_rotate_degree90_to_yuyv(unsigned char *vuyy, unsigned char *rotatedVuyy, int width, int height);

/**
 * @brief vuyy clockwise rotate270, and output yuyv
 */
int vuyy_rotate_degree270_to_yuyv(unsigned char *vuyy, unsigned char *rotatedVuyy, int width, int height);

/**
 * @brief yuyv to rgb data clockwise rotate90
 */
int yuyv2rgb_rotate_degree90(unsigned char *yuyv, unsigned char *rotatedYuyv, int width, int height);

/**
 * @brief vuyy to rgb data clockwise rotate90
*/
int vuyy2rgb_rotate_degree90(unsigned char *vuyy, unsigned char *rotatedVuyy, int width, int height);

/**
 * @brief image scale init
 */
void image_scale_init(void);

/**
 * @brief  uyvy422 data convert to RGB565 data
 */
int vuyy_to_rgb565_convert(unsigned char *src_buf, unsigned char *out_buf,int img_width, int img_height);

/**
 * @brief  uyvy422 data convert to RGB565 data
 */
int uyvy_to_rgb565_convert(unsigned char *src_buf, unsigned char *out_buf,int img_width, int img_height);

/**
 * @brief  yuyv422 data convert to RGB565 data
 */
int yuyv_to_rgb565_convert(unsigned char *src_buf, unsigned char *out_buf, int img_width, int img_height);

/**
 * @brief   RGB565 data convert to uyvy422 data
 */
int rgb565_to_uyvy_convert(uint16_t *sourceLcdBuffer, uint16_t *destLcdBuffer,int img_width, int img_height);

/**
 * @brief   RGB565 data convert to yuyv422 data
 */
int rgb565_to_yuyv_convert(uint16_t *sourceLcdBuffer, uint16_t *destLcdBuffer,int img_width, int img_height);

int rgb565_to_rgb565le_convert(void *rgb565_buffer, int img_width, int img_height);

int rgb888_to_rgb565_convert(uint8_t *src_buffer, uint16_t *dst_buffer, int img_width, int img_height);
int rgb565_to_rgb888_convert(uint16_t *src_buffer, uint8_t *dst_buffer, int img_width, int img_height);

void yyuv_to_rgb888(uint8_t *input_ptr, uint8_t *output_ptr, uint32_t width, uint32_t height);


/**
 * @brief   yuyv (H->L : YUYV) convert to rgb888
 */
void yuyv_to_rgb888_convert(uint8_t *input_ptr, uint8_t *output_ptr, uint32_t width, uint32_t height);

void vuyy_to_rgb888(uint8_t *input_ptr, uint8_t *output_ptr, uint32_t width, uint32_t height);

void vyuy_to_rgb888_convert(uint8_t *input_ptr, uint8_t *output_ptr, uint32_t width, uint32_t height);


int rgb565_to_vuyy_convert(uint16_t *sourceLcdBuffer, uint16_t *destLcdBuffer,int img_width, int img_height);

/*yuyv to rgb565*/
int yuyv_to_rgb565(uint8_t *src_buff, uint8_t *dst_buff, int width, int height);

/*vuyy to rgb565*/
int vuyy_to_rgb565(uint8_t *src_buff, uint8_t *dst_buff, int width, int height);


void argb8888_to_vuyy_blend(uint8_t *src, uint8_t *dst, uint32_t width, uint32_t height);
void argb8888_to_yuyv_blend(uint8_t *src, uint8_t *dst, uint32_t width, uint32_t height);
void argb8888_to_rgb565_blend(uint8_t *src, uint8_t *dst, uint32_t width, uint32_t height);
void argb8888_to_rgb888_blend(uint8_t *src, uint8_t *dst, uint32_t width, uint32_t height);

int vuyy_image_resize(uint8_t *src_img, uint8_t *dst_img, uint32_t src_width, uint32_t src_height, uint32_t dst_width, uint32_t dst_height);

// 640X480-->800X480
int vuyy_image_vga_to_lvga(uint8_t * src_buf, uint8_t *dst_buf, uint8_t row_count);

// 640X480-->480X320
int vuyy_image_vga_to_rsvga(uint8_t * src_buf, uint8_t *dst_buf, uint8_t row_count);

// 640X480-->320X240
int vuyy_image_vga_to_qvga(uint8_t * src_buf, uint8_t *dst_buf, uint8_t row_count);

// 640X480-->320X240
int yuyv_image_vga_to_qvga(uint8_t * src_buf, uint8_t *dst_buf, uint8_t row_count);

yuv_enc_fmt_t bk_get_original_jpeg_encode_data_format(uint8_t *src_buf, uint32_t length);


/*
 * @}
 */

#ifdef __cplusplus
	}
#endif



