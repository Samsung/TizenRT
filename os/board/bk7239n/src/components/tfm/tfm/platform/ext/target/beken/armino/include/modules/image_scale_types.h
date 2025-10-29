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


/* @brief define rgb565 data type, two rgb565 combain a word
 *
 */

typedef union{
	struct {
		uint32_t b2 : 5;//bit[0-4]
		uint32_t g2 : 6;
		uint32_t r2 : 5;
		uint32_t b1 : 5;
		uint32_t g1 : 6;
		uint32_t r1 : 5;//bit[27-31]
	};
	uint32_t v;
} rgb565_t;


#ifdef __cplusplus
	}
#endif

