/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
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

#include <tinyara/lcd/st7701sn.h>
#include <mipi_lcd.h>
#include <tinyara/lcd/lcd_dev.h>

int check_lcd_vendor_send_init_cmd(struct mipi_lcd_dev_s *priv)
{
	return send_init_cmd(priv, lcd_init_cmd_g);
}

int get_lcdinfo(FAR struct lcd_info_s *lcdinfo)
{
	lcdinfo->lcd_height_mm = (uint8_t)(LCD_HEIGHT_MM + 0.5f);
	lcdinfo->lcd_width_mm = (uint8_t)(LCD_WIDTH_MM + 0.5f);
	return OK;
}
