/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <debug.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include <tinyara/board.h>
#include <tinyara/lcd/lcd.h>
#include <tinyara/lcd/ili9341.h>

#include <tinyara/gpio.h>
#include <tinyara/spi/spi.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "esp32_lcd.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Configuration ************************************************************/
/* Check contrast selection */

#if !defined(CONFIG_LCD_MAXCONTRAST)
#define CONFIG_LCD_MAXCONTRAST 100
#endif

/* Check power setting */

#if !defined(CONFIG_LCD_MAXPOWER)
#define CONFIG_LCD_MAXPOWER 100
#endif

/* Simulated LCD geometry and color format */

#ifndef CONFIG_ESP32_FBWIDTH
#define CONFIG_ESP32_FBWIDTH  320 /* Framebuffer width in pixels */
#endif

#ifndef CONFIG_ESP32_FBHEIGHT
#define CONFIG_ESP32_FBHEIGHT 240 /* Framebuffer height in pixels */
#endif

#ifndef CONFIG_ESP32_FBBPP
#define CONFIG_ESP32_FBBPP    16  /* Framebuffer bytes per pixel (RGB) */
#endif

#define FB_STRIDE ((CONFIG_ESP32_FBBPP * CONFIG_ESP32_FBWIDTH + 7) >> 3)

#undef FB_FMT
#if CONFIG_ESP32_FBBPP == 1
#define FB_FMT FB_FMT_RGB1
#elif CONFIG_ESP32_FBBPP == 4
#define FB_FMT FB_FMT_RGB4
#elif CONFIG_ESP32_FBBPP == 8
#define FB_FMT FB_FMT_RGB8
#elif CONFIG_ESP32_FBBPP == 16
#define FB_FMT FB_FMT_RGB16_565
#elif CONFIG_ESP32_FBBPP == 24
#define FB_FMT FB_FMT_RGB24
#elif CONFIG_ESP32_FBBPP == 32
#define FB_FMT FB_FMT_RGB32
#else
#error "Unsupported BPP"
#endif

#define ESP_LCD_DC_GPIO 21
#define ESP_LCD_RESET_GPIO 18
#define ESP_LCD_BL_GPIO 5
#define ESP_LCD_SPI_CLOCK 40000000
#ifdef CONFIG_ESP_LCD_SPI_PORTNUM
#define ESP_LCD_SPI_NUM CONFIG_ESP_LCD_SPI_PORTNUM
#else
#define ESP_LCD_SPI_NUM 2
#endif

#define LCD_CMD_LEV   (0)
#define LCD_DATA_LEV  (1)
#define GPIO_NUM_MAX  (40)
#define MSLEEP(t) usleep((t) * 1000)
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define SWAPBYTES(i) ((i >> 8) | (i << 8))
//when enabling spiram and use 80M VSPI, it's conflict to use spiram for LCD transmit buffer
#if CONFIG_KMM_NHEAPS > 1
#define LCD_MALLOC(size) kmm_malloc_at(0, (size))
#else
#define LCD_MALLOC(size) kmm_malloc(size)
#endif

/****************************************************************************
 * Private Type Definition
 ****************************************************************************/

static void esp32_ili9341_select(FAR struct ili9341_lcd_s *lcd);
static void esp32_ili9341_deselect(FAR struct ili9341_lcd_s *lcd);
static int esp32_ili9341_sendcmd(FAR struct ili9341_lcd_s *lcd, const uint8_t cmd);
static int esp32_ili9341_sendparam(FAR struct ili9341_lcd_s *lcd, const uint8_t param);
static int esp32_ili9341_recvparam(FAR struct ili9341_lcd_s *lcd, uint8_t *param);
static int esp32_ili9341_recvgram(FAR struct ili9341_lcd_s *lcd, uint16_t *wd, uint32_t nwords);
static int esp32_ili9341_sendgram(FAR struct ili9341_lcd_s *lcd, const uint16_t *wd, uint32_t nwords);
static int esp32_ili9341_backlight(FAR struct ili9341_lcd_s *lcd, int level);

/* 
 * the esp32 lcd device abstraction, include public ili9341 control interface,
 * and a specific spi device which provide the cmd/data transmission.
 */
struct esp32_lcd_s {
	/* Publically visible device structure */
	struct ili9341_lcd_s lcd;

	/* Private driver-specific information follows */
	struct spi_dev_s* spidev;
};

FAR struct esp32_lcd_s g_lcd = {
	.lcd.select = esp32_ili9341_select,
	.lcd.deselect = esp32_ili9341_deselect,
	.lcd.sendcmd = esp32_ili9341_sendcmd,
	.lcd.sendparam = esp32_ili9341_sendparam,
	.lcd.recvparam = esp32_ili9341_recvparam,
	.lcd.recvgram = esp32_ili9341_recvgram,
	.lcd.sendgram = esp32_ili9341_sendgram,
	.lcd.backlight = esp32_ili9341_backlight
};

/* 
 * LCD device control interface provided in public ili9341
 */
FAR struct lcd_dev_s *g_lcddev = NULL;

lcd_conf_t g_devconf = {
	.pin_num_dc = ESP_LCD_DC_GPIO,
	.pin_num_rst = ESP_LCD_RESET_GPIO,
	.pin_num_bckl = ESP_LCD_BL_GPIO,
	.clk_freq = ESP_LCD_SPI_CLOCK,
	.rst_active_level = 0,
	.bckl_active_level = 0,//ESP_BCKL_ACTIVE_LEVEL, seems 0 or 1
	.spi_host = ESP_LCD_SPI_NUM, //(spi_host_device_t)ESP_LCD_SPI_NUM, means HSPI, replaced by spi port para ->ESP32  HSPI_PORT
};


/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
/****************************************************************************
 * Name: gpio_set_direction
 *
 * Description:
 *	 Set gpio direction
 *
 * Input Parameters:
 *	 port - gpio port number
 *     dir  - direction in/out
 * Returned Value:
 *	 OK - On Success
 *
 ****************************************************************************/

static int gpio_set_direction(int port, gpio_direciton_t dir)
{
	char devpath[16];
	snprintf(devpath, 16, "/dev/gpio%d", port);
	int fd = open(devpath, O_RDWR);
	if (fd < 0) {
		lldbg("fd open %s fail\n", devpath);
		return -1;
	}

	ioctl(fd, GPIOIOC_SET_DIRECTION, dir);

	close(fd);
	return OK;
}

/****************************************************************************
 * Name: gpio_set_level
 *
 * Description:
 *	 Set gpio pin value
 *
 * Input Parameters:
 *	 port  - gpio port number
 *     value - pin level 0/1
 * Returned Value:
 *	 OK - On Success
 *
 ****************************************************************************/
static int gpio_set_level(int port, int value)
{
	char buf[4];
	char devpath[16];
	int fd;

	snprintf(devpath, 16, "/dev/gpio%d", port);
	fd = open(devpath, O_RDWR);
	if (fd < 0) {
		lldbg("fd open fail\n");
		return -1;
	}

	ioctl(fd, GPIOIOC_SET_DIRECTION, GPIO_DIRECTION_OUT);
	if (write(fd, buf, snprintf(buf, sizeof(buf), "%d", !!value)) < 0) {
		lldbg("write error\n");
	}

	close(fd);
	return OK;
}

/****************************************************************************
 * Name: esp32_ili9341_select
 *
 * Description:
 *   Select the SPI, locking and re-configuring if necessary
 *
 * Input Parameters:
 *   lcd  - Reference to the public driver structure
 *
 * Returned Value:
 *
 ****************************************************************************/
static void esp32_ili9341_select(FAR struct ili9341_lcd_s *lcd)
{
	FAR struct esp32_lcd_s *priv = (FAR struct esp32_lcd_s *)lcd;
	SPI_LOCK(priv->spidev, true);
	SPI_SELECT(priv->spidev, CONFIG_ESP32_SPI_CS, true);
}

/****************************************************************************
 * Name: esp32_ili9341_deselect
 *
 * Description:
 *   De-select the SPI
 *
 * Input Parameters:
 *   lcd  - Reference to the public driver structure
 *
 * Returned Value:
 *
 ****************************************************************************/
static void esp32_ili9341_deselect(FAR struct ili9341_lcd_s *lcd)
{
	FAR struct esp32_lcd_s *priv = (FAR struct esp32_lcd_s *)lcd;
	SPI_SELECT(priv->spidev, CONFIG_ESP32_SPI_CS, false);
	SPI_LOCK(priv->spidev, false);
}

/****************************************************************************
 * Name: esp32_ili9341_sendcmd
 *
 * Description:
 *   Send a command to the lcd driver.
 *
 * Input Parameters:
 *   lcd  - Reference to the ili9341_lcd_s driver structure
 *   cmd  - command to send
 *
 * Returned Value:
 *   On success - OK
 *
 ****************************************************************************/
static int esp32_ili9341_sendcmd(FAR struct ili9341_lcd_s *lcd, const uint8_t cmd)
{
	FAR struct esp32_lcd_s *priv = (FAR struct esp32_lcd_s *)lcd;
	gpio_set_level((int)g_devconf.pin_num_dc, LCD_CMD_LEV);
	SPI_SNDBLOCK(priv->spidev, &cmd, 1);

	return OK;
}

/****************************************************************************
 * Name: esp32_lcd_data
 *
 * Description:
 *   Send data to the lcd driver with buffer and length.
 *
 * Input Parameters:
 *   spi  - Reference to the spi device driver
 *   data - command to send
 *   len  - data length to send
 * Returned Value:
 *
 ****************************************************************************/
static void esp32_lcd_data(struct spi_dev_s* spi, const uint8_t *data, int len)
{
	gpio_set_level((int)g_devconf.pin_num_dc, LCD_DATA_LEV);
	SPI_SNDBLOCK(spi, data, len);
}

/****************************************************************************
 * Name: esp32_ili9341_sendparam
 *
 * Description:
 *   Send a parameter to the lcd driver.
 *
 * Input Parameters:
 *   lcd    - Reference to the ili9341_lcd_s driver structure
 *   param  - parameter to send
 *
 * Returned Value:
 *   OK - On Success
 *
 ****************************************************************************/
static int esp32_ili9341_sendparam(FAR struct ili9341_lcd_s *lcd, const uint8_t param)
{
	FAR struct esp32_lcd_s *priv = (FAR struct esp32_lcd_s *)lcd;

	esp32_lcd_data(priv->spidev, &param, 1);

	return OK;
}

/****************************************************************************
 * Name: esp32_ili9341_recvparam
 *
 * Description:
 *   Receive a parameter from the lcd driver.
 *
 * Input Parameters:
 *   lcd    - Reference to the ili9341_lcd_s driver structure
 *   param  - Reference to where parameter receive
 *
 * Returned Value:
 *   OK - On Success
 *
 ****************************************************************************/
static int esp32_ili9341_recvparam(FAR struct ili9341_lcd_s *lcd, uint8_t *param)
{
	FAR struct esp32_lcd_s *priv = (FAR struct esp32_lcd_s *)lcd;

	gpio_set_level((int)g_devconf.pin_num_dc, LCD_DATA_LEV);
	SPI_RECVBLOCK(priv->spidev, param, 1);

	return OK;
}

/****************************************************************************
 * Name: esp32_ili9341_recvgram
 *
 * Description:
 *   Receive pixel words from the lcd driver gram.
 *
 * Input Parameters:
 *   lcd    - Reference to the public driver structure
 *   wd     - Reference to where the pixel words receive
 *   nwords - number of pixel words to receive
 *
 * Returned Value:
 *   OK - On Success
 *
 ****************************************************************************/
static int esp32_ili9341_recvgram(FAR struct ili9341_lcd_s *lcd, uint16_t *wd, uint32_t nwords)
{
	FAR struct esp32_lcd_s *priv = (FAR struct esp32_lcd_s *)lcd;

	gpio_set_level((int)g_devconf.pin_num_dc, LCD_DATA_LEV);
	SPI_RECVBLOCK(priv->spidev, wd, nwords);

	return nwords;
}

/****************************************************************************
 * Name: esp32_ili9341_sendgram
 *
 * Description:
 *   Send a number of pixel words to the lcd driver gram.
 *
 * Input Parameters:
 *   lcd    - Reference to the ili9341_lcd_s driver structure
 *   wd     - Reference to the words to send
 *   nwords - number of words to send
 *
 * Returned Value:
 *   OK - On Success
 *
 ****************************************************************************/
static int esp32_ili9341_sendgram(FAR struct ili9341_lcd_s *lcd, const uint16_t *wd, uint32_t nwords)
{
	int i;
	uint16_t data;
	FAR struct esp32_lcd_s *priv = (FAR struct esp32_lcd_s *)lcd;
	uint16_t *data_buf;
	int dma = 0;
	int dmasize;
	int buffidx;
	int trans_word = nwords;
#if (defined(CONFIG_SPI2_DMA_CHANNEL) && CONFIG_SPI2_DMA_CHANNEL > 0)
	dma = 1;
#endif

	if (dma) {
		dmasize = CONFIG_SPI_DMA_MAX_DATALEN / sizeof(uint16_t);

		data_buf = (uint16_t*)LCD_MALLOC(dmasize * sizeof(uint16_t));
		if (data_buf == NULL) {
			lldbg("[sendgram]MALLOC FAILED!\n");
			return 0;
		}

		buffidx = 0;
		while (trans_word > 0) {
			if (trans_word < dmasize) {
				dmasize = trans_word;
			}

			for (i = 0; i < dmasize; i++) {
				data_buf[i] = SWAPBYTES(wd[buffidx + i]);
			}
			esp32_lcd_data(priv->spidev, (uint8_t*)(data_buf), dmasize * sizeof(uint16_t));

			trans_word -= dmasize;
			buffidx += dmasize;
		}
		kmm_free(data_buf);
		data_buf = NULL;
	} else {
		for (i = 0; i < nwords; i++) {
			data = SWAPBYTES(wd[i]);
			esp32_lcd_data(priv->spidev, (uint8_t*)&data, 2);
		}
	}

	return nwords;
}

/****************************************************************************
 * Name: esp32_ili9341_backlight
 *
 * Description:
 *   Set the backlight level of the connected display.
 *
 * Input Parameters:
 *   lcd   - Reference to the public driver structure
 *   level - backligth level
 *
 * Returned Value:
 *   OK - On Success
 *
 ****************************************************************************/
static int esp32_ili9341_backlight(FAR struct ili9341_lcd_s *lcd, int level)
{
	int bckl_active_level  = level ? 0 : 1;

	gpio_pad_select_gpio(g_devconf.pin_num_bckl);
	gpio_set_direction(g_devconf.pin_num_bckl, GPIO_DIRECTION_OUT);
	gpio_set_level(g_devconf.pin_num_bckl, bckl_active_level & 0x1);

	return OK;
}


/****************************************************************************
 * Public Functions
 ****************************************************************************/
/****************************************************************************
 * Name:  esp32_ili9341_initialize
 *
 * Description:
 *	 Initialize the device structure to control the LCD Single chip driver.
 *
 * Input Parameters:
 *
 * Returned Value:
 *	 On success, this function returns a reference to the LCD control object
 *	 for the specified ILI9341 LCD Single chip driver connected as 4 wire serial
 *	 (spi). NULL is returned on any failure.
 *
 ****************************************************************************/
FAR struct ili9341_lcd_s *esp32_ili9341_initialize(void)
{
	struct spi_dev_s* lcd_handle;
	//Initialize non-SPI GPIOs
	gpio_pad_select_gpio(g_devconf.pin_num_dc);
	gpio_set_direction(g_devconf.pin_num_dc, GPIO_DIRECTION_OUT);

	//Reset the display
	if (g_devconf.pin_num_rst < GPIO_NUM_MAX) {
		gpio_pad_select_gpio(g_devconf.pin_num_rst);
		gpio_set_direction(g_devconf.pin_num_rst, GPIO_DIRECTION_OUT);
		gpio_set_level(g_devconf.pin_num_rst, (g_devconf.rst_active_level) & 0x1);
		MSLEEP(100);
		gpio_set_level(g_devconf.pin_num_rst, (~(g_devconf.rst_active_level)) & 0x1);
		MSLEEP(100);
	}

	g_lcd.spidev = lcd_handle = up_spiinitialize(g_devconf.spi_host); //HSPI_PORT
	SPI_SELECT(lcd_handle, CONFIG_ESP32_SPI_CS, true);
	SPI_SETBITS(lcd_handle, 8);
	SPI_SETMODE(lcd_handle, SPIDEV_MODE0);
	SPI_SETFREQUENCY(lcd_handle, g_devconf.clk_freq);

	//Enable backlight
	if (g_devconf.pin_num_bckl < GPIO_NUM_MAX) {
		gpio_pad_select_gpio(g_devconf.pin_num_bckl);
		gpio_set_direction(g_devconf.pin_num_bckl, GPIO_DIRECTION_OUT);
		gpio_set_level(g_devconf.pin_num_bckl, (g_devconf.bckl_active_level) & 0x1);
	}

	return &g_lcd.lcd;
}

/****************************************************************************
 * Name:  board_lcd_initialize
 *
 * Description:
 *   Initialize the LCD video hardware.  The initial state of the LCD is
 *   fully initialized, display memory cleared, and the LCD ready to use,
 *   but with the power setting at 0 (full off).
 *
 ****************************************************************************/
int board_lcd_initialize(void)
{
	g_lcddev = ili9341_initialize(esp32_ili9341_initialize(), 0);

	return OK;
}

/****************************************************************************
 * Name:  board_lcd_getdev
 *
 * Description:
 *   Return a a reference to the LCD object for the specified LCD.  This
 *   allows support for multiple LCD devices.
 *
 ****************************************************************************/
FAR struct lcd_dev_s *board_lcd_getdev(int lcddev)
{
	DEBUGASSERT(lcddev == 0);

	return g_lcddev;
}

/****************************************************************************
 * Name:  board_lcd_uninitialize
 *
 * Description:
 *   Unitialize the LCD support
 *
 ****************************************************************************/
void board_lcd_uninitialize(void)
{
	g_lcddev->setpower(g_lcddev, 0);
	
	g_lcddev = NULL;
}

