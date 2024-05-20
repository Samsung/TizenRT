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

#include <tinyara/config.h>
#include <tinyara/lcd/lcd.h>
#include <tinyara/lcd/lcd_dev.h>
#if defined(CONFIG_LCD_ST7785)
#include <tinyara/lcd/st7785.h>
#elif defined(CONFIG_LCD_ST7701)
#include <tinyara/lcd/st7701.h>
#endif
#include <tinyara/mipidsi/mipi_dsi.h>
#include <tinyara/mipidsi/mipi_display.h>
#include <debug.h>
#include <assert.h>

#define REGFLAG_DELAY                       0xFC
#define REGFLAG_END_OF_TABLE                0xFD	// END OF REGISTERS MARKER

#if !defined(CONFIG_LCD_MAXPOWER) || CONFIG_LCD_MAXPOWER < 1
#define CONFIG_LCD_MAXPOWER 100
#endif

struct mipi_lcd_dev_s {
	/* Publicly visible device structure */

	struct lcd_dev_s dev;

	FAR struct mipi_dsi_device *dsi_dev;

	u8 *lcd_img_buffer[MAX_NO_PLANES];
	//u8 *BackupLcdImgBuffer;
	int fb_alloc_count;
	uint8_t power;
	struct mipi_lcd_config_s *config;
};

static struct mipi_lcd_dev_s g_lcdcdev;

static int send_cmd(struct mipi_lcd_dev_s *priv, lcm_setting_table_t command)
{
	int transfer_status = OK;
	u8 cmd = command.cmd;
	u8 *cmd_addr = command.para_list;
	u32 payload_len = command.count;
	struct mipi_dsi_msg msg;
	msg.channel = cmd;
	if (payload_len == 0) {
		msg.type = MIPI_DSI_DCS_SHORT_WRITE_0_PARAM;
	} else if (payload_len == 1) {
		msg.type = MIPI_DSI_DCS_SHORT_WRITE_1_PARAM;
	} else {
		msg.type = MIPI_DSI_DCS_LONG_WRITE;
	}
	msg.tx_buf = cmd_addr;
	msg.tx_len = payload_len;
	msg.flags = 0;
	priv->config->lcd_mode_switch(false);
	transfer_status = mipi_dsi_transfer(priv->dsi_dev, &msg);
	priv->config->lcd_mode_switch(true);
	if (transfer_status != OK) {
		lcddbg("Command %x not sent \n", cmd);
	}
	return transfer_status;
}

static int send_init_cmd(struct mipi_lcd_dev_s *priv, lcm_setting_table_t *table)
{
	u8 send_cmd_idx_s = 0;
	u32 payload_len;
	u8 cmd;
	u8 *cmd_addr;
	int transfer_status = 0;
	struct mipi_dsi_msg msg;
	while (1) {
		cmd = table[send_cmd_idx_s].cmd;
		switch (cmd) {
		case REGFLAG_DELAY:
			usleep(table[send_cmd_idx_s].count * 1000);
			break;
		case REGFLAG_END_OF_TABLE:
			msg.type = MIPI_DSI_END_OF_TRANSMISSION;
			return mipi_dsi_transfer(priv->dsi_dev, &msg);
		default:
			cmd_addr = table[send_cmd_idx_s].para_list;
			payload_len = table[send_cmd_idx_s].count;
			msg.channel = cmd;
			if (payload_len == 0) {
				msg.type = MIPI_DSI_DCS_SHORT_WRITE_0_PARAM;
			} else if (payload_len == 1) {
				msg.type = MIPI_DSI_DCS_SHORT_WRITE_1_PARAM;
			} else {
				msg.type = MIPI_DSI_DCS_LONG_WRITE;
			}
			msg.tx_buf = cmd_addr;
			msg.tx_len = payload_len;
			msg.flags = 0;
			transfer_status = mipi_dsi_transfer(priv->dsi_dev, &msg);
			if (transfer_status != OK) {
				lcddbg("Command %x not sent \n", cmd);
				return transfer_status;
			}
		}
		send_cmd_idx_s++;
	}
}

/* LCD Data Transfer Methods */

/* LCD Specific Controls */

/****************************************************************************
 * Name:  lcd_putrun
 *
 * Description:
 *   This method can be used to write a partial raster line to the LCD:
 *
 *   dev     - The lcd device
 *   row     - Starting row to write to (range: 0 <= row < yres)
 *   col     - Starting column to write to (range: 0 <= col <= xres-npixels)
 *   buffer  - The buffer containing the run to be written to the LCD
 *   npixels - The number of pixels to write to the LCD
 *             (range: 0 < npixels <= xres-col)
 *
 ****************************************************************************/

static int lcd_putrun(FAR struct lcd_dev_s *dev, fb_coord_t row, fb_coord_t col, FAR const uint8_t *buffer, size_t npixels)
{
	return OK;
}

/****************************************************************************
 * Name:  lcd_putarea
 *
 * Description:
 *   This method can be used to write a partial area to the LCD:
 *
 *   dev       - The lcd device
 *   row_start - Starting row to write to (range: 0 <= row < yres)
 *   row_end   - Ending row to write to (range: row_start <= row < yres)
 *   col_start - Starting column to write to (range: 0 <= col <= xres)
 *   col_end   - Ending column to write to
 *               (range: col_start <= col_end < xres)
 *   buffer    - The buffer containing the area to be written to the LCD
 *   stride    - Length of a line in bytes. This parameter may be necessary
 *               to allow the LCD driver to calculate the offset for partial
 *               writes when the buffer needs to be splited for row-by-row
 *               writing.
 *
 ****************************************************************************/

static int lcd_putarea(FAR struct lcd_dev_s *dev, fb_coord_t row_start, fb_coord_t row_end, fb_coord_t col_start, fb_coord_t col_end, FAR const uint8_t *buffer, fb_coord_t stride)
{
	FAR struct mipi_lcd_dev_s *priv = (FAR struct mipi_lcd_dev_s *)dev;
	//coordinate shift from (0,0) -> (1,1) and (XRES-1,YRES-1) -> (XRES,YRES)
	row_start += 1;
	row_end += 1;
	col_start += 1;
	col_end += 1;
	priv->config->lcd_put_area(buffer, row_start, col_start, row_end, col_end);
	return OK;
}

/****************************************************************************
 * Name:  lcd_getrun
 *
 * Description:
 *   This method can be used to read a partial raster line from the LCD:
 *
 *  dev     - The lcd device
 *  row     - Starting row to read from (range: 0 <= row < yres)
 *  col     - Starting column to read read (range: 0 <= col <= xres-npixels)
 *  buffer  - The buffer in which to return the run read from the LCD
 *  npixels - The number of pixels to read from the LCD
 *            (range: 0 < npixels <= xres-col)
 *
 ****************************************************************************/

#ifndef CONFIG_LCD_NOGETRUN
static int lcd_getrun(FAR struct lcd_dev_s *dev, fb_coord_t row, fb_coord_t col, FAR uint8_t *buffer, size_t npixels)
{
	return OK;
}
#endif

/****************************************************************************
 * Name:  lcd_getvideoinfo
 *
 * Description:
 *   Get information about the LCD video controller configuration.
 *
 ****************************************************************************/

static int lcd_getvideoinfo(FAR struct lcd_dev_s *dev, FAR struct fb_videoinfo_s *vinfo)
{
	DEBUGASSERT(dev && vinfo);
	//vinfo->fmt = st7785_COLORFMT; /* Color format: RGB16-565: RRRR RGGG GGGB BBBB */
	vinfo->xres = LCD_XRES;	/* Horizontal resolution in pixel columns */
	vinfo->yres = LCD_YRES;	/* Vertical resolution in pixel rows */
	vinfo->nplanes = MAX_NO_PLANES;	/* Number of color planes supported */
	return OK;
}

/****************************************************************************
 * Name:  lcd_getplaneinfo
 *
 * Description:
 *   Get information about the configuration of each LCD color plane.
 *
 ****************************************************************************/

static int lcd_getplaneinfo(FAR struct lcd_dev_s *dev, unsigned int planeno, FAR struct lcd_planeinfo_s *pinfo)
{
	DEBUGASSERT(dev && pinfo && planeno == 0);
	pinfo->putrun = &lcd_putrun;	/* Put a run into LCD memory */
	pinfo->putarea = &lcd_putarea;	/* Put an area into LCD */
#ifndef CONFIG_LCD_NOGETRUN
	pinfo->getrun = &lcd_getrun;	/* Get a run from LCD memory */
#endif
	return OK;
}

/****************************************************************************
 * Name:  lcd_getpower
 ****************************************************************************/

static int lcd_getpower(FAR struct lcd_dev_s *dev, int color)
{
	return OK;
}

/****************************************************************************
 * Name:  lcd_setpower
 ****************************************************************************/

static int lcd_setpower(FAR struct lcd_dev_s *dev, int power)
{
	FAR struct mipi_lcd_dev_s *priv = (FAR struct mipi_lcd_dev_s *)dev;
	if (power > CONFIG_LCD_MAXPOWER) {
		lcddbg("Power exceeds CONFIG_LCD_MAXPOWER %d", CONFIG_LCD_MAXPOWER);
		return -1;
	}
	if (power == 0) {
		lcm_setting_table_t display_off_cmd = {0x28, 0, {0x00}};
		send_cmd(priv, display_off_cmd);
	} else {
		lcm_setting_table_t display_on_cmd = {0x29, 0, {0x00}};
		send_cmd(priv, display_on_cmd);
	}
	priv->config->backlight(power);
	priv->power = power;
	return OK;
}

/****************************************************************************
 * Name:  lcd_init
 *
 * Description:
 *   init the lcd
 *
 ****************************************************************************/

static int lcd_init(FAR struct lcd_dev_s *dev)
{
	FAR struct mipi_lcd_dev_s *priv = (FAR struct mipi_lcd_dev_s *)dev;
	priv->config->reset();
	if (send_init_cmd(priv, lcd_init_cmd_g) == OK) {
                lcdvdbg("LCD Init sequence completed\n");
        } else {
                lcddbg("ERROR: LCD Init sequence failed\n");
        }
	priv->config->init();
	priv->config->lcd_enable();
	return OK;
}

/****************************************************************************
 * Name:  lcd_setorientation
 *
 * Description:
 *   Set LCD orientation(PORTRAIT/LANDSCAPE).
 *
 ****************************************************************************/

static int lcd_setorientation(FAR struct lcd_dev_s *dev, unsigned int orientation)
{
	return OK;
}

/****************************************************************************
 * Name:  lcd_getcontrast
 *
 * Description:
 *   Get the current contrast setting (0-CONFIG_LCD_MAXCONTRAST).
 *
 ****************************************************************************/

static int lcd_getcontrast(FAR struct lcd_dev_s *dev)
{
	return OK;
}

/****************************************************************************
 * Name:  lcd_setcontrast
 *
 * Description:
 *   Set LCD panel contrast (0-CONFIG_LCD_MAXCONTRAST).
 *
 ****************************************************************************/

static int lcd_setcontrast(FAR struct lcd_dev_s *dev, unsigned int contrast)
{
	return OK;
}

FAR struct lcd_dev_s *mipi_lcdinitialize(FAR struct mipi_dsi_device *dsi, struct mipi_lcd_config_s *config)
{
	FAR struct mipi_lcd_dev_s *priv = &g_lcdcdev;
	priv->dev.getplaneinfo = lcd_getplaneinfo;
	priv->dev.getvideoinfo = lcd_getvideoinfo;
	priv->dev.getpower = lcd_getpower;
	priv->dev.setpower = lcd_setpower;
	priv->dev.getcontrast = lcd_getcontrast;
	priv->dev.setcontrast = lcd_setcontrast;
	priv->dev.init = lcd_init;
	priv->dsi_dev = dsi;
	priv->config = config;
	//priv->config->reset();
	if (send_init_cmd(priv, lcd_init_cmd_g) == OK) {
		lcdvdbg("LCD Init sequence completed\n");
	} else {
		lcddbg("ERROR: LCD Init sequence failed\n");
	}
	priv->config->lcd_enable();
	priv->config->backlight(CONFIG_LCD_MAXPOWER);
	return &priv->dev;
}
