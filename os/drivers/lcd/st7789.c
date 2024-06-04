/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
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
/****************************************************************************
 * drivers/lcd/st7789.c
 *
 *   Copyright (C) 2017=-2018 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/arch.h>
#include <tinyara/spi/spi.h>
#include <tinyara/lcd/lcd.h>
#include <tinyara/lcd/lcd_dev.h>
#include <tinyara/lcd/st7789.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Verify that all configuration requirements have been met */

#ifndef CONFIG_LCD_ST7789_SPIMODE
#define CONFIG_LCD_ST7789_SPIMODE SPIDEV_MODE0
#endif

/* SPI frequency */

#ifndef CONFIG_LCD_ST7789_FREQUENCY
#define CONFIG_LCD_ST7789_FREQUENCY 50000000
#endif

/* Check contrast selection */

#if !defined(CONFIG_LCD_MAXCONTRAST)
#define CONFIG_LCD_MAXCONTRAST 1
#endif

/* Check power setting */

#if !defined(CONFIG_LCD_MAXPOWER) || CONFIG_LCD_MAXPOWER < 1
#define CONFIG_LCD_MAXPOWER 100
#endif

#if CONFIG_LCD_MAXPOWER > 255
#error "CONFIG_LCD_MAXPOWER must be less than 256 to fit in uint8_t"
#endif

/* Define prefixes for 3 wire communication if used */

#ifdef CONFIG_LCD_ST7789_3WIRE
#define LCD_ST7789_SPI_BITS 9
#define LCD_ST7789_DATA_PREFIX (1 << 8)
#define LCD_ST7789_CMD_PREFIX  (0 << 8)
#else
#define LCD_ST7789_SPI_BITS 8
#define LCD_ST7789_DATA_PREFIX (0)
#define LCD_ST7789_CMD_PREFIX  (0)
#endif

/* Display Resolution */

#if !defined(CONFIG_LCD_ST7789_XRES)
#define CONFIG_LCD_ST7789_XRES 240
#endif

#if !defined(CONFIG_LCD_ST7789_YRES)
#define CONFIG_LCD_ST7789_YRES 320
#endif

#if !defined(CONFIG_LCD_ST7789_YOFFSET)
#define CONFIG_LCD_ST7789_YOFFSET 0
#endif

#if !defined(CONFIG_LCD_ST7789_XOFFSET)
#define CONFIG_LCD_ST7789_XOFFSET 0
#endif

#define ST7789_LUT_SIZE    CONFIG_LCD_ST7789_YRES

#if defined(CONFIG_LCD_LANDSCAPE) || defined(CONFIG_LCD_RLANDSCAPE)
#define ST7789_XRES       CONFIG_LCD_ST7789_YRES
#define ST7789_YRES       CONFIG_LCD_ST7789_XRES
#define ST7789_XOFFSET    CONFIG_LCD_ST7789_YOFFSET
#define ST7789_YOFFSET    CONFIG_LCD_ST7789_XOFFSET
#elif defined(CONFIG_LCD_PORTRAIT) || defined(CONFIG_LCD_RPORTRAIT)
#define ST7789_XRES       CONFIG_LCD_ST7789_XRES
#define ST7789_YRES       CONFIG_LCD_ST7789_YRES
#define ST7789_XOFFSET    0
#define ST7789_YOFFSET    0
#else
#define ST7789_XRES       CONFIG_LCD_ST7789_YRES
#define ST7789_YRES       CONFIG_LCD_ST7789_XRES
#define ST7789_XOFFSET    CONFIG_LCD_ST7789_YOFFSET
#define ST7789_YOFFSET    CONFIG_LCD_ST7789_XOFFSET
#endif

/* Color depth and format */

//#ifdef CONFIG_LCD_ST7789_BPP
#if (CONFIG_LCD_ST7789_BPP == 12)
#define ST7789_BPP           12
#define ST7789_COLORFMT      FB_FMT_RGB12_444
#define ST7789_BYTESPP       2
#elif (CONFIG_LCD_ST7789_BPP == 16)
#define ST7789_BPP           16
#define ST7789_COLORFMT      FB_FMT_RGB16_565
#define ST7789_BYTESPP       2
#else
#define ST7789_BPP           16
#define ST7789_COLORFMT      FB_FMT_RGB16_565
#define ST7789_BYTESPP       2
#warning "Invalid color depth.  Falling back to 16bpp"
#endif
//#endif

/* Default orientation */
#if defined(CONFIG_LCD_PORTRAIT)
#define ST7789_DEFAULT_ORIENTATION LCD_PORTRAIT
#elif defined(CONFIG_LCD_LANDSCAPE)
#define ST7789_DEFAULT_ORIENTATION LCD_LANDSCAPE
#elif defined(CONFIG_LCD_RLANDSCAPE)
#define ST7789_DEFAULT_ORIENTATION LCD_RLANDSCAPE
#elif defined(CONFIG_LCD_RPORTRAIT)
#define ST7789_DEFAULT_ORIENTATION LCD_RPORTRAIT
#else
#define ST7789_DEFAULT_ORIENTATION LCD_LANDSCAPE
#endif

/****************************************************************************
 * Private Types
 ****************************************************************************/

/* This structure describes the state of this driver */

struct st7789_dev_s {
	/* Publicly visible device structure */

	struct lcd_dev_s dev;
	struct st7789_config_s *config;
	/* Private LCD-specific information follows */

	FAR struct spi_dev_s *spi;	/* SPI device */
	uint8_t bpp;				/* Selected color depth */
	uint8_t power;				/* Current power setting */

	/* This is working memory allocated by the LCD driver for each LCD device
	 * and for each color plane. This memory will hold one raster line of data.
	 * The size of the allocated run buffer must therefore be at least
	 * (bpp * xres / 8).  Actual alignment of the buffer must conform to the
	 * bitwidth of the underlying pixel type.
	 *
	 * If there are multiple planes, they may share the same working buffer
	 * because different planes will not be operate on concurrently.  However,
	 * if there are multiple LCD devices, they must each have unique run
	 * buffers.
	 */
	uint16_t runbuffer[ST7789_LUT_SIZE];
};

typedef struct Lcd_Init_Data {
	u8 type;					//cmd 0 or data 1
	u8 data;
} Lcd_Init_Data;

/* 3 wire interface for ST7789 requires the driver to send information
 * about command/data transfer as 9th bit of SPI transfer. This would
 * require non standard SPI interface that is not supported so a little
 * workaround is used here (inspire by SSD1351 driver). We split our
 * buffer into rows and send those rows separately with added 9th bit.
 * The price for this is a small overhead in SPI communication.
 */

#ifdef CONFIG_LCD_ST7789_3WIRE
uint16_t rowbuff[ST7789_XRES * ST7789_BYTESPP];
#endif

/****************************************************************************
 * Private Function Protototypes
 ****************************************************************************/

/* Misc. Helpers */
static void lcd_lock(FAR struct spi_dev_s *dev);
static inline void lcd_unlock(FAR struct spi_dev_s *dev);
static inline void st7789_sendcmd(FAR struct st7789_dev_s *dev, uint8_t cmd);
static inline void st7789_senddata(FAR struct st7789_dev_s *dev, uint8_t *txbuf, int size);
static inline void st7789_getdata(FAR struct st7789_dev_s *dev, uint8_t *rxbuf, int size);
static void st7789_setarea(FAR struct st7789_dev_s *dev, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
static void st7789_bpp(FAR struct st7789_dev_s *dev, int bpp);
static void st7789_wrram(FAR struct st7789_dev_s *dev, FAR const uint8_t *buff, size_t size, size_t skip, size_t count);
#ifndef CONFIG_LCD_NOGETRUN
static void st7789_rdram(FAR struct st7789_dev_s *dev, FAR uint8_t *buff, size_t size);
#endif

/* LCD Data Transfer Methods */

static int st7789_putrun(FAR struct lcd_dev_s *dev, fb_coord_t row, fb_coord_t col, FAR const uint8_t *buffer, size_t npixels);
static int st7789_putarea(FAR struct lcd_dev_s *dev, fb_coord_t row_start, fb_coord_t row_end, fb_coord_t col_start, fb_coord_t col_end, FAR const uint8_t *buffer, fb_coord_t stride);
#ifndef CONFIG_LCD_NOGETRUN
static int st7789_getrun(FAR struct lcd_dev_s *dev, fb_coord_t row, fb_coord_t col, FAR uint8_t *buffer, size_t npixels);
#endif

/* LCD Configuration */

static int st7789_getvideoinfo(FAR struct lcd_dev_s *dev, FAR struct fb_videoinfo_s *vinfo);
static int st7789_getplaneinfo(FAR struct lcd_dev_s *dev, unsigned int planeno, FAR struct lcd_planeinfo_s *pinfo);

/* LCD Specific Controls */

static int st7789_getpower(FAR struct lcd_dev_s *dev);
static int st7789_setpower(FAR struct lcd_dev_s *dev, int power);
static int st7789_getcontrast(FAR struct lcd_dev_s *dev);
static int st7789_setcontrast(FAR struct lcd_dev_s *dev, unsigned int contrast);
static int st7789_setorientation(FAR struct lcd_dev_s *dev, unsigned int orientation);
static int st7789_init(FAR struct lcd_dev_s *dev);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static struct st7789_dev_s g_lcddev;

static Lcd_Init_Data st7789_init_data[] = {
	{0, 0x11},
	{2, 120},
	{0, 0x2a},
	{1, 0x00}, {1, 0x00}, {1, 0x00}, {1, 0xef},
	{0, 0x2b},
	{1, 0x00}, {1, 0x28}, {1, 0x01}, {1, 0x17},
	{0, 0xb2},
	{1, 0x0c}, {1, 0x0c}, {1, 0x00}, {1, 0x33}, {1, 0x33},
	{0, 0x20},
	{0, 0xb7},
	{1, 0x56},
	{0, 0xbb},
	{1, 0x38},
	{0, 0xc0},
	{1, 0x2c},
	{0, 0xc2},
	{1, 0x01},
	{0, 0xc3},
	{1, 0x1f},
	{0, 0xc4},
	{1, 0x20},
	{0, 0xc6},
	{1, 0x0f},
	{0, 0xd0},
	{1, 0xa6}, {1, 0xa1},
	{0, 0xe0},
	{1, 0xd0}, {1, 0x0d}, {1, 0x14}, {1, 0x0b}, {1, 0x0b}, {1, 0x07}, {1, 0x3a}, {1, 0x44}, {1, 0x50}, {1, 0x08}, {1, 0x13}, {1, 0x13}, {1, 0x2d}, {1, 0x32},
	{0, 0xe1},
	{1, 0xd0}, {1, 0x0d}, {1, 0x14}, {1, 0x0b}, {1, 0x0b}, {1, 0x07}, {1, 0x3a}, {1, 0x44}, {1, 0x50}, {1, 0x08}, {1, 0x13}, {1, 0x13}, {1, 0x2d}, {1, 0x32}, {1, 0x00}, {1, 0x00},
	{0, 0x36},
	{1, 0x00},
	{0, 0x3A},
	{1, 0x55},
	{0, 0xe7},
	{1, 0x00},
	{0, 0x21},
	{0, 0x29},
	{0, 0x2C}
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/
/************************************************************************************
 * Name: lcd_lock
 ************************************************************************************/

static void lcd_lock(FAR struct spi_dev_s *dev)
{
	/* On SPI busses where there are multiple devices, it will be necessary to
	 * lock SPI to have exclusive access to the busses for a sequence of
	 * transfers.  The bus should be locked before the chip is selected.
	 *
	 * This is a blocking call and will not return until we have exclusiv access to
	 * the SPI buss.  We will retain that exclusive access until the bus is unlocked.
	 */

	(void)SPI_LOCK(dev, true);

	/* After locking the SPI bus, the we also need call the setfrequency, setbits, and
	 * setmode methods to make sure that the SPI is properly configured for the device.
	 * If the SPI buss is being shared, then it may have been left in an incompatible
	 * state.
	 */
	SPI_SETMODE(dev, CONFIG_LCD_ST7789_SPIMODE);
	SPI_SETBITS(dev, LCD_ST7789_SPI_BITS);
	SPI_SETFREQUENCY(dev, CONFIG_LCD_ST7789_FREQUENCY);
}

/************************************************************************************
 * Name: lcd_unlock
 ************************************************************************************/

static inline void lcd_unlock(FAR struct spi_dev_s *dev)
{
	(void)SPI_LOCK(dev, false);
}

/****************************************************************************
 * Name: st7789_sendcmd
 *
 * Description:
 *   Send a command to the driver.
 *
 ****************************************************************************/
static inline void st7789_sendcmd(FAR struct st7789_dev_s *dev, uint8_t cmd)
{
#ifdef CONFIG_LCD_ST7789_3WIRE
	uint16_t txbuf;
	/* Add command prefix (9th bit shoudl be 0 ) */
	txbuf = LCD_ST7789_CMD_PREFIX | cmd;
	SPI_SELECT(dev->spi, SPIDEV_DISPLAY, true);
	SPI_SEND(dev->spi, txbuf);
	SPI_SELECT(dev->spi, SPIDEV_DISPLAY, false);
#else
	uint8_t spi_cmd[4];
	spi_cmd[0] = cmd;
	SPI_SELECT(dev->spi, SPIDEV_DISPLAY, true);
	dev->config->cmddata(0);
	SPI_SNDBLOCK(dev->spi, spi_cmd, 1);
	SPI_SELECT(dev->spi, SPIDEV_DISPLAY, false);
#endif
}

/****************************************************************************
 * Name: st7789_senddata
 *
 * Description:
 *   Send init data to the driver.
 * data - tx buffer
 * size - total bytes of words
 *        word size(8 bits)
 ****************************************************************************/

static inline void st7789_senddata(FAR struct st7789_dev_s *dev, uint8_t *txbuf, int size)
{
#ifdef CONFIG_LCD_ST7789_3WIRE
	uint16_t txbuf;
	/* Add data prefix (9th bit shoudl be 1 ) */
	SPI_SELECT(dev->spi, SPIDEV_DISPLAY, true);
	for (int j = 0; j < size; j += 2) {
		/* Take care of correct byte order. */

		rowbuff[j] = LCD_ST7789_DATA_PREFIX | (uint16_t) txbuf[j + 1 + (i * size)];
		rowbuff[j + 1] = LCD_ST7789_DATA_PREFIX | (uint16_t) txbuf[j + (i * size)];
	}
	SPI_SNDBLOCK(dev->spi, rowbuff, size);
	SPI_SELECT(dev->spi, SPIDEV_DISPLAY, false);
#else
	SPI_SELECT(dev->spi, SPIDEV_DISPLAY, true);
	dev->config->cmddata(1);
	SPI_SNDBLOCK(dev->spi, txbuf, size);
	SPI_SELECT(dev->spi, SPIDEV_DISPLAY, false);
#endif
}

/****************************************************************************
 * Name: st7789_getdata
 *
 * Description:
 *   Get data from LCD.
 *
 * buf - rx buffer
 * size - total bytes of words
 *        word size(8 bits)
 ****************************************************************************/

static inline void st7789_getdata(FAR struct st7789_dev_s *dev, uint8_t *rxbuf, int size)
{
	SPI_SELECT(dev->spi, SPIDEV_DISPLAY, true);
	dev->config->cmddata(1);
	SPI_RECVBLOCK(dev->spi, rxbuf, size);
	SPI_SELECT(dev->spi, SPIDEV_DISPLAY, false);
}

/****************************************************************************
 * Name: st7789_setarea
 *
 * Description:
 *   Set the rectangular area for an upcoming read or write from RAM.
 *
 ****************************************************************************/

static void st7789_setarea(FAR struct st7789_dev_s *dev, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
	/* Set row address */
	st7789_sendcmd(dev, ST7789_RASET);
	SPI_SELECT(dev->spi, SPIDEV_DISPLAY, true);
	dev->config->cmddata(1);
	SPI_SEND(dev->spi, LCD_ST7789_DATA_PREFIX | ((y0 + ST7789_YOFFSET) >> 8));
	SPI_SEND(dev->spi, LCD_ST7789_DATA_PREFIX | ((y0 + ST7789_YOFFSET) & 0xff));
	SPI_SEND(dev->spi, LCD_ST7789_DATA_PREFIX | ((y1 + ST7789_YOFFSET) >> 8));
	SPI_SEND(dev->spi, LCD_ST7789_DATA_PREFIX | ((y1 + ST7789_YOFFSET) & 0xff));
	SPI_SELECT(dev->spi, SPIDEV_DISPLAY, false);

	/* Set column address */
	st7789_sendcmd(dev, ST7789_CASET);
	SPI_SELECT(dev->spi, SPIDEV_DISPLAY, true);
	dev->config->cmddata(1);
	SPI_SEND(dev->spi, LCD_ST7789_DATA_PREFIX | ((x0 + ST7789_XOFFSET) >> 8));
	SPI_SEND(dev->spi, LCD_ST7789_DATA_PREFIX | ((x0 + ST7789_XOFFSET) & 0xff));
	SPI_SEND(dev->spi, LCD_ST7789_DATA_PREFIX | ((x1 + ST7789_XOFFSET) >> 8));
	SPI_SEND(dev->spi, LCD_ST7789_DATA_PREFIX | ((x1 + ST7789_XOFFSET) & 0xff));
	SPI_SELECT(dev->spi, SPIDEV_DISPLAY, false);
}

/****************************************************************************
 * Name: st7789_bpp
 *
 * Description:
 *   Set the color depth of the device.
 *
 ****************************************************************************/

static void st7789_bpp(FAR struct st7789_dev_s *dev, int bpp)
{
	uint8_t depth;
	/* REVISIT: Works only for 12 and 16 bpp! */
	depth = bpp >> 2 | 1;
	st7789_sendcmd(dev, ST7789_COLMOD);
	SPI_SELECT(dev->spi, SPIDEV_DISPLAY, true);
	dev->config->cmddata(1);
	SPI_SEND(dev->spi, LCD_ST7789_DATA_PREFIX | depth);
	SPI_SELECT(dev->spi, SPIDEV_DISPLAY, false);
	/* Cache the new BPP */
	dev->bpp = bpp;
}

/****************************************************************************
 * Name: st7789_wrram
 *
 * Description:
 *   Write to the driver's RAM. It is possible to write multiples of size
 *   while skipping some values.
 *
 ****************************************************************************/

static void st7789_wrram(FAR struct st7789_dev_s *dev, FAR const uint8_t *buff, size_t size, size_t skip, size_t count)
{
	size_t i;
#ifdef CONFIG_LCD_ST7789_3WIRE
	size_t j;
#endif
	st7789_sendcmd(dev, ST7789_RAMWR);

#ifdef CONFIG_LCD_ST7789_3WIRE
	if (count == 1) {
		/* We cannot send the entire buffer at once, split it to
		 * separate rows.
		 */
		count = ST7789_YRES;
		size = ST7789_XRES * ST7789_BYTESPP;
	}

	SPI_SELECT(dev->spi, SPIDEV_DISPLAY, true);
	/* For each row */
	for (i = 0; i < count; i++) {
		/* Copy data to rowbuff and add 9th bit */
		for (j = 0; j < ST7789_XRES * ST7789_BYTESPP; j += 2) {
			/* Take care of correct byte order. */
			rowbuff[j] = LCD_ST7789_DATA_PREFIX | (uint16_t) buff[j + 1 + (i * (size + skip))];
			rowbuff[j + 1] = LCD_ST7789_DATA_PREFIX | (uint16_t) buff[j + (i * (size + skip))];
		}
		SPI_SNDBLOCK(dev->spi, rowbuff, size);
	}
#else
	SPI_SELECT(dev->spi, SPIDEV_DISPLAY, true);
	dev->config->cmddata(1);
	for (i = 0; i < count; i++) {
		SPI_SNDBLOCK(dev->spi, buff + (i * (size + skip)), size);
	}
#endif

	SPI_SELECT(dev->spi, SPIDEV_DISPLAY, false);
}

/****************************************************************************
 * Name: st7789_rdram
 *
 * Description:
 *   Read from the driver's RAM.
 *
 ****************************************************************************/

static void st7789_rdram(FAR struct st7789_dev_s *dev, FAR uint8_t *buff, size_t size)
{
	st7789_sendcmd(dev, ST7789_RAMRD);
	SPI_SELECT(dev->spi, SPIDEV_DISPLAY, true);
	dev->config->cmddata(1);
	SPI_RECVBLOCK(dev->spi, buff, size);
	SPI_SELECT(dev->spi, SPIDEV_DISPLAY, false);
}

/****************************************************************************
 * Name:  st7789_init_cmd
 *
 * Description:
 *   Initialization command and data
 *
 ****************************************************************************/

static int st7789_init_cmd(FAR struct st7789_dev_s *dev)
{
	size_t i = 0;
	size_t len = sizeof(st7789_init_data) / sizeof(st7789_init_data[0]);
	while (i < len) {
		switch (st7789_init_data[i].type) {
		case 0:
			st7789_sendcmd(dev, st7789_init_data[i].data);
			break;
		case 1:
			st7789_senddata(dev, &st7789_init_data[i].data, 1);
			break;
		case 2:
			up_mdelay(st7789_init_data[i].data);
			break;
		default:
			printf("unknown command\n");
			break;
		}
		i++;
	}
	return OK;
}

/****************************************************************************
 * Name:  st7789_putrun
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

static int st7789_putrun(FAR struct lcd_dev_s *dev, fb_coord_t row, fb_coord_t col, FAR const uint8_t *buffer, size_t npixels)
{
	FAR struct st7789_dev_s *priv = (FAR struct st7789_dev_s *)dev;
	DEBUGASSERT(buffer && ((uintptr_t) buffer & 1) == 0);
	lcd_lock(priv->spi);
	st7789_setarea(priv, col, row, col + npixels - 1, row);
	st7789_wrram(priv, buffer, npixels * ST7789_BYTESPP, 0, 1);
	lcd_unlock(priv->spi);
	return OK;
}

/****************************************************************************
 * Name:  st7789_putarea
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

static int st7789_putarea(FAR struct lcd_dev_s *dev, fb_coord_t row_start, fb_coord_t row_end, fb_coord_t col_start, fb_coord_t col_end, FAR const uint8_t *buffer, fb_coord_t stride)
{
	FAR struct st7789_dev_s *priv = (FAR struct st7789_dev_s *)dev;
	size_t cols = col_end - col_start + 1;
	size_t rows = row_end - row_start + 1;
	size_t row_size = cols * ST7789_BYTESPP;
	DEBUGASSERT(buffer && ((uintptr_t) buffer & 1) == 0);
	lcd_lock(priv->spi);
	st7789_setarea(priv, col_start, row_start, col_end, row_end);
	/* If the stride is the same of the row, a single SPI transfer is enough.
	 * That is always true for lcddev. For framebuffer, that indicates a full
	 * screen or full row update.
	 */
	if (stride == row_size) {
		st7789_wrram(priv, buffer, rows * row_size, 0, 1);
	} else {
		st7789_wrram(priv, buffer, row_size, stride - row_size, rows);
	}
	lcd_unlock(priv->spi);
	return OK;
}

/****************************************************************************
 * Name:  st7789_getrun
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
static int st7789_getrun(FAR struct lcd_dev_s *dev, fb_coord_t row, fb_coord_t col, FAR uint8_t *buffer, size_t npixels)
{
	FAR struct st7789_dev_s *priv = (FAR struct st7789_dev_s *)dev;
	FAR uint8_t *dest = (FAR uint8_t *) buffer;
	DEBUGASSERT(buffer && ((uintptr_t) buffer & 1) == 0);
	lcd_lock(priv->spi);
	st7789_setarea(priv, col, row, col + npixels - 1, row);
	st7789_rdram(priv, dest, npixels);
	lcd_unlock(priv->spi);
	return OK;
}
#endif

/****************************************************************************
 * Name:  st7789_getvideoinfo
 *
 * Description:
 *   Get information about the LCD video controller configuration.
 *
 ****************************************************************************/

static int st7789_getvideoinfo(FAR struct lcd_dev_s *dev, FAR struct fb_videoinfo_s *vinfo)
{
	DEBUGASSERT(dev && vinfo);
	vinfo->fmt = ST7789_COLORFMT;	/* Color format: RGB16-565: RRRR RGGG GGGB BBBB */
	vinfo->xres = ST7789_XRES;	/* Horizontal resolution in pixel columns */
	vinfo->yres = ST7789_YRES;	/* Vertical resolution in pixel rows */
	vinfo->nplanes = 1;			/* Number of color planes supported */
	return OK;
}

/****************************************************************************
 * Name:  st7789_getplaneinfo
 *
 * Description:
 *   Get information about the configuration of each LCD color plane.
 *
 ****************************************************************************/

static int st7789_getplaneinfo(FAR struct lcd_dev_s *dev, unsigned int planeno, FAR struct lcd_planeinfo_s *pinfo)
{
	FAR struct st7789_dev_s *priv = (FAR struct st7789_dev_s *)dev;
	DEBUGASSERT(dev && pinfo && planeno == 0);
	pinfo->putrun = st7789_putrun;	/* Put a run into LCD memory */
	pinfo->putarea = st7789_putarea;	/* Put an area into LCD */
#ifndef CONFIG_LCD_NOGETRUN
	pinfo->getrun = st7789_getrun;	/* Get a run from LCD memory */
#endif
	pinfo->buffer = (FAR uint8_t *) priv->runbuffer;	/* Run scratch buffer */
	pinfo->bpp = priv->bpp;		/* Bits-per-pixel */
	return OK;
}

/****************************************************************************
 * Name:  st7789_getpower
 ****************************************************************************/

static int st7789_getpower(FAR struct lcd_dev_s *dev)
{
	FAR struct st7789_dev_s *priv = (FAR struct st7789_dev_s *)dev;
	return priv->power;
}

/****************************************************************************
 * Name:  st7789_setpower
 ****************************************************************************/

static int st7789_setpower(FAR struct lcd_dev_s *dev, int power)
{
	FAR struct st7789_dev_s *priv = (FAR struct st7789_dev_s *)dev;
	DEBUGASSERT((unsigned)power <= CONFIG_LCD_MAXPOWER);
	/* Set new power level */
	lcd_lock(priv->spi);
	if (power > 0) {
		priv->config->backlight(power);
		st7789_sendcmd(priv, ST7789_DISPON);
		priv->power = power;
	} else {
		st7789_sendcmd(priv, ST7789_DISPOFF);
		priv->power = 0;
	}
	lcd_unlock(priv->spi);
	return OK;
}

/****************************************************************************
 * Name:  st7789_init
 *
 * Description:
 *   init the lcd
 *
 ****************************************************************************/

static int st7789_init(FAR struct lcd_dev_s *dev)
{
	FAR struct st7789_dev_s *priv = (FAR struct st7789_dev_s *)dev;
	priv->config->reset();
	lcd_lock(priv->spi);
	st7789_init_cmd(priv);
	st7789_bpp(priv, ST7789_BPP);
	st7789_sendcmd(priv, ST7789_DISPON);
	lcd_unlock(priv->spi);

	st7789_setorientation(dev, ST7789_DEFAULT_ORIENTATION);

	return OK;
}

/****************************************************************************
 * Name:  st7789_setorientation
 *
 * Description:
 *   Set LCD orientation(PORTRAIT/LANDSCAPE).
 *
 ****************************************************************************/

static int st7789_setorientation(FAR struct lcd_dev_s *dev, unsigned int orientation)
{
	FAR struct st7789_dev_s *priv = (FAR struct st7789_dev_s *)dev;
	lcd_lock(priv->spi);
	st7789_sendcmd(priv, ST7789_MADCTL);
	SPI_SELECT(priv->spi, SPIDEV_DISPLAY, true);
	priv->config->cmddata(1);
	if (orientation == LCD_PORTRAIT) {
		SPI_SEND(priv->spi, LCD_ST7789_DATA_PREFIX | ST7789_PORTRAIT);
	}
	else if (orientation == LCD_RPORTRAIT) {
		SPI_SEND(priv->spi, LCD_ST7789_DATA_PREFIX | ST7789_RPORTRAIT);
	}
	else if (orientation == LCD_LANDSCAPE) {
		SPI_SEND(priv->spi, LCD_ST7789_DATA_PREFIX | ST7789_LANDSCAPE);
	}
	else if (orientation == LCD_RLANDSCAPE) {
		SPI_SEND(priv->spi, LCD_ST7789_DATA_PREFIX | ST7789_RLANDSCAPE);
	}
	SPI_SELECT(priv->spi, SPIDEV_DISPLAY, false);
	lcd_unlock(priv->spi);
	return OK;
}

/****************************************************************************
 * Name:  st7789_getcontrast
 *
 * Description:
 *   Get the current contrast setting (0-CONFIG_LCD_MAXCONTRAST).
 *
 ****************************************************************************/

static int st7789_getcontrast(FAR struct lcd_dev_s *dev)
{
	FAR struct st7789_dev_s *priv = (FAR struct st7789_dev_s *)dev;
	uint8_t buf[2];
	lcd_lock(priv->spi);
	st7789_sendcmd(priv, ST7789_RDDISBV);
	st7789_getdata(priv, buf, 2);
	lcd_unlock(priv->spi);
	return buf[0] << 2 || buf[1];
}

/****************************************************************************
 * Name:  st7789_setcontrast
 *
 * Description:
 *   Set LCD panel contrast (0-CONFIG_LCD_MAXCONTRAST).
 *
 ****************************************************************************/

static int st7789_setcontrast(FAR struct lcd_dev_s *dev, unsigned int contrast)
{
	FAR struct st7789_dev_s *priv = (FAR struct st7789_dev_s *)dev;
	lcd_lock(priv->spi);
	st7789_sendcmd(priv, ST7789_WRDISBV);
	st7789_senddata(priv, &contrast, 1);
	lcd_unlock(priv->spi);
	return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name:  st7789_initialize
 *
 * Description:
 *   Initialize the ST7789 video hardware.  The initial state of the
 *   LCD is fully initialized, display memory cleared, and the LCD ready
 *   to use, but with the power setting at 0 (full off == sleep mode).
 *
 * Returned Value:
 *
 *   On success, this function returns a reference to the LCD object for
 *   the specified LCD.  NULL is returned on any failure.
 *
 ****************************************************************************/

FAR struct lcd_dev_s *st7789_lcdinitialize(FAR struct spi_dev_s *spi, struct st7789_config_s *config)
{
	FAR struct st7789_dev_s *priv = &g_lcddev;
	/* Initialize the driver data structure */
	priv->dev.getvideoinfo = st7789_getvideoinfo;
	priv->dev.getplaneinfo = st7789_getplaneinfo;
	priv->dev.getpower = st7789_getpower;
	priv->dev.setpower = st7789_setpower;
	priv->dev.getcontrast = st7789_getcontrast;
	priv->dev.setcontrast = st7789_setcontrast;
	priv->dev.setorientation = st7789_setorientation;
	priv->dev.init = st7789_init;
	priv->spi = spi;
	priv->config = config;

	priv->config->backlight(CONFIG_LCD_MAXPOWER);

	st7789_init(&priv->dev);
	return &priv->dev;
}

//#endif /* CONFIG_LCD_ST7789 */
