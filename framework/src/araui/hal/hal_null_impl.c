/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <araui/ui_commons.h>
#include <araui/ui_asset.h>
#include "hal/ui_hal_interface.h"

// When a hardware key has any action, 
// Should call one of handler function.
//
// Handler functions:
//  - ui_error_t ui_core_handle_key_down(uint32_t key_code);
//  - ui_error_t ui_core_handle_key_up(uint32_t key_code);
//  - ui_error_t ui_core_handle_key_long_press(uint32_t key_code);

ui_error_t ui_hal_init(void)
{
	return UI_OK;
}

ui_error_t ui_hal_deinit(void)
{
	return UI_OK;
}

void ui_hal_redraw(int32_t x, int32_t y, int32_t width, int32_t height)
{
}

ui_error_t ui_hal_clear(ui_color_t color)
{
	return UI_OK;
}

inline void ui_hal_draw_pixel(int32_t x, int32_t y, ui_color_t color)
{
}

void ui_hal_draw_image(int32_t x, int32_t y, ui_asset_t image)
{
}

void ui_hal_draw_cropped_resized_image(int32_t x, int32_t y, ui_asset_t image, ui_rect_t crop_rect, int32_t resized_width, int32_t resized_height)
{
}

ui_error_t ui_hal_set_viewport(int32_t x, int32_t y, int32_t width, int32_t height)
{
	return UI_OK;
}

ui_rect_t ui_hal_get_viewport(void)
{
	static ui_rect_t rect = {0, 0, 0, 0};
	return rect;
}

bool ui_hal_get_touch(bool *pressed, ui_coord_t *coord)
{
	return false;
}

ui_color_t ui_hal_get_pixel_color(int32_t x, int32_t y)
{
	return 0x0000;
}

