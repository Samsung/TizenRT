/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <sys/types.h>
#include "lcd_logo.h"
#include <debug.h>

extern const uint8_t lcd_logo_raw_data[LOGO_XRES * LOGO_YRES * 2];
uint8_t *lcd_init_fullscreen_image = NULL;

#if defined(CONFIG_LCD_SW_ROTATION)
extern uint8_t* get_lcd_buffer(void);
#endif
/* Memory optimization applied in case of software rotation is used in driver Rotation buffer
 * If rotation is enabled, then we have two buffers allocated for rotation.
 * During bootup, rotation buffer will not be used (No putarea call from application)
 * Therefore, the rotation buffer can be safely used for storing logo data.
 * 
 * If rotation is disabled, then we need to allocate memory for full screen data
 * and it will allocate memory to lcd_init_fullscreen_image buffer.
 * 
 * If rotation is enabled, then lcd_init_fullscreen_image contains pointer of rotation buffer
 * 
 * Define a callback function in driver get_lcd_buffer() which can return pointer
 * of the buffer used for rotation. */

uint8_t* lcd_logo_allocate_buffer(void)
{
#if defined(CONFIG_LCD_SW_ROTATION)
	lcd_init_fullscreen_image = get_lcd_buffer();
#else
	lcd_init_fullscreen_image = (uint8_t *)kmm_malloc(CONFIG_LCD_XRES * CONFIG_LCD_YRES * 2 + 1);
	if (!lcd_init_fullscreen_image) {
		lcddbg("ERROR: LCD logo data memory allocation failed\n");
		return;
	}
#endif
}

void lcd_logo_release_buffer(void)
{
#if !defined(CONFIG_LCD_SW_ROTATION)
	if (lcd_init_fullscreen_image != NULL) {
		kmm_free(lcd_init_fullscreen_image);
	}
#endif
	if (!lcd_init_fullscreen_image) {
		lcd_init_fullscreen_image = NULL;
	}
}

void lcd_logo_fill_buffer(int xres, int yres)
{
	int logo_arr_index = 0;
	int lcd_data_col_count = 0;
	int lcd_data_index = xres * (yres - LOGO_YRES) + (xres - LOGO_XRES);

	if (!lcd_init_fullscreen_image) {
		lcddbg("ERROR: LOGO buffer is not initialized\n");
	}

	memset(lcd_init_fullscreen_image, LCD_BLACK_VAL, xres * yres * 2);
#if defined(CONFIG_LCD_LOGO)
	while (logo_arr_index < (LOGO_YRES * LOGO_XRES * 2)) {
		lcd_init_fullscreen_image[lcd_data_index] = lcd_logo_raw_data[logo_arr_index++];
		lcd_init_fullscreen_image[lcd_data_index + 1] = lcd_logo_raw_data[logo_arr_index++];
		lcd_data_index += 2;
		lcd_data_col_count += 1;
		if (lcd_data_col_count == LOGO_XRES) {
			lcd_data_index += ((xres - LOGO_XRES) * 2);
			lcd_data_col_count = 0;
		}
	}
#endif
}
