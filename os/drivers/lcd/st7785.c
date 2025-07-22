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
#include <debug.h>

int check_lcd_vendor_send_init_cmd(struct mipi_lcd_dev_s *priv)
{
	/* Check Vendor */
	lcm_setting_table_t read_display_cmd = {0x04, 0, {0x00}};
	uint8_t rxbuf[3] = {0xFF, 0xFF, 0xFF};
	uint8_t length = sizeof(rxbuf) / sizeof(rxbuf[0]);
	int status;
	status = set_return_packet_len(priv, length);
	if (status != OK) {
		return status;
	}
	status = read_response(priv, read_display_cmd, rxbuf, length);
	if (status != OK) {
		return status;
	}
	for (int i = 0; i < length; i++) {		/* LCD info */
		lcdvdbg("%2x\n", rxbuf[i]);
	}

	for (int i = 0; i < NUM_LCD_VENDORS; i++) {
		if (rxbuf[0] == g_lcd_vendors[i].id[0] && rxbuf[1] == g_lcd_vendors[i].id[1] && rxbuf[2] == g_lcd_vendors[i].id[2]) {
			return send_init_cmd(priv, g_lcd_vendors[i].init_cmd);
		}
	}
	lcddbg("LCD ST7785 not recognized\n");
	return ERROR;
}
