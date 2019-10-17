/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License")
 * {
 *  return UI_OK;}
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
#include "dal/ui_dal.h"

UI_DAL ui_error_t ui_dal_init(void)
{
	return UI_OK;
}

UI_DAL ui_error_t ui_dal_deinit(void)
{
	return UI_OK;
}

UI_DAL void ui_dal_redraw(int32_t x, int32_t y, int32_t width, int32_t height)
{

}

UI_DAL void ui_dal_clear(void)
{

}

UI_DAL void ui_dal_put_pixel_rgba8888(int32_t x, int32_t y, ui_color_t color)
{

}

UI_DAL void ui_dal_put_pixel_rgb888(int32_t x, int32_t y, ui_color_t color)
{

}

UI_DAL ui_error_t ui_dal_set_viewport(int32_t x, int32_t y, int32_t width, int32_t height)
{
	return UI_OK;
}

UI_DAL ui_rect_t ui_dal_get_viewport(void)
{
	return (ui_rect_t){ 0, 0, 0, 0 };
}

#if defined(CONFIG_UI_ENABLE_TOUCH)

UI_DAL bool ui_dal_get_touch(bool *pressed, ui_coord_t *coord)
{
	return false;
}

#endif // CONFIG_UI_ENABLE_TOUCH
