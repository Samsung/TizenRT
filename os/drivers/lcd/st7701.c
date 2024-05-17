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
#include <tinyara/lcd/st7701.h>
#include <tinyara/mipidsi/mipi_dsi.h>
#include <tinyara/mipidsi/mipi_display.h>
#include <debug.h>
#include <assert.h>

#define REGFLAG_DELAY                       0xFC
#define REGFLAG_END_OF_TABLE                0xFD	// END OF REGISTERS MARKER

struct st7701_dev_s {
	/* Publicly visible device structure */

	struct lcd_dev_s dev;

	FAR struct mipi_dsi_device *dsi_dev;

	u8 *lcd_img_buffer[MAX_NO_PLANES];
	u8 *BackupLcdImgBuffer;
	int fb_alloc_count;

	struct st7701_config_s *config;
};

static struct st7701_dev_s g_lcdcdev;

typedef struct lcm_setting_table {
	u8 cmd;
	u8 count;
	u8 para_list[16];
} lcm_setting_table_t;

static const lcm_setting_table_t ST7701S_init_cmd_g[] = {	/* DCS Write Long */
	/* ST7701S Reset Sequence */
	/* LCD_Nreset (1); Delayms (1); */
	/* LCD_Nreset (0); Delayms (1); */
	/* LCD_Nreset (1); Delayms (120); */
	{0x11, 0, {0x00}},
	{REGFLAG_DELAY, 120, {}},	/* Delayms (120) */

	/* Bank0 Setting */
	/* Display Control setting */
	{0xFF, 5, {0x77, 0x01, 0x00, 0x00, 0x10}},
	{0xC0, 2, {0x63, 0x00}},
	{0xC1, 2, {0x0C, 0x02}},
	{0xC2, 2, {0x31, 0x08}},
	{0xCC, 1, {0x10}},

	/* Gamma Cluster Setting */
	{0xB0, 16, {0x00, 0x11, 0x19, 0x0C, 0x10, 0x06, 0x07, 0x0A, 0x09, 0x22, 0x04, 0x10, 0x0E, 0x28, 0x30, 0x1C}},
	{0xB1, 16, {0x00, 0x12, 0x19, 0x0D, 0x10, 0x04, 0x06, 0x07, 0x08, 0x23, 0x04, 0x12, 0x11, 0x28, 0x30, 0x1C}},

	/* End Gamma Setting */
	/* End Display Control setting */
	/* End Bank0 Setting */

	/* Bank1 Setting */
	/* Power Control Registers Initial */
	{0xFF, 5, {0x77, 0x01, 0x00, 0x00, 0x11}},
	{0xB0, 1, {0x4D}},

	/* Vcom Setting */
	{0xB1, 1, {0x3E}},			/* VCOM */
	/* End End Vcom Setting */

	{0xB2, 1, {0x09}},			/* 07 */
	{0xB3, 1, {0x80}},
	{0xB5, 1, {0x4B}},			/* 47 4A */
	{0xB7, 1, {0x85}},
	{0xB8, 1, {0x21}},
	{0xB9, 1, {0x10}},
	{0xC1, 1, {0x78}},
	{0xC2, 1, {0x78}},
	{0xD0, 1, {0x88}},
	/* End Power Control Registers Initial */
	{REGFLAG_DELAY, 100, {}},	/* Delayms (100) */
	/* GIP Setting */
	{0xE0, 3, {0x00, 0x00, 0x02}},
	{0xE1, 11, {0x04, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x20, 0x20}},
	{0xE2, 13, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
	{0xE3, 4, {0x00, 0x00, 0x33, 0x33}},
	{0xE4, 2, {0x22, 0x00}},
	{0xE5, 16, {0x04, 0x34, 0xAA, 0xAA, 0x06, 0x34, 0xAA, 0xAA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
	{0xE6, 4, {0x00, 0x00, 0x33, 0x00}},
	{0xE7, 2, {0x22, 0x00}},
	{0xE8, 16, {0x05, 0x34, 0xAA, 0xAA, 0x07, 0x34, 0xAA, 0xAA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
	//{0xEA, 16, {0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00}},
	{0xEB, 7, {0x02, 0x00, 0x40, 0x40, 0x00, 0x00, 0x00}},
	{0xEC, 2, {0x00, 0x00}},
	{0xED, 16, {0xFA, 0x45, 0x0B, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xB0, 0x54, 0xAF}},
	// {0xEF, 12, {0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04}},
	{0xEF, 6, {0x08, 0x08, 0x08, 0x44, 0x3F, 0x54}},
	/* End GIP Setting */

	{0xFF, 5, {0x77, 0x01, 0x00, 0x00, 0x00}},
	{0x29, 0, {0x00}},
	{REGFLAG_END_OF_TABLE, 0x00, {}},
};

static int st7701_send_init_cmd(struct mipi_dsi_device *device, lcm_setting_table_t *table)
{
	static u8 send_cmd_idx_s = 0;
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
			return mipi_dsi_transfer(device, &msg);
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
			transfer_status = mipi_dsi_transfer(device, &msg);
			if (transfer_status != OK) {
				return transfer_status;
			}
		}

		send_cmd_idx_s++;
	}
}

/* LCD Data Transfer Methods */

/* LCD Specific Controls */

/****************************************************************************
 * Name:  st7701_putrun
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

static int st7701_putrun(FAR struct lcd_dev_s *dev, fb_coord_t row, fb_coord_t col, FAR const uint8_t *buffer, size_t npixels)
{
	return OK;
}

/****************************************************************************
 * Name:  st7701_putarea
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

static int st7701_putarea(FAR struct lcd_dev_s *dev, fb_coord_t row_start, fb_coord_t row_end, fb_coord_t col_start, fb_coord_t col_end, FAR const uint8_t *buffer, fb_coord_t stride)
{
	FAR struct st7701_dev_s *priv = (FAR struct st7701_dev_s *)dev;
	//coordinate shift from (0,0) -> (1,1) and (XRES-1,YRES-1) -> (XRES,YRES)
	row_start += 1;
	row_end += 1;
	col_start += 1;
	col_end += 1;
	priv->config->lcd_put_area(buffer, row_start, col_start, row_end, col_end);
	return OK;
}

/****************************************************************************
 * Name:  st7701_getrun
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
static int st7701_getrun(FAR struct lcd_dev_s *dev, fb_coord_t row, fb_coord_t col, FAR uint8_t *buffer, size_t npixels)
{
	return OK;
}
#endif

/****************************************************************************
 * Name:  st7701_getvideoinfo
 *
 * Description:
 *   Get information about the LCD video controller configuration.
 *
 ****************************************************************************/

static int st7701_getvideoinfo(FAR struct lcd_dev_s *dev, FAR struct fb_videoinfo_s *vinfo)
{
	DEBUGASSERT(dev && vinfo);
	//vinfo->fmt = ST7701_COLORFMT; /* Color format: RGB16-565: RRRR RGGG GGGB BBBB */
	vinfo->xres = XRES;			/* Horizontal resolution in pixel columns */
	vinfo->yres = YRES;			/* Vertical resolution in pixel rows */
	vinfo->nplanes = MAX_NO_PLANES;	/* Number of color planes supported */
	return OK;
}

/****************************************************************************
 * Name:  st7701_getplaneinfo
 *
 * Description:
 *   Get information about the configuration of each LCD color plane.
 *
 ****************************************************************************/

static int st7701_getplaneinfo(FAR struct lcd_dev_s *dev, unsigned int planeno, FAR struct lcd_planeinfo_s *pinfo)
{
	DEBUGASSERT(dev && pinfo && planeno == 0);
	pinfo->putrun = &st7701_putrun;	/* Put a run into LCD memory */
	pinfo->putarea = &st7701_putarea;	/* Put an area into LCD */
#ifndef CONFIG_LCD_NOGETRUN
	pinfo->getrun = &st7701_getrun;	/* Get a run from LCD memory */
#endif
	return OK;
}

/****************************************************************************
 * Name:  st7701_getpower
 ****************************************************************************/

static int st7701_getpower(FAR struct lcd_dev_s *dev, int color)
{
	return OK;
}

/****************************************************************************
 * Name:  st7701_setpower
 ****************************************************************************/

static int st7701_setpower(FAR struct lcd_dev_s *dev, int power)
{
	return OK;
}

/****************************************************************************
 * Name:  st7701_init
 *
 * Description:
 *   init the lcd
 *
 ****************************************************************************/

static int st7701_init(FAR struct lcd_dev_s *dev)
{
	FAR struct st7701_dev_s *priv = (FAR struct st7701_dev_s *)dev;
	priv->config->init();
	priv->config->lcd_enable();
	return OK;
}

/****************************************************************************
 * Name:  st7701_setorientation
 *
 * Description:
 *   Set LCD orientation(PORTRAIT/LANDSCAPE).
 *
 ****************************************************************************/

static int st7701_setorientation(FAR struct lcd_dev_s *dev, unsigned int orientation)
{
	return OK;
}

/****************************************************************************
 * Name:  st7701_getcontrast
 *
 * Description:
 *   Get the current contrast setting (0-CONFIG_LCD_MAXCONTRAST).
 *
 ****************************************************************************/

static int st7701_getcontrast(FAR struct lcd_dev_s *dev)
{
	return OK;
}

/****************************************************************************
 * Name:  st7701_setcontrast
 *
 * Description:
 *   Set LCD panel contrast (0-CONFIG_LCD_MAXCONTRAST).
 *
 ****************************************************************************/

static int st7701_setcontrast(FAR struct lcd_dev_s *dev, unsigned int contrast)
{
	return OK;
}

FAR struct lcd_dev_s *st7701_lcdinitialize(FAR struct mipi_dsi_device *dsi, struct st7701_config_s *config)
{
	FAR struct st7701_dev_s *priv = &g_lcdcdev;
	priv->dev.getplaneinfo = st7701_getplaneinfo;
	priv->dev.getvideoinfo = st7701_getvideoinfo;
	priv->dev.getpower = st7701_getpower;
	priv->dev.setpower = st7701_setpower;
	priv->dev.getcontrast = st7701_getcontrast;
	priv->dev.setcontrast = st7701_setcontrast;
	priv->dev.init = st7701_init;
	priv->dsi_dev = dsi;
	priv->config = config;
	if (st7701_send_init_cmd(priv->dsi_dev, ST7701S_init_cmd_g) == OK) {
		lcdvdbg("LCD Init sequence completed\n");
	} else {
		lcddbg("ERROR: LCD Init sequence failed\n");
	}
	return &priv->dev;
}
