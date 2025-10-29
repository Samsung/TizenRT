#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>

#include <modules/image_scale_types.h>

#if CONFIG_SYS_CPU1
#define MINOOR_ITCM __attribute__((section(".itcm_sec_code ")))
#else
#define MINOOR_ITCM
#endif

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
 * @brief according target image scaling and rotate src image and display in lcd center. 
 */
int image_16bit_scaling_rotate(const unsigned char* src_img, unsigned char* dst_img, 
		unsigned int src_width, unsigned int src_height, 
		unsigned int dst_width, unsigned int dst_height);

/**
 * @brief only crop src image to dst image.
 */
int image_center_crop(const uint8_t* src_img, uint8_t* dst_img, 
			unsigned int src_width, unsigned int src_height,
			unsigned int dst_width, unsigned int dst_height);

/**
 * @brief anticlockwise rotate90
 */
int image_16bit_rotate90_anticlockwise(uint8_t *des,const uint8_t *src, uint32_t img_width, uint32_t img_height);

/**
 * @brief  clockwise rotate90
 */
int image_16bit_rotate90_clockwise(uint8_t *des, uint8_t *src, uint32_t img_width, uint32_t img_height);

/**
 * @brief yuyv data clockwise rotate90
 */
MINOOR_ITCM int yuyv_rotate_degree90(unsigned char *yuyv, unsigned char *rotatedYuyv, int width, int height);

/**
 * @brief yuyv data clockwise rotate90
 */
MINOOR_ITCM int vuyy_rotate_degree90(unsigned char *vuyy, unsigned char *rotatedVuyy, int width, int height);

/**
 * @brief yuyv data clockwise rotate270
 */
MINOOR_ITCM int vuyy_rotate_degree270(unsigned char *vuyy, unsigned char *rotatedVuyy, int width, int height);


/**
 * @brief yuyv to rgb data clockwise rotate90
 */
MINOOR_ITCM int yuyv2rgb_rotate_degree90(unsigned char *yuyv, unsigned char *rotatedYuyv, int width, int height);

/**
 * @brief vuyy to rgb data clockwise rotate90
*/
MINOOR_ITCM int vuyy2rgb_rotate_degree90(unsigned char *vuyy, unsigned char *rotatedVuyy, int width, int height);

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



void yyuv_to_rgb888(uint8_t *input_ptr, uint8_t *output_ptr, uint32_t width, uint32_t height);


/**
 * @brief   yuyv (H->L : YUYV) convert to rgb888
 */
void yuyv_to_rgb888_convert(uint8_t *input_ptr, uint8_t *output_ptr, uint32_t width, uint32_t height);

void vuyy_to_rgb888(uint8_t *input_ptr, uint8_t *output_ptr, uint32_t width, uint32_t height);

int rgb565_to_vuyy_convert(uint16_t *sourceLcdBuffer, uint16_t *destLcdBuffer,int img_width, int img_height);

/*yuyv to rgb565*/
MINOOR_ITCM int yuyv_to_rgb565(uint8_t *src_buff, uint8_t *dst_buff, int width, int height);

/*vuyy to rgb565*/
MINOOR_ITCM int vuyy_to_rgb565(uint8_t *src_buff, uint8_t *dst_buff, int width, int height);


MINOOR_ITCM void argb8888_to_vuyy_blend(uint8_t *src, uint8_t *dst, uint32_t width, uint32_t height);
MINOOR_ITCM void argb8888_to_yuyv_blend(uint8_t *src, uint8_t *dst, uint32_t width, uint32_t height);

MINOOR_ITCM int vuyy_image_resize(uint8_t *src_img, uint8_t *dst_img, uint32_t src_width, uint32_t src_height, uint32_t dst_width, uint32_t dst_height);

MINOOR_ITCM int vuyy_image_vga_to_lvga(uint8_t * src_buf, uint8_t *dst_buf, uint8_t row_count);

MINOOR_ITCM int vuyy_image_vga_to_rsvga(uint8_t * src_buf, uint8_t *dst_buf, uint8_t row_count);

/*
 * @}
 */

#ifdef __cplusplus
	}
#endif



