#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>

/* @brief define image reseize target size
 *
 */
#define IMAGE_SRC_WIDTH      640 /** < source image width */
#define IMAGE_SRC_HEIGHT     480 /** < source image height */
#define IMAGE_DST_WIDTH      480 /** < target image width after resize */
#define IMAGE_DST_HEIGHT     320 /** < target image height after resize */

#define RGB565_RED   0xf800
#define RGB565_GREEN 0x07e0
#define RGB565_BLUE  0x001f
#define RATIO_R2V (1.403)
#define RATIO_G2U (0.343)
#define RATIO_G2V (0.714)
#define RATIO_B2U (1.770)
#define RESIZE_PRECI    64
#define YUV2RGB565_ENABLE        0
#define IMAGE_RESIZE_ENABLE      0

#define compare_max(a, b)      (a >= b ? a : b)
#define compare_min(a, b)      (a < b ? a: b)

#define RGB_FILP(var) ((var < 0) ? 0 : ((var > 255) ? 255 : var))
#define CLIP(X) ((X) > 255 ? 255 : (X) < 0 ? 0 : X)

#define TABLE_MAX_SIZE 256
//extern int TABLE_R2V[TABLE_MAX_SIZE];

/* @brief define rgb565 data type, two rgb565 combain a word
 *
 */

typedef enum
{
    YUV_ERR,
    YUV_444,
    YUV_422,
    YUV_420,
    YUV_400, // gray image
} yuv_enc_fmt_t;

typedef union
{
    struct
    {
        uint32_t b2 : 5;//bit[0-4]
        uint32_t g2 : 6;
        uint32_t r2 : 5;
        uint32_t b1 : 5;
        uint32_t g1 : 6;
        uint32_t r1 : 5;//bit[27-31]
    };
    uint32_t v;
} rgb565_t;



typedef enum
{
    PIXEL_MSB_YUYV = 0,
    PIXEL_MSB_VUYY,
    PIXEL_MSB_UYVY,
    PIXEL_MSB_YYUV,
    PIXEL_MSB_VYUY,
    PIXEL_MSB_RGB888,
    PIXEL_MSB_RGB565,
    PIXEL_MSB_ARGB8888,
} img_fmt;


typedef struct codec_img
{
    uint8_t *src_buf;
    uint8_t *dst_buf;
    img_fmt input_type;
    img_fmt output_type;
    uint32_t width;
    uint32_t height;
    media_rotate_t rotate;
    uint8_t if_resize;
    uint32_t new_width;
    uint32_t new_height;
} codec_img_t;



#ifdef __cplusplus
}
#endif

