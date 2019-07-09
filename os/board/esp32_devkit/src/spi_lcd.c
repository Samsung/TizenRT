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

#include <tinyara/gpio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include "spi_lcd.h"

// Forces data into DRAM instead of flash
#define DRAM_ATTR __attribute__((section(".dram1")))

/*
 This struct stores a bunch of command values to be initialized for ILI9341
*/
typedef struct {
	uint8_t cmd;
	uint8_t data[16];
	uint8_t databytes; //No of data in data; bit 7 = delay after set; 0xFF = end of cmds.
} lcd_init_cmd_t;

DRAM_ATTR static const lcd_init_cmd_t ili_init_cmds[] = {
	{0xCF, {0x00, 0x83, 0x30}, 3},
	{0xED, {0x64, 0x03, 0x12, 0x81}, 4},
	{0xE8, {0x85, 0x01, 0x79}, 3},
	{0xCB, {0x39, 0x2C, 0x00, 0x34, 0x02}, 5},
	{0xF7, {0x20}, 1},
	{0xEA, {0x00, 0x00}, 2},
	{0xC0, {0x26}, 1},
	{0xC1, {0x11}, 1},
	{0xC5, {0x35, 0x3E}, 2},
	{0xC7, {0xBE}, 1},
	{0x36, {0x28}, 1},
	{0x3A, {0x55}, 1},
	{0xB1, {0x00, 0x1B}, 2},
	{0xF2, {0x08}, 1},
	{0x26, {0x01}, 1},
	{0xE0, {0x1F, 0x1A, 0x18, 0x0A, 0x0F, 0x06, 0x45, 0X87, 0x32, 0x0A, 0x07, 0x02, 0x07, 0x05, 0x00}, 15},
	{0XE1, {0x00, 0x25, 0x27, 0x05, 0x10, 0x09, 0x3A, 0x78, 0x4D, 0x05, 0x18, 0x0D, 0x38, 0x3A, 0x1F}, 15},
	{0x2A, {0x00, 0x00, 0x00, 0xEF}, 4},
	{0x2B, {0x00, 0x00, 0x01, 0x3f}, 4},
	{0x2C, {0}, 0},
	{0xB7, {0x07}, 1},
	{0xB6, {0x0A, 0x82, 0x27, 0x00}, 4},
	{0x11, {0}, 0x80},
	{0x29, {0}, 0x80},
	{0, {0}, 0xff},
};

DRAM_ATTR static const lcd_init_cmd_t st7789_init_cmds[] = {
	{0xC0, {0x00}, 1},           //LCMCTRL: LCM Control [2C] //sumpremely related to 0x36, MADCTL
	{0xC2, {0x01, 0xFF}, 2},     //VDVVRHEN: VDV and VRH Command Enable [01 FF]
	{0xC3, {0x13}, 1},           //VRHS: VRH Set VAP=???, VAN=-??? [0B]
	{0xC4, {0x20}, 1},           //VDVS: VDV Set [20]
	{0xC6, {0x0F}, 1},           //FRCTRL2: Frame Rate control in normal mode [0F]
	{0xCA, {0x0F}, 1},           //REGSEL2 [0F]
	{0xC8, {0x08}, 1},           //REGSEL1 [08]
	{0x55, {0xB0}, 1},           //WRCACE  [00]
	{0x36, {0x00}, 1},
	{0x3A, {0x55}, 1},             //this says 0x05
	{0xB1, {0x40, 0x02, 0x14}, 3}, //sync setting not reqd
	{0x26, {0x01}, 1},
	{0x2A, {0x00, 0x00, 0x00, 0xEF}, 4},
	{0x2B, {0x00, 0x00, 0x01, 0x3F}, 4},
	{0x2C, {0x00}, 1},
	{0xE0, {0xD0, 0x00, 0x05, 0x0E, 0x15, 0x0D, 0x37, 0x43, 0x47, 0x09, 0x15, 0x12, 0x16, 0x19}, 14},    //PVGAMCTRL: Positive Voltage Gamma control
	{0xE1, {0xD0, 0x00, 0x05, 0x0D, 0x0C, 0x06, 0x2D, 0x44, 0x40, 0x0E, 0x1C, 0x18, 0x16, 0x19}, 14},    //NVGAMCTRL: Negative Voltage Gamma control
	{0x11, {0}, 0x80},
	{0x29, {0}, 0x80},
	{0, {0}, 0xff},
};

#define LCD_CMD_LEV   (0)
#define LCD_DATA_LEV  (1)

#define MSLEEP(t) usleep((t) * 1000)
#define MIN(a,b) ((a)<(b) ? (a):(b))

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
	return 0;
}

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
	return 0;
}

void lcd_cmd(spi_device_handle_t spi, const uint8_t cmd, lcd_dc_t *dc)
{
	SPI_LOCK(spi, true);

	gpio_set_level((int)dc->dc_io, LCD_CMD_LEV);
	SPI_SNDBLOCK(spi, &cmd, 1);

	SPI_LOCK(spi, false);
	return;
}

void lcd_data(spi_device_handle_t spi, const uint8_t *data, int len, lcd_dc_t *dc)
{
	SPI_LOCK(spi, true);

	gpio_set_level((int)dc->dc_io, LCD_DATA_LEV);
	SPI_SNDBLOCK(spi, data, len);

	SPI_LOCK(spi, false);
	return;
}

uint32_t lcd_init(lcd_conf_t *lcd_conf, spi_device_handle_t *spi_wr_dev, lcd_dc_t *dc, int dma_chan)
{
	uint32_t lcd_id;
	int cmd;
	spi_device_handle_t lcd_handle;
	const lcd_init_cmd_t *lcd_init_cmds = NULL;

	//Initialize non-SPI GPIOs
	gpio_pad_select_gpio(lcd_conf->pin_num_dc);
	gpio_set_direction(lcd_conf->pin_num_dc, GPIO_DIRECTION_OUT);

	//Reset the display
	if (lcd_conf->pin_num_rst < GPIO_NUM_MAX) {
		gpio_pad_select_gpio(lcd_conf->pin_num_rst);
		gpio_set_direction(lcd_conf->pin_num_rst, GPIO_DIRECTION_OUT);
		gpio_set_level(lcd_conf->pin_num_rst, (lcd_conf->rst_active_level) & 0x1);
		MSLEEP(100);
		gpio_set_level(lcd_conf->pin_num_rst, (~(lcd_conf->rst_active_level)) & 0x1);
		MSLEEP(100);
	}

	*spi_wr_dev = lcd_handle = up_spiinitialize(lcd_conf->spi_host); //HSPI_PORT

	SPI_SELECT(lcd_handle, CONFIG_ESP32_SPI_CS, true);
	SPI_SETBITS(lcd_handle, 8);
	SPI_SETMODE(lcd_handle, SPIDEV_MODE0);
	SPI_SETFREQUENCY(lcd_handle, lcd_conf->clk_freq);
	lcd_id = lcd_get_id(lcd_handle, dc);

	// Use high speed to write LCD
	SPI_SETFREQUENCY(lcd_handle, lcd_conf->clk_freq);

	if (lcd_conf->lcd_model == LCD_MOD_ST7789) {
		lcd_init_cmds = st7789_init_cmds;
	} else if (lcd_conf->lcd_model == LCD_MOD_ILI9341) {
		lcd_init_cmds = ili_init_cmds;
	} else if (lcd_conf->lcd_model == LCD_MOD_AUTO_DET) {
		if (((lcd_id >> 8) & 0xff) == 0x42) {
			lcd_init_cmds = st7789_init_cmds;
		} else {
			lcd_init_cmds = ili_init_cmds;
		}
	}
	assert(lcd_init_cmds != NULL);
	//Send all the commands
	cmd = 0;
	while (lcd_init_cmds[cmd].databytes != 0xff) {
		lcd_cmd(*spi_wr_dev, lcd_init_cmds[cmd].cmd, dc);
		lcd_data(*spi_wr_dev, lcd_init_cmds[cmd].data, lcd_init_cmds[cmd].databytes & 0x1F, dc);
		if (lcd_init_cmds[cmd].databytes & 0x80) {
			MSLEEP(100);
		}
		cmd++;
	}

	//Enable backlight
	if (lcd_conf->pin_num_bckl < GPIO_NUM_MAX) {
		gpio_pad_select_gpio(lcd_conf->pin_num_bckl);
		gpio_set_direction(lcd_conf->pin_num_bckl, GPIO_DIRECTION_OUT);
		gpio_set_level(lcd_conf->pin_num_bckl, (lcd_conf->bckl_active_level) & 0x1);
	}

	lldbg("[lcd_init] finished.\n");
	return lcd_id;
}

void lcd_send_uint16_r(spi_device_handle_t spi, const uint16_t data, int32_t repeats, lcd_dc_t *dc)
{
	uint32_t i;
	uint32_t word_tmp[16];
	uint32_t word = data << 16 | data;

	while (repeats > 0) {
		uint16_t bytes_to_transfer = MIN(repeats * sizeof(uint16_t), 16 * sizeof(uint32_t));
		for (i = 0; i < (bytes_to_transfer + 3) / 4; i++) {
			word_tmp[i] = word;
		}

		lcd_data(spi, (uint8_t *)word_tmp, bytes_to_transfer, dc);
		repeats -= bytes_to_transfer / 2;
	}
}

uint32_t lcd_get_id(spi_device_handle_t spi, lcd_dc_t *dc)
{
	uint32_t buf = 0;

	lcd_cmd(spi, 0x04, dc);

	SPI_LOCK(spi, true);

	gpio_set_level((int)dc->dc_io, LCD_DATA_LEV);
	SPI_RECVBLOCK(spi, &buf, 4);

	SPI_LOCK(spi, false);
	return buf;
}

