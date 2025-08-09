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

#include <tinyara/lcd/st7785.h>
#include <mipi_lcd.h>
#include <tinyara/lcd/lcd_dev.h>
#include <debug.h>

int check_lcd_vendor_send_init_cmd(struct mipi_lcd_dev_s *priv)
{
	/* Check Vendor */
	lcm_setting_table_t read_display_cmd = {0x04, 0, {0x00}};
	uint8_t rxbuf[3] = {0xFF, 0xFF, 0xFF};
	uint8_t length = sizeof(rxbuf) / sizeof(rxbuf[0]);
	uint32_t combined_id;
	int status;
	status = set_return_packet_len(priv, length);
	if (status != OK) {
		lcddbg("Fail set return pkt len!\n");

		/* 
		allow this error case to drop down to next, allowing the MIPI DPHY to read ANY response length
		because it is possible that some other response may be received outside of expected length 
		as long as LCD responds with anything, we can properly reset it back to a working state
		*/
	}
	status = read_response(priv, read_display_cmd, rxbuf, length);
	if (status != OK) {
		lcddbg("Fail read resp!\n");
		return status;
	}
	combined_id = (rxbuf[0] << 16) | (rxbuf[1] << 8) | rxbuf[2];

	lcddbg("LCD ID: %6x\n", combined_id);

	for (int i = 0; i < NUM_LCD_VENDORS; i++) {
		if (combined_id == g_lcd_vendors[i].id) {
			return send_init_cmd(priv, g_lcd_vendors[i].init_cmd);
		}
	}
	lcddbg("LCD ST7785 not recognized\n");
	return ERROR;
}

int get_lcdinfo(FAR struct lcd_info_s *lcdinfo)
{
	lcdinfo->lcd_height_mm = (uint8_t)(LCD_HEIGHT_MM + 0.5f);
	lcdinfo->lcd_width_mm = (uint8_t)(LCD_WIDTH_MM + 0.5f);
	return OK;
}
