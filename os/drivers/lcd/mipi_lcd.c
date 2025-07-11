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
#elif defined(CONFIG_LCD_ST7701SN)
#include <tinyara/lcd/st7701sn.h>
#endif
#include <tinyara/mipidsi/mipi_dsi.h>
#include <tinyara/mipidsi/mipi_display.h>
#include "lcd_logo.h"
#include <debug.h>
#include <assert.h>
#include <errno.h>
#include <semaphore.h>

#define REGFLAG_DELAY                       0xFC
#define REGFLAG_END_OF_TABLE                0xFD	// END OF REGISTERS MARKER

#if !defined(CONFIG_LCD_MAXPOWER) || CONFIG_LCD_MAXPOWER < 1
#define CONFIG_LCD_MAXPOWER 100
#endif

enum lcd_mode_e {
	LCD_ON = 0,
	LCD_OFF = 1
};

typedef enum lcd_mode_e lcd_mode_t;

#if !defined(CONFIG_LCD_SEND_CMD_RETRY_COUNT)
#define CONFIG_LCD_SEND_CMD_RETRY_COUNT 0
#endif

extern const uint8_t lcd_logo_raw_data[]; // Buffer containing only logo
static uint8_t *lcd_init_fullscreen_image = NULL; // Buffer containing full screen data with logo on specific position

#if defined(CONFIG_LCD_SW_ROTATION)
#define NUM_OF_LCD_BUFFER	2
static uint8_t *lcd_buffer[NUM_OF_LCD_BUFFER] = { NULL, NULL };	//Two lcd buffers to avoid screen tearing
static int lcd_buffer_index = 0;
static void lcd_rotate_buffer(short int* src, short int* dst)
{
	int row;
	int col;
	int dst_inc = 2 * CONFIG_LCD_XRES;
	short int val0;
	short int val1;
	short int val2;
	short int val3;
	short int *psrc;
	short int *pdst;

#if defined(CONFIG_LCD_LANDSCAPE)
	for (row = 0; row < CONFIG_LCD_XRES; row += 2) {
		psrc = src + row * CONFIG_LCD_YRES;
		pdst = dst + CONFIG_LCD_XRES - row - 2;
		for (col = 0; col < CONFIG_LCD_YRES; col += 2) {
			val0 = *(psrc + 0);
			val1 = *(psrc + 1);
			val2 = *(psrc + CONFIG_LCD_YRES + 0);
			val3 = *(psrc + CONFIG_LCD_YRES + 1);
			psrc += 2;
			*(pdst + 0) = val2;
			*(pdst + 1) = val0;
			*(pdst + CONFIG_LCD_XRES) = val3;
			*(pdst + CONFIG_LCD_XRES + 1) = val1;
			pdst += dst_inc;
		}
	}
#elif defined(CONFIG_LCD_RLANDSCAPE)
	for (row = 0; row < CONFIG_LCD_XRES; row += 2) {
		psrc = src + row * CONFIG_LCD_YRES;
		pdst = dst + row + (CONFIG_LCD_YRES - 1) * CONFIG_LCD_XRES;
		for (col = 0; col < CONFIG_LCD_YRES; col += 2) {
			val0 = *(psrc + 0);
			val1 = *(psrc + 1);
			val2 = *(psrc + CONFIG_LCD_YRES + 0);
			val3 = *(psrc + CONFIG_LCD_YRES + 1);
			psrc += 2;
			*(pdst + 0) = val0;
			*(pdst + 1) = val2;
			*(pdst - CONFIG_LCD_XRES) = val1;
			*(pdst - CONFIG_LCD_XRES + 1) = val3;
			pdst -= dst_inc;
		}
	}
#else
	#error LCD Screen Rotation support only available from PORTRAIT to LANDSCAPE AND RLANDSCAPE
#endif
}
#endif

struct mipi_lcd_dev_s {
	/* Publicly visible device structure */

	struct lcd_dev_s dev;

	FAR struct mipi_dsi_device *dsi_dev;

	u8 *lcd_img_buffer[MAX_NO_PLANES];
	//u8 *BackupLcdImgBuffer;
	int fb_alloc_count;
	uint8_t power;
	lcd_mode_t lcdonoff;
	sem_t sem;
	struct mipi_lcd_config_s *config;
};

static struct mipi_lcd_dev_s g_lcdcdev;

static int send_to_mipi_dsi(struct mipi_lcd_dev_s *priv, struct mipi_dsi_msg* msg)
{
	int transfer_status = ERROR;
	int max_cmd_retry_count = CONFIG_LCD_SEND_CMD_RETRY_COUNT;
	transfer_status = mipi_dsi_transfer(priv->dsi_dev, msg);

	/* Retry Case */
	while (max_cmd_retry_count && transfer_status != OK) {
		transfer_status = mipi_dsi_transfer(priv->dsi_dev, msg);
		max_cmd_retry_count--;
	}

	return transfer_status;
}

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
	transfer_status = send_to_mipi_dsi(priv, &msg);

	if (transfer_status != OK) {
		lcddbg("Command %x not sent\n", cmd);
	}

	return transfer_status;
}
/* rx_len is the maximum return packet size*/
static int set_return_packet_len(struct mipi_lcd_dev_s *priv, u8 rx_len)
{
	int transfer_status = OK;
	struct mipi_dsi_msg msg;
	msg.channel = rx_len;
	msg.type = MIPI_DSI_SET_MAXIMUM_RETURN_PACKET_SIZE;
	msg.tx_len = 0;
	msg.flags = 0;
	transfer_status = send_to_mipi_dsi(priv, &msg);

	if (transfer_status != OK) {
		lcddbg("Command %x not sent\n", rx_len);
	}
	return transfer_status;
}

static int read_response(struct mipi_lcd_dev_s *priv, lcm_setting_table_t command, u8 *rxbuf, u8 rx_len)
{
	int transfer_status = OK;
	u8 cmd = command.cmd;
	u8 *cmd_addr = command.para_list;
	u32 payload_len = command.count;
	struct mipi_dsi_msg msg;
	msg.channel = cmd;
	if (payload_len == 0) {
		msg.type = MIPI_DSI_GENERIC_READ_0_PARAM;
	} else if (payload_len == 1) {
		msg.type = MIPI_DSI_GENERIC_READ_1_PARAM;
	} else if (payload_len == 2) {
		msg.type = MIPI_DSI_GENERIC_READ_2_PARAM;
	}
	msg.tx_buf = cmd_addr;
	msg.tx_len = payload_len;
	msg.flags = 0;
	msg.rx_buf = rxbuf;
	msg.rx_len = rx_len;
	transfer_status = send_to_mipi_dsi(priv, &msg);

	if (transfer_status != OK) {
		lcddbg("Command %x not sent\n", cmd);
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
			return send_to_mipi_dsi(priv, &msg);
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
			transfer_status = send_to_mipi_dsi(priv, &msg);
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
	while (sem_wait(&priv->sem) != OK) {
		ASSERT(get_errno() == EINTR);
	}
	if (priv->power == 0) {
		sem_post(&priv->sem);
		return ERROR;
	}
	//coordinate shift from (0,0) -> (1,1) and (XRES-1,YRES-1) -> (XRES,YRES)
	row_start += 1;
	row_end += 1;
	col_start += 1;
	col_end += 1;
#if defined(CONFIG_LCD_SW_ROTATION)
	lcd_rotate_buffer((uint8_t *)buffer, lcd_buffer[lcd_buffer_index]);
	priv->config->lcd_put_area((u8 *)lcd_buffer[lcd_buffer_index], row_start, col_start, row_end, col_end);
	lcd_buffer_index = (1 - lcd_buffer_index);
#else
	if (lcd_init_fullscreen_image != NULL) {
		kmm_free(lcd_init_fullscreen_image);
		lcd_init_fullscreen_image = NULL;
	}
	priv->config->lcd_put_area((u8 *)buffer, row_start, col_start, row_end, col_end);
#endif
	if (priv->lcdonoff == LCD_OFF) {
		lcddbg("Switch to VIDEO mode\n");
		priv->config->mipi_mode_switch(VIDEO_MODE);
		priv->lcdonoff = LCD_ON;
	}
	sem_post(&priv->sem);
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
#if defined(CONFIG_LCD_SW_ROTATION)
	vinfo->xres = LCD_YRES;	/* Horizontal resolution in pixel columns */
	vinfo->yres = LCD_XRES;	/* Vertical resolution in pixel rows */
#else
	vinfo->xres = LCD_XRES;	/* Horizontal resolution in pixel columns */
	vinfo->yres = LCD_YRES;	/* Vertical resolution in pixel rows */
	vinfo->nplanes = MAX_NO_PLANES;	/* Number of color planes supported */
#endif
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
	pinfo->putrun = (struct lcd_planeinfo_s *)&lcd_putrun;	/* Put a run into LCD memory */
	pinfo->putarea = (struct lcd_planeinfo_s *)&lcd_putarea;	/* Put an area into LCD */
#ifndef CONFIG_LCD_NOGETRUN
	pinfo->getrun = (struct lcd_planeinfo_s *)&lcd_getrun;	/* Get a run from LCD memory */
#endif
	return OK;
}

/****************************************************************************
 * Name:  lcd_getpower
 ****************************************************************************/

static int lcd_getpower(FAR struct lcd_dev_s *dev)
{
	FAR struct mipi_lcd_dev_s *priv = (FAR struct mipi_lcd_dev_s *)dev;
	return priv->power;
}

/****************************************************************************
 * Name:  lcd_setpower
 ****************************************************************************/

static int lcd_setpower(FAR struct lcd_dev_s *dev, int power)
{
	FAR struct mipi_lcd_dev_s *priv = (FAR struct mipi_lcd_dev_s *)dev;
	if (power > CONFIG_LCD_MAXPOWER) {
		lcddbg("Power exceeds CONFIG_LCD_MAXPOWER %d\n", power);
		return -EINVAL;
	}

	while (sem_wait(&priv->sem) != OK) {
		ASSERT(get_errno() == EINTR);
	}

	if (power == priv->power) {
		lcddbg("ERROR: Already in the requested power state(%d)\n", power);
		sem_post(&priv->sem);
		return OK;
	}

	if (power == 0) {
		lcddbg("Powering down the LCD\n");
		priv->config->backlight(power);
		lcddbg("Switch to CMD mode\n");
		priv->config->mipi_mode_switch(CMD_MODE);
		priv->lcdonoff = LCD_OFF;
		lcm_setting_table_t display_off_cmd = {0x28, 0, {0x00}};
		send_cmd(priv, display_off_cmd);
		/* The power off must operate only when LCD is ON */
		priv->config->power_off();
	} else {
		/* The power on must operate only when LCD is OFF */
		if (priv->power == 0) {
			lcddbg("Powering up the LCD\n");
			priv->config->power_on();
			/* We need to send init cmd after LCD IC power on */
			if (send_init_cmd(priv, lcd_init_cmd_g) != OK) {
				lcddbg("ERROR: LCD Init sequence failed\n");
			}
		}
		priv->config->backlight(power);
	}

	priv->power = power;
	lcddbg("Changed lcd backlight to %d\n", priv->power);
	sem_post(&priv->sem);
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

FAR void lcd_init_put_image(FAR struct lcd_dev_s *dev)
{
	int logo_arr_index = 0;
	int lcd_data_index = CONFIG_LCD_XRES * (CONFIG_LCD_YRES - LOGO_YRES) + (CONFIG_LCD_XRES - LOGO_XRES);
	int lcd_data_col_count = 0;
	FAR struct mipi_lcd_dev_s *priv = (FAR struct mipi_lcd_dev_s *)dev;

	/* Memory optimization applied using Rotation buffer
	 * If rotation is enabled, then we have two buffers allocated for rotation.
	 * During bootup, rotation buffer will not be used (No putarea call from application)
	 * Therefore, the rotation buffer can be safely used for storing logo data.
	 * 
	 * If rotation is disabled, then we need to allocate memory for full screen data
	 * and it will allocate memory to lcd_init_fullscreen_image buffer.
	 * 
	 * If rotation is enabled, then lcd_init_fullscreen_image contains pointer of rotation buffer */
#if defined(CONFIG_LCD_SW_ROTATION)
	lcd_init_fullscreen_image = lcd_buffer[lcd_buffer_index];
	lcd_buffer_index = (1 - lcd_buffer_index);
#else
	lcd_init_fullscreen_image = (uint8_t *)kmm_malloc(CONFIG_LCD_XRES * CONFIG_LCD_YRES * 2 + 1);
	if (!lcd_init_fullscreen_image) {
		lcddbg("ERROR: LCD logo data memory allocation failed\n");
		return;
	}
#endif
	/* Filling buffer with black color using memset and
	 * then filling it with logo data on specific index in while loop */
	memset(lcd_init_fullscreen_image, LCD_BLACK_VAL, CONFIG_LCD_XRES * CONFIG_LCD_YRES * 2);

#if defined(CONFIG_LCD_LOGO)
	while (logo_arr_index < (LOGO_YRES * LOGO_XRES * 2)) {
		lcd_init_fullscreen_image[lcd_data_index + 1] = lcd_logo_raw_data[logo_arr_index++];
		lcd_init_fullscreen_image[lcd_data_index] = lcd_logo_raw_data[logo_arr_index++];
		lcd_data_index += 2;
		lcd_data_col_count += 1;
		if (lcd_data_col_count == LOGO_XRES) {
			lcd_data_index += ((CONFIG_LCD_XRES - LOGO_XRES) * 2);
			lcd_data_col_count = 0;
		}
	}
#endif

	priv->config->lcd_put_area((u8 *)lcd_init_fullscreen_image, 1, 1, CONFIG_LCD_XRES, CONFIG_LCD_YRES);	// 1, 1 -> Start index of the frame buffer
	priv->config->mipi_mode_switch(VIDEO_MODE);
	priv->lcdonoff = LCD_ON;

}

FAR struct lcd_dev_s *mipi_lcdinitialize(FAR struct mipi_dsi_device *dsi, struct mipi_lcd_config_s *config)
{
	FAR struct mipi_lcd_dev_s *priv = &g_lcdcdev;
	priv->dev.getplaneinfo = lcd_getplaneinfo;
	priv->dev.getvideoinfo = lcd_getvideoinfo;
	priv->dev.getpower = (struct mipi_lcd_dev_s *)lcd_getpower;
	priv->dev.setpower = lcd_setpower;
	priv->dev.getcontrast = lcd_getcontrast;
	priv->dev.setcontrast = lcd_setcontrast;
	priv->dev.init = lcd_init;
	priv->dsi_dev = dsi;
	priv->config = config;
	priv->power = 0;
	priv->lcdonoff = LCD_OFF;

	sem_init(&priv->sem, 0 , 1);
#if defined(CONFIG_LCD_SW_ROTATION)
	uint8_t *mem = (uint8_t *)kmm_malloc((CONFIG_LCD_XRES * CONFIG_LCD_YRES * 2 + 1) * NUM_OF_LCD_BUFFER);	// each pixel is 8bit int
	if (!mem) {
		lcddbg("ERROR: LCD rotate buffer memory allocation failed\n");
		return NULL;
	} else {
		for (int i = 0; i < NUM_OF_LCD_BUFFER; i++) {
			lcd_buffer[i] = mem + i * CONFIG_LCD_XRES * CONFIG_LCD_YRES * 2;
		}
		lcdvdbg("Memory allocated for SW screen rotation, Number of buffers created %d\n", NUM_OF_LCD_BUFFER);
	}
#endif
	return &priv->dev;
}
