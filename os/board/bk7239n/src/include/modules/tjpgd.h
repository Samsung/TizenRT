/*----------------------------------------------------------------------------/
/ TJpgDec - Tiny JPEG Decompressor R0.03 include file         (C)ChaN, 2021
/----------------------------------------------------------------------------*/
#ifndef DEF_TJPGDEC
#define DEF_TJPGDEC

#include "driver/media_types.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef enum{
	JD_FORMAT_RGB888 = 0,
	JD_FORMAT_RGB565 = 1,
	JD_FORMAT_Grayscale = 2,
	JD_FORMAT_YUYV,
	JD_FORMAT_VYUY,
	JD_FORMAT_VUYY,
}JD_FORMAT_OUTPUT;
/* Specifies output pixel format.
/  0: RGB888 (24-bit/pix)
/  1: RGB565 (16-bit/pix)
/  2: Grayscale (8-bit/pix)
/  3: VYUY (16-bit/pix)
/  4: YUYV (16-bit/pix)
*/

typedef enum{
	JD_LITTLE_ENDIAN = 0,
	JD_BIG_ENDIAN
} JD_BYTE_ORDER;
/* Specifies output byte order
/  0: LITTLE_ENDIAN
/  1: BIG_ENDIAN
*/

typedef enum{
	JD_NO_BUFFER_COPY = 0,
	JD_SINGLE_BUFFER_COPY,
	JD_DOUBLE_BUFFER_COPY,
} JD_COPY_TYPE_T;


typedef struct{
	JD_FORMAT_OUTPUT format;
	JD_BYTE_ORDER byte_order;
	uint8_t scale;
} jd_output_format;

void jd_set_output_format(jd_output_format *output_format);
void jd_set_format(JD_FORMAT_OUTPUT format);
JD_FORMAT_OUTPUT jd_get_format(void);
void jd_set_scale(uint8_t scale);
int jd_get_scale(void);
void jd_set_byte_order(JD_BYTE_ORDER byte_order);
JD_BYTE_ORDER jd_get_byte_order(void);
media_rotate_t jd_get_rotate(void);
void jd_set_rotate(media_rotate_t rotate_angle, uint8_t *rotate_buf);

void jd_set_jpg_copy_func(uint8_t *copy_buffer_1, uint8_t *copy_buffer_2, uint32_t copy_buffer_size, void *jpg_copy_func, JD_COPY_TYPE_T type);

#ifdef __cplusplus
}
#endif

#endif /* _TJPGDEC */
