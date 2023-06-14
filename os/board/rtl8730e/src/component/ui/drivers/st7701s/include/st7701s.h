#ifndef _ST7701S_H
#define _ST7701S_H

#include "ameba_soc.h"

typedef enum {
	RGB565,
	ARGB8888,
} st7701s_image_format;

void st7701s_init(int image_format);
u8 *st7701s_get_layer_buffer(int layer_id);
void st7701s_clean_invalidate_buffer(int layer_id);

#endif