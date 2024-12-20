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

// Contains desired LOGO pixels
const uint8_t lcd_logo_raw_data[LOGO_XRES * LOGO_YRES * 2] = {LCD_BLACK_VAL, };

void lcd_logo_fill_buffer(uint8_t *lcd_init_fullscreen_image)
{
	int logo_arr_index = 0;
	int lcd_data_col_count = 0;
	int lcd_data_index = CONFIG_LCD_XRES * (CONFIG_LCD_YRES - LOGO_YRES) + (CONFIG_LCD_XRES - LOGO_XRES);
	while (logo_arr_index < (LOGO_YRES * LOGO_XRES * 2)) {
		lcd_init_fullscreen_image[lcd_data_index] = lcd_logo_raw_data[logo_arr_index++];
		lcd_init_fullscreen_image[lcd_data_index + 1] = lcd_logo_raw_data[logo_arr_index++];
		lcd_data_index += 2;
		lcd_data_col_count += 1;
		if (lcd_data_col_count == LOGO_XRES) {
			lcd_data_index += ((CONFIG_LCD_XRES - LOGO_XRES) * 2);
			lcd_data_col_count = 0;
		}
	}
}
