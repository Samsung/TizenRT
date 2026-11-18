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
#include <tinyara/lcd/mipi_lcd.h>
#include <tinyara/mipidsi/mipi_dsi.h>
#include <tinyara/mipidsi/mipi_display.h>
#include <debug.h>
#include <assert.h>
#include <errno.h>
#include <semaphore.h>
#include <math.h>
#include <stdio.h>

#ifdef CONFIG_LCD_SPLASH_IMAGE
#include "image_lib.h"
#endif

#define LCD_XRES CONFIG_LCD_XRES
#define LCD_YRES CONFIG_LCD_YRES

#define REGFLAG_DELAY                       0xFC
#define REGFLAG_END_OF_TABLE                0xFD	// END OF REGISTERS MARKER

#if !defined(CONFIG_LCD_MAXPOWER) || CONFIG_LCD_MAXPOWER < 1
#define CONFIG_LCD_MAXPOWER 100
#endif

/* MIPI State Machine Definitions */
typedef enum {
	MIPI_STATE_CMD_OFF,     // LCD power off, Display off, Backlight 0, CMD mode
	MIPI_STATE_CMD_ON,      // LCD power on, Display on, Backlight >= 0, CMD mode 
	MIPI_STATE_VIDEO_ON     // LCD power on, Display on, Backlight >= 0, VIDEO mode
} mipi_state_t;

#if !defined(CONFIG_LCD_SEND_CMD_RETRY_COUNT)
#define CONFIG_LCD_SEND_CMD_RETRY_COUNT 0
#endif

#if !defined(CONFIG_LCD_SEND_VENDOR_INIT_CMD_RETRY_COUNT)
#define CONFIG_LCD_SEND_VENDOR_INIT_CMD_RETRY_COUNT 3
#endif

struct mipi_lcd_dev_s {
	/* Publicly visible device structure */
	struct lcd_dev_s dev;
	FAR struct mipi_dsi_device *dsi_dev;
	u8 *lcd_img_buffer[MAX_NO_PLANES];
	//u8 *BackupLcdImgBuffer;
	int fb_alloc_count;
	uint8_t power;
	mipi_state_t mipi_state;
	sem_t sem;
	struct mipi_lcd_config_s *config;
};

static struct mipi_lcd_dev_s g_lcdcdev;

extern int check_lcd_vendor_send_init_cmd(struct mipi_lcd_dev_s *priv);
extern int get_lcdinfo(FAR struct lcd_info_s *lcdinfo);

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
	msg.rx_buf = NULL;
	msg.rx_len = 0;
	msg.flags = 0;
	transfer_status = send_to_mipi_dsi(priv, &msg);

	if (transfer_status != OK) {
		lcddbg("Command 0x%x not sent\n", cmd);
	}

	return transfer_status;
}

/* rx_len is the maximum return packet size*/
int set_return_packet_len(struct mipi_lcd_dev_s *priv, u8 rx_len)
{
	int transfer_status = OK;
	struct mipi_dsi_msg msg;
	msg.channel = rx_len;
	msg.type = MIPI_DSI_SET_MAXIMUM_RETURN_PACKET_SIZE;
	msg.tx_buf = NULL;
	msg.tx_len = 0;
	msg.rx_buf = NULL;
	msg.rx_len = 0;
	msg.flags = 0;
	transfer_status = send_to_mipi_dsi(priv, &msg);

	if (transfer_status != OK) {
		lcddbg("Rxlen 0x%x not sent\n", rx_len);
	}
	return transfer_status;
}

int read_response(struct mipi_lcd_dev_s *priv, lcm_setting_table_t command, u8 *rxbuf, u8 rx_len)
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

int send_init_cmd(struct mipi_lcd_dev_s *priv, lcm_setting_table_t *table)
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
			msg.rx_buf = NULL;
			msg.rx_len = 0;
			msg.flags = 0;
			transfer_status = send_to_mipi_dsi(priv, &msg);
			if (transfer_status != OK) {
				return transfer_status;
			}
		}
		send_cmd_idx_s++;
	}
}

static mipi_state_t get_mipi_state(struct mipi_lcd_dev_s *priv)
{
	return priv->mipi_state;
}

static int set_mipi_state(struct mipi_lcd_dev_s *priv, mipi_state_t new_state)
{
	int retries = CONFIG_LCD_SEND_VENDOR_INIT_CMD_RETRY_COUNT;
	mipi_state_t old_state = get_mipi_state(priv);
	
	if (old_state == new_state) {
		lcddbg("Already in requested state\n");
		return OK;
	}

	switch(new_state) {
	case MIPI_STATE_CMD_OFF:
		priv->config->mipi_mode_switch(CMD_MODE);
		mipi_dsi_dcs_set_display_off(priv->dsi_dev);
		lcddbg("Powering down the LCD\n");
		priv->config->power_off();
		break;
	case MIPI_STATE_CMD_ON:
		if (old_state == MIPI_STATE_VIDEO_ON) {
			lcddbg("Switching to CMD mode\n");
			priv->config->mipi_mode_switch(CMD_MODE);
		} else {
			lcddbg("Powering up the LCD\n");
			priv->config->power_on();

			/* We need to send init cmd after LCD IC power on */
			while (retries) {
				if (check_lcd_vendor_send_init_cmd(priv) != OK) {
					/* attempt a full reset of lcd + driver stack */
					priv->config->reset();
					priv->config->power_on();
					priv->config->mipi_drv_reset();
				} else {
					/* panel accepted the command, normal operation */
					break;
				}
				retries--;
			}

			if (retries <= 0) {
				lcddbg("ERROR: LCD Init sequence failed\n");
				return ERROR;
			}
		}
		break;
	case MIPI_STATE_VIDEO_ON:
		if (old_state == MIPI_STATE_CMD_OFF) {
			if (set_mipi_state(priv, MIPI_STATE_CMD_ON) != OK) {
				lcddbg("ERROR: Failed to power up LCD for video mode\n");
				return ERROR;
			}
		}
		lcddbg("Switching to Video mode\n");
		priv->config->mipi_mode_switch(VIDEO_MODE);
		break;
	default:
		lcddbg("Wrong state requested\n");
		return ERROR;
	}

	priv->mipi_state = new_state;

	return OK;
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
		lcddbg("ERROR: LCD is off. putarea failed\n");
		sem_post(&priv->sem);
		return -EIO;
	}
	//coordinate shift from (0,0) -> (1,1) and (XRES-1,YRES-1) -> (XRES,YRES)
	row_start += 1;
	row_end += 1;
	col_start += 1;
	col_end += 1;
	priv->config->lcd_put_area((u8 *)buffer, row_start, col_start, row_end, col_end);
	if (get_mipi_state(priv) == MIPI_STATE_CMD_ON) {
		set_mipi_state(priv, MIPI_STATE_VIDEO_ON);
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
	DEBUGASSERT(dev);
	if (!vinfo) {
		return -EINVAL;
	}
	//vinfo->fmt = st7785_COLORFMT; /* Color format: RGB16-565: RRRR RGGG GGGB BBBB */
	vinfo->xres = LCD_XRES;	/* Horizontal resolution in pixel columns */
	vinfo->yres = LCD_YRES;	/* Vertical resolution in pixel rows */
	vinfo->nplanes = MAX_NO_PLANES;	/* Number of color planes supported */
	return OK;
}

/****************************************************************************
 * Name:  lcd_getlcdinfo
 *
 * Description:
 *   Get information about the Lcd such as size, height, width, etc.
 *
 ****************************************************************************/

static int lcd_getlcdinfo(FAR struct lcd_dev_s *dev, FAR struct lcd_info_s *lcdinfo)
{
	int ret;
	DEBUGASSERT(dev);
	if (!lcdinfo) {
		return -EINVAL;
	}
	ret = get_lcdinfo(lcdinfo);	// Fill lcdinfo with vendor specific values
	if (ret != OK) {
		return ret;
	}
	lcdinfo->lcd_size_inch = sqrtf(lcdinfo->lcd_height_mm * lcdinfo->lcd_height_mm + lcdinfo->lcd_width_mm * lcdinfo->lcd_width_mm) / 25.4f;
	lcdinfo->lcd_dpi = sqrtf(LCD_XRES * LCD_XRES + LCD_YRES * LCD_YRES) / lcdinfo->lcd_size_inch;
	return ret;
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
	DEBUGASSERT(dev);
	if (!pinfo || planeno != 0) {
		return -EINVAL;
	}
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
	DEBUGASSERT(dev);
	FAR struct mipi_lcd_dev_s *priv = (FAR struct mipi_lcd_dev_s *)dev;
	return priv->power;
}

/****************************************************************************
 * Name:  lcd_power_off
 *
 * Description:
 *   Power off the LCD.
 * 	 Switch to command mode and display off, power off the LCD.
 *
 * Returns:
 * 	 On success, OK(O). Otherwise, a negative value.
 *
 ****************************************************************************/

static int lcd_power_off(FAR struct mipi_lcd_dev_s *priv)
{
	if (get_mipi_state(priv) == MIPI_STATE_CMD_OFF) {
		lcddbg("LCD is already OFF\n");
		return OK;
	}

	return set_mipi_state(priv, MIPI_STATE_CMD_OFF);
}

/****************************************************************************
 * Name:  lcd_power_on
 *
 * Description:
 *   Power on the LCD and send init command.
 *
 * Assumption:
 *   This function is called when LCD is OFF.
 * 	 So we can safely assume that LCD is in command mode.
 *
 * Returns:
 * 	 On success, OK(O). Otherwise, a negative value.
 *
 ****************************************************************************/

static int lcd_power_on(FAR struct mipi_lcd_dev_s *priv)
{
	if (get_mipi_state(priv) != MIPI_STATE_CMD_OFF) {
		lcddbg("LCD is already ON\n");
		return OK;
	}

	return set_mipi_state(priv, MIPI_STATE_CMD_ON);
}


/****************************************************************************
 * Name:  lcd_setpower
 ****************************************************************************/

static int lcd_setpower(FAR struct lcd_dev_s *dev, int power)
{
	DEBUGASSERT(dev);
	int ret;
	FAR struct mipi_lcd_dev_s *priv = (FAR struct mipi_lcd_dev_s *)dev;

	lcddbg("Changing lcd backlight to %d requested\n", power);
	if (power > CONFIG_LCD_MAXPOWER) {
		lcddbg("Power exceeds CONFIG_LCD_MAXPOWER %d\n", power);
		return -EINVAL;
	}

	while (sem_wait(&priv->sem) != OK) {
		ASSERT(get_errno() == EINTR);
	}

	if (power == priv->power) {
		lcddbg("Already in the requested power state(%d)\n", power);
		sem_post(&priv->sem);
		return OK;
	}

	if (power == 0) {
		priv->config->backlight(power);
		ret = lcd_power_off(priv);
		if (ret != OK) {
			lcddbg("ERROR: LCD power off failed ret : %d\n", ret);
			sem_post(&priv->sem);
			return ret;
		}
	} else {
		/* The power on must operate only when LCD is OFF */
		if (priv->power == 0) {
			/* The power on must operate only when LCD is OFF */
			ret = lcd_power_on(priv);
			if (ret != OK) {
				lcddbg("ERROR: LCD power on failed ret : %d\n", ret);
				sem_post(&priv->sem);
				return ret;
			}
		}
		priv->config->backlight(power);
	}

	priv->power = power;
	sem_post(&priv->sem);
	lcddbg("Changed lcd backlight to %d\n", priv->power);
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
	DEBUGASSERT(dev);
	int ret;
	FAR struct mipi_lcd_dev_s *priv = (FAR struct mipi_lcd_dev_s *)dev;
	
	priv->config->reset();
	ret = set_mipi_state(priv, MIPI_STATE_CMD_OFF);
	if (ret != OK) {
		lcddbg("ERROR: LCD Init sequence failed\n");
		return ret;
	}

	while (sem_wait(&priv->sem) != OK) {
		DEBUGASSERT(get_errno() == EINTR);
	}

	ret = lcd_power_on(priv);
	if (ret != OK) {
		lcddbg("Failed to turn on the LCD\n");
		sem_post(&priv->sem);
		return ret;
	}
	priv->config->init();
	priv->config->lcd_enable();
	sem_post(&priv->sem);
	
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
	priv->dev.getlcdinfo = lcd_getlcdinfo;
	priv->dev.getpower = lcd_getpower;
	priv->dev.setpower = lcd_setpower;
	priv->dev.getcontrast = lcd_getcontrast;
	priv->dev.setcontrast = lcd_setcontrast;
	priv->dev.init = lcd_init;
	priv->dsi_dev = dsi;
	priv->config = config;
	priv->power = 0;
	priv->mipi_state = MIPI_STATE_CMD_OFF;

	sem_init(&priv->sem, 0 , 1);
	return &priv->dev;
}
