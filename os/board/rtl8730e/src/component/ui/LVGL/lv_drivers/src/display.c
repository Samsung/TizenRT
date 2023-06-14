/*
 * Copyright (c) 2021 Realtek, LLC.
 * All rights reserved.
 *
 * Licensed under the Realtek License, Version 1.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License from Realtek
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "lvgl.h"
#include "ameba_soc.h"
#include "display.h"
#ifdef CONFIG_DISDLAY_DRIVER_ST7701S_EN
#include "st7701s.h"
#endif

#define LAYER_ID 2
#define DISPLAY_WIDTH   480
#define DISPLAY_HEIGHT  800

static u8 *g_img_buffer = NULL;

void display_init()
{
#ifdef CONFIG_DISDLAY_DRIVER_ST7701S_EN
	int image_format = 0;
	if (LV_COLOR_DEPTH == 16) {
		image_format = RGB565;
	} else {
		image_format = ARGB8888;
	}
	st7701s_init(image_format);
	g_img_buffer = st7701s_get_layer_buffer(LAYER_ID);
#endif
}

void display_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
	if (!g_img_buffer) {
		return;
	}

	u32 y, i;

	u32 w = ((u32) area->x2 - (u32) area->x1 + 1);
	u32 h = ((u32) area->y2 - (u32) area->y1 + 1);

	if (w == DISPLAY_WIDTH) {
		memcpy(g_img_buffer + (u32) area->y1 * sizeof(u8) * DISPLAY_WIDTH * LV_COLOR_DEPTH / 8,
			   (u8 *) color_p, sizeof(u8) * DISPLAY_WIDTH * LV_COLOR_DEPTH / 8 * h);
	} else {
		for (y = 0, i = (u32) area->y1 ; i <= (u32) area->y2 ; ++i, ++y) {
			memcpy(g_img_buffer + (u32) area->x1 * sizeof(u8) * LV_COLOR_DEPTH / 8 + i * sizeof(u8) * DISPLAY_WIDTH * LV_COLOR_DEPTH / 8,
				   (u8 *) color_p + y * sizeof(u8) * w * LV_COLOR_DEPTH / 8,
				   sizeof(u8) * w * LV_COLOR_DEPTH / 8);
		}
	}

#ifdef CONFIG_DISDLAY_DRIVER_ST7701S_EN
	st7701s_clean_invalidate_buffer(LAYER_ID);
#endif

	lv_disp_flush_ready(disp_drv);
}