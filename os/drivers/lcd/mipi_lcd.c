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
#include <tinyara/silent_reboot.h>
#include "lcd_logo.h"
#include <debug.h>
#include <assert.h>
#include <errno.h>
#include <semaphore.h>
#include <math.h>
#include <stdio.h>

#define LCD_XRES CONFIG_LCD_XRES
#define LCD_YRES CONFIG_LCD_YRES

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

#if !defined(CONFIG_LCD_SEND_VENDOR_ID_CMD_RETRY_COUNT)
#define CONFIG_LCD_SEND_VENDOR_ID_CMD_RETRY_COUNT 3
#endif

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

int count_trailing_zeros(uint32_t mask)
{
	if (mask == 0)
		return 0;
	int count = 0;
	while ((mask & 1) == 0) {
		mask >>= 1;
		count++;
	}
	return count;
}

int count_set_bits(uint32_t mask)
{
	int count = 0;
	while (mask) {
		count += mask & 1;
		mask >>= 1;
	}
	return count;
}

static uint16_t convert_pixel_to_rgb565(uint16_t bmp_pixel, uint32_t r_mask, uint32_t g_mask, uint32_t b_mask)
{
	uint8_t r, g, b;
	// Extract red channel
	if (r_mask == 0) {
		r = 0;
	} else {
		uint8_t r_bits = count_set_bits(r_mask);
		uint8_t r_shift =  count_trailing_zeros(r_mask);
		r = ((bmp_pixel & r_mask) >> r_shift) << (5 - r_bits); // Scale to 5 bits
		if (r_bits > 5)
			r >>= (r_bits - 5); // Scale down if source has more bits
	}
	// Extract green channel
	if (g_mask == 0) {
		g = 0;
	} else {
		uint8_t g_bits = count_set_bits(g_mask);
		uint8_t g_shift = count_trailing_zeros(g_mask);
		g = ((bmp_pixel & g_mask) >> g_shift) << (6 - g_bits); // Scale to 6 bits
		if (g_bits > 6)
			g >>= (g_bits - 6); // Scale down if source has more bits
	}
	// Extract blue channel
	if (b_mask == 0) {
		b = 0;
	} else {
		uint8_t b_bits = count_set_bits(b_mask);
		uint8_t b_shift = count_trailing_zeros(b_mask);
		b = ((bmp_pixel & b_mask) >> b_shift) << (5 - b_bits); // Scale to 5 bits
		if (b_bits > 5)
			b >>= (b_bits - 5); // Scale down if source has more bits
	}
	return (r << 11) | (g << 5) | b;
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
#if defined(CONFIG_LCD_SW_ROTATION)
	lcd_rotate_buffer((uint8_t *)buffer, lcd_buffer[lcd_buffer_index]);
	priv->config->lcd_put_area((u8 *)lcd_buffer[lcd_buffer_index], row_start, col_start, row_end, col_end);
	lcd_buffer_index = (1 - lcd_buffer_index);
#else
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
	DEBUGASSERT(dev);
	if (!vinfo) {
		return -EINVAL;
	}
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
	int ret;

	if (priv->lcdonoff == LCD_OFF) {
		lcddbg("LCD already powered off\n");
		return OK;
	}

	lcddbg("Switch to CMD mode\n");
	priv->config->mipi_mode_switch(CMD_MODE);

	lcm_setting_table_t display_off_cmd = {0x28, 0, {0x00}};
	ret = send_cmd(priv, display_off_cmd);
	if (ret != OK) {
		lcddbg("ERROR: LCD power off failed ret : %d\n", ret);
		return ret;
	}

	priv->config->power_off();
	priv->lcdonoff = LCD_OFF;

	return OK;
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
	int retries = CONFIG_LCD_SEND_VENDOR_ID_CMD_RETRY_COUNT;

	if (priv->lcdonoff == LCD_ON) {
		lcddbg("LCD already powered on\n");
		return OK;
	}
	
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
		return -EIO;
	}
	
	return OK;
}


/****************************************************************************
 * Name:  lcd_setpower
 ****************************************************************************/

static int lcd_setpower(FAR struct lcd_dev_s *dev, int power)
{
	DEBUGASSERT(dev);
	int ret;
	FAR struct mipi_lcd_dev_s *priv = (FAR struct mipi_lcd_dev_s *)dev;
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
		lcddbg("Powering down the LCD\n");
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
	int ret;
	DEBUGASSERT(dev);
	FAR struct mipi_lcd_dev_s *priv = (FAR struct mipi_lcd_dev_s *)dev;

	priv->config->reset();

	ret = check_lcd_vendor_send_init_cmd(priv);
	if (ret != OK) {
        lcddbg("ERROR: LCD Init sequence failed: %d\n", ret);
        return ret; 
    }

	lcdvdbg("LCD Init sequence completed\n");
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

static int lcd_render_bmp(FAR struct lcd_dev_s *dev, const char *bmp_filename)
{
	int xres = LCD_XRES;
	int yres = LCD_YRES;
	uint8_t *fullscreen_buffer = NULL;
	FAR struct mipi_lcd_dev_s *priv = (FAR struct mipi_lcd_dev_s *)dev;
	FILE *bmp_file = fopen(bmp_filename, "rb");
	if (!bmp_file) {
		lcddbg("Failed to open BMP file\n");
		return -ENOENT;
	}
	bmp_header_t header;
	bmp_info_header_t info_header;
	if (fread(&header, sizeof(bmp_header_t), 1, bmp_file) != 1) {
		lcddbg("Failed to read BMP header\n");
		goto errout;
	}
	if (header.signature != 0x4D42) {
		lcddbg("Invalid BMP file: signature mismatch\n");
		goto errout;
	}
	if (fread(&info_header, sizeof(bmp_info_header_t), 1, bmp_file) != 1) {
		lcddbg("Failed to read BMP info header\n");
		goto errout;
	}

	if (info_header.bits_per_pixel != 16) {
		lcddbg("Unsupported BMP format: only 16-bit color is supported (found %d-bit)\n", info_header.bits_per_pixel);
		goto errout;
	}

	uint32_t r_mask = 0, g_mask = 0, b_mask = 0;
	if (info_header.compression == 3) { // BI_BITFIELDS
		if (fread(&r_mask, sizeof(uint32_t), 1, bmp_file) != 1 ||
			fread(&g_mask, sizeof(uint32_t), 1, bmp_file) != 1 ||
			fread(&b_mask, sizeof(uint32_t), 1, bmp_file) != 1) {
			lcddbg("Failed to read BMP bitmasks\n");
			goto errout;
		}
	} else if (info_header.compression == 0) { // BI_RGB
		r_mask = 0x00007C00;				   // 0111110000000000 (5 bits for red)
		g_mask = 0x000003E0;				   // 0000001111100000 (5 bits for green)
		b_mask = 0x0000001F;				   // 0000000000011111 (5 bits for blue)
	} else {
		lcddbg("Unsupported BMP compression: %d. Only BI_RGB (0) and BI_BITFIELDS (3) are supported for 16-bit.\n", info_header.compression);
		goto errout;
	}

	int img_width = info_header.width;
	int img_height = info_header.height;
	if (img_width > yres || img_height > xres) {
		lcddbg("Error: Image too large for screen\n");
		goto errout;
	}
	// For 16-bit, each pixel is 2 bytes. Padding is to the next 4-byte boundary.
	int padding = (4 - (img_width * 2) % 4) % 4;
#if defined(CONFIG_LCD_SW_ROTATION)
	fullscreen_buffer = lcd_buffer[lcd_buffer_index];
	lcd_buffer_index = (1 - lcd_buffer_index);
#else
	fullscreen_buffer = (uint8_t *)kmm_malloc(xres * yres * 2 + 1);
	if (!fullscreen_buffer) {
		lcddbg("Failed to allocate memory for fullscreen buffer\n");
		goto errout;
	}
#endif
	memset(fullscreen_buffer, 0, xres * yres * 2);

	// Calculate offset to center the image
	int x_offset = (xres - img_height) / 2;
	int y_offset = (yres - img_width) / 2;
	if (x_offset < 0) {
		x_offset = 0;
	}
	if (y_offset < 0) {
		y_offset = 0;
	}

	// BMP stores pixels bottom-to-top. We read row by row.
	for (int src_y = 0; src_y < img_height; src_y++) {
		int bmp_file_y = img_height - 1 - src_y;
		long file_offset_for_row = header.data_offset + (long)bmp_file_y * (img_width * 2 + padding);
		if (fseek(bmp_file, file_offset_for_row, SEEK_SET) != 0) {
			lcddbg("Failed to seek to BMP row\n");
			goto errout;
		}
		// Read one row of pixel data (as 16-bit words)
		uint16_t temp_row_buffer[img_width];
		if (fread(temp_row_buffer, sizeof(uint16_t), img_width, bmp_file) != img_width) {
			lcddbg("Failed to read BMP pixel row\n");
			goto errout;
		}
		for (int src_x = 0; src_x < img_width; src_x++) {
			int dest_x = (img_height - 1) - src_y;
			int dest_y = src_x;
			int target_buffer_index = ((y_offset + dest_y) * xres + (x_offset + dest_x)) * 2;
			uint16_t bmp_pixel = temp_row_buffer[src_x];
			uint16_t rgb565_pixel = convert_pixel_to_rgb565(bmp_pixel, r_mask, g_mask, b_mask);
			fullscreen_buffer[target_buffer_index] = (uint8_t)(rgb565_pixel & 0xFF);
			fullscreen_buffer[target_buffer_index + 1] = (uint8_t)((rgb565_pixel >> 8) & 0xFF);
		}
	}
	priv->config->lcd_put_area((u8 *)fullscreen_buffer, 1, 1, CONFIG_LCD_XRES, CONFIG_LCD_YRES);
	fclose(bmp_file);
	return OK;
errout:
	fclose(bmp_file);
#if !defined(CONFIG_LCD_SW_ROTATION)
	if (fullscreen_buffer) {
		kmm_free(fullscreen_buffer);
	}
#endif
	return -EIO;
}

/****************************************************************************
 * Name:  lcd_init_put_image
 *
 * Description:
 *   In normal booting case, render splash_normal.bmp and turn LCD on.
 *   In silent booting case, render splash_silent.bmp and turn LCD on.
 *   If there's no BMP file, turn off the LCD.
 * 
 * Returns:
 *   OK if image rendering succeeded and LCD turned on,
 *   ERROR if image rendering failed and LCD turned off
 *
 ****************************************************************************/

FAR int lcd_init_put_image(FAR struct lcd_dev_s *dev)
{
	bool is_silent_mode;
	char bmp_file_path[50];
	int ret = OK;
	FAR struct mipi_lcd_dev_s *priv = (FAR struct mipi_lcd_dev_s *)dev;

	is_silent_mode = silent_reboot_is_silent_mode();
	if (!is_silent_mode) {
		snprintf(bmp_file_path, sizeof(bmp_file_path), "%s/%dx%d/splash_normal.bmp", CONFIG_LCD_SPLASH_IMAGE_PATH, CONFIG_LCD_YRES, CONFIG_LCD_XRES);
	} else {
		snprintf(bmp_file_path, sizeof(bmp_file_path), "%s/%dx%d/splash_silent.bmp", CONFIG_LCD_SPLASH_IMAGE_PATH, CONFIG_LCD_YRES, CONFIG_LCD_XRES);
	}

	// Check if BMP file exists before rendering
	FILE *test_file = fopen(bmp_file_path, "rb");
	if (!test_file) {
		lcddbg("BMP file not found at %s. LCD OFF\n", bmp_file_path);
		return -ENOENT;
	}
	fclose(test_file);
	
	while (sem_wait(&priv->sem) != OK) {
		ASSERT(get_errno() == EINTR);
	}

	ret = lcd_power_on(priv);
	if (ret != OK) {
		lcddbg("Failed to turn on the LCD\n");
		sem_post(&priv->sem);
		return ret;
	}

	ret = lcd_render_bmp(dev, bmp_file_path);
	if (ret != OK) {
		if (lcd_power_off(priv) != OK) {
			sem_post(&priv->sem);
			return -EIO;
		}
		sem_post(&priv->sem);
		return ret;
	}
	priv->config->backlight(CONFIG_LCD_MAXPOWER);
	priv->power = CONFIG_LCD_MAXPOWER;
	sem_post(&priv->sem);
	priv->config->mipi_mode_switch(VIDEO_MODE);
	priv->lcdonoff = LCD_ON;

	return OK;
}

FAR struct lcd_dev_s *mipi_lcdinitialize(FAR struct mipi_dsi_device *dsi, struct mipi_lcd_config_s *config)
{
	FAR struct mipi_lcd_dev_s *priv = &g_lcdcdev;
	priv->dev.getplaneinfo = lcd_getplaneinfo;
	priv->dev.getvideoinfo = lcd_getvideoinfo;
	priv->dev.getlcdinfo = lcd_getlcdinfo;
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
