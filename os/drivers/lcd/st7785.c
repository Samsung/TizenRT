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

static struct mipi_lcd_dev_s g_lcdcdev;
static struct mipi_lcd_config_s g_st7785_config;

int check_lcd_vendor_send_init_cmd(struct mipi_lcd_dev_s *priv)
{
	/* Only one config is set in the build configuration. If it's true, use that init code. Otherwise, check vendor id. */
#if NUM_LCD_VENDERS == 1
#if defined(CONFIG_LCD_INIT_AVD_2701)
	return send_init_cmd(priv, lcd_init_cmd_g_avd_2701);
#elif defined(CONFIG_LCD_INIT_AVD_NB01)
	return send_init_cmd(priv, lcd_init_cmd_g_avd_nb01);
#elif defined(CONFIG_LCD_INIT_HLT_A196)
	return send_init_cmd(priv, lcd_init_cmd_g_hlt_a196);
#elif defined(CONFIG_LCD_INIT_TEC_181A)
	return send_init_cmd(priv, lcd_init_cmd_g_tec_181a);
#endif
#elif NUM_LCD_VENDERS > 1
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

	for (int i = 0; i < NUM_LCD_VENDORS_ID; i++) {
		if (combined_id == g_lcd_vendors[i].id) {
			return send_init_cmd(priv, g_lcd_vendors[i].init_cmd);
		}
	}
	lcddbg("LCD ST7785 not recognized\n");
	return ERROR;
#else
#error "No LCD init code defined"
#endif	
}

int get_lcdinfo(FAR struct lcd_info_s *lcdinfo)
{
	lcdinfo->lcd_height_mm = (uint8_t)(LCD_HEIGHT_MM + 0.5f);
	lcdinfo->lcd_width_mm = (uint8_t)(LCD_WIDTH_MM + 0.5f);
	return OK;
}

static const struct lcd_panel_ops g_panel_ops = {
	.send_init_cmd = check_lcd_vendor_send_init_cmd,
	.get_lcdinfo = get_lcdinfo,
};

struct mipi_lcd_dev_s* st7785_initialize(void) {
	FAR struct mipi_lcd_dev_s *priv = &g_lcdcdev;
	priv->panel.mipi_host_config = &g_st7785_config;
	
	config.XPixels = CONFIG_LCD_XRES;
	config.YPixels = CONFIG_LCD_YRES;
	config.mipi_frame_rate = MIPI_FRAME_RATE;
	config.mipi_dsi_HBP = MIPI_DSI_HBP;
	config.mipi_dsi_HFP = MIPI_DSI_HFP;
	config.mipi_dsi_HSA = MIPI_DSI_HSA;
	config.mipi_dsi_RTNI = MIPI_DSI_RTNI;
	config.mipi_dsi_VBP = MIPI_DSI_VBP;
	config.mipi_dsi_VFP = MIPI_DSI_VFP;
	config.mipi_dsi_VSA = MIPI_DSI_VSA;
	config.mipi_lcd_limit = MIPI_LCD_LIMIT;
	config.lcd_lane_num = MIPI_LANE_NUMBER;
	priv->panel.mipi_host_config = &g_st7785_config;

	priv->panel.width = LCD_HEIGHT_MM;
	priv->panel.height = LCD_WIDTH_MM;
	// priv->panel.lcd_size_inch = 
	// priv->panel.lcd_dpi = 

	priv->panel.ops =  g_panel_ops;

	return &priv;
}