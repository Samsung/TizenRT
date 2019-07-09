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

/* C Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
/* RTOS Includes */
#include <tinyara/config.h>
#include <esp32_spi.h>
/* SPI Includes */
#include "esp_ili9341.h"
#include "spi_lcd.h"

#define ESP_LCD_MISO_GPIO 25
#define ESP_LCD_MOSI_GPIO 23
#define ESP_LCD_CLK_GPIO 19
#define ESP_LCD_CS_GPIO 22

#define ESP_LCD_DC_GPIO 21
#define ESP_LCD_RESET_GPIO 18
#define ESP_LCD_BL_GPIO 5
#define ESP_LCD_SPI_CLOCK 40000000
//HSPI_PORT -> 2
#ifdef CONFIG_ESP_LCD_SPI_PORTNUM
#define ESP_LCD_SPI_NUM CONFIG_ESP_LCD_SPI_PORTNUM
#else
#define ESP_LCD_SPI_NUM 2
#endif
//#define ESP_DISP_ROTATE_0 1
#define ESP_DISP_ROTATE_90 1

#define LCD_HOR_RES          (320)
#define LCD_VER_RES          (240)

#define SWAPBYTES(i) ((i >> 8) | (i << 8))
#define MAKEWORD(b1, b2, b3, b4) ((uint32_t) ((b1) | ((b2) << 8) | ((b3) << 16) | ((b4) << 24)))
//when enabling spiram and use 80M VSPI, it's conflict to use spiram for LCD transmit buffer
#if CONFIG_MM_NHEAPS > 1
#define LCD_MALLOC(size) malloc_at(0, (size))
#else
#define LCD_MALLOC(size) malloc(size)
#endif

typedef int gpio_num_t;

typedef struct {
	spi_device_handle_t spi_wr;
	uint8_t tabcolor;
	bool dma_mode;
	int dma_buf_size;
	uint8_t m_dma_chan;
	uint16_t m_height;
	uint16_t m_width;
	sem_t spi_mux;
	gpio_num_t cmd_io;
	lcd_dc_t dc;
	lcd_id_t id;
} lcd_device_ctl_t;

static lcd_device_ctl_t dev_ctl;

static void _set_spi_bus(lcd_conf_t *lcd_conf)
{
	dev_ctl.cmd_io = (gpio_num_t) lcd_conf->pin_num_dc;
	dev_ctl.dc.dc_io = dev_ctl.cmd_io;
	dev_ctl.id.id = lcd_init(lcd_conf, &dev_ctl.spi_wr, &dev_ctl.dc, dev_ctl.m_dma_chan);
	dev_ctl.id.mfg_id = (dev_ctl.id.id >> (8 * 1)) & 0xff ;
	dev_ctl.id.lcd_driver_id = (dev_ctl.id.id >> (8 * 2)) & 0xff;
	dev_ctl.id.lcd_id = (dev_ctl.id.id >> (8 * 3)) & 0xff;
}

static void _lcd_init(lcd_conf_t *config, int height, int width, bool dma_en, int dma_word_size, int dma_chan)
{
	dev_ctl.m_height = height;
	dev_ctl.m_width = width;
	dev_ctl.tabcolor = 0;
	dev_ctl.dma_mode = dma_en;
	dev_ctl.dma_buf_size = dma_word_size;
	sem_init(&dev_ctl.spi_mux, 0, 1);
	dev_ctl.m_dma_chan = dma_chan;
	_set_spi_bus(config);
}

static void _board_lcd_write_cmd(uint8_t cmd)
{
	sem_wait(&dev_ctl.spi_mux);
	lcd_cmd(dev_ctl.spi_wr, cmd, &dev_ctl.dc);
	sem_post(&dev_ctl.spi_mux);
}

static void _board_lcd_write_data_byte(uint8_t data)
{
	sem_wait(&dev_ctl.spi_mux);
	lcd_data(dev_ctl.spi_wr, (uint8_t *)&data, 1, &dev_ctl.dc);
	sem_post(&dev_ctl.spi_mux);
}

static void _transmit_cmd_data(uint8_t cmd, uint32_t data)
{
	sem_wait(&dev_ctl.spi_mux);
	lcd_cmd(dev_ctl.spi_wr, cmd, &dev_ctl.dc);
	lcd_data(dev_ctl.spi_wr, (uint8_t *)&data, 4, &dev_ctl.dc);
	sem_post(&dev_ctl.spi_mux);
}

static void _transmit_cmd(uint8_t cmd)
{
	sem_wait(&dev_ctl.spi_mux);
	lcd_cmd(dev_ctl.spi_wr, cmd, &dev_ctl.dc);
	sem_post(&dev_ctl.spi_mux);
}

static void _transmit_data(uint8_t *data, int length)
{
	sem_wait(&dev_ctl.spi_mux);
	lcd_data(dev_ctl.spi_wr, (uint8_t *)data, length, &dev_ctl.dc);
	sem_post(&dev_ctl.spi_mux);
}

static void _transmit_16data(uint16_t data, int32_t repeats)
{
	sem_wait(&dev_ctl.spi_mux);
	lcd_send_uint16_r(dev_ctl.spi_wr, data, repeats, &dev_ctl.dc);
	sem_post(&dev_ctl.spi_mux);
}

static void _set_addr_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
	_transmit_cmd_data(LCD_CASET, MAKEWORD(x0 >> 8, x0 & 0xFF, x1 >> 8, x1 & 0xFF));
	_transmit_cmd_data(LCD_PASET, MAKEWORD(y0 >> 8, y0 & 0xFF, y1 >> 8, y1 & 0xFF));
	_transmit_cmd(LCD_RAMWR); // write to RAM
}

static void _fast_send_buff(const uint16_t *buf, int point_num, bool swap)
{
	int gap_point;
	uint16_t *data_buf;
	int offset;
	int trans_points;
	int i;

	if ((point_num * sizeof(uint16_t)) <= (16 * sizeof(uint32_t))) {
		_transmit_data((uint8_t *) buf, sizeof(uint16_t) * point_num);
	} else {
		gap_point = dev_ctl.dma_buf_size;
		data_buf = (uint16_t *) LCD_MALLOC(gap_point * sizeof(uint16_t));
		if (data_buf == NULL) {
			lldbg("[_fast_send_buff]MALLOC FAILED!\n");
			return;
		}

		offset = 0;
		while (point_num > 0) {
			trans_points = point_num > gap_point ? gap_point : point_num;
			if (swap) {
				for (i = 0; i < trans_points; i++) {
					data_buf[i] = SWAPBYTES(buf[i + offset]);
				}
			} else {
				memcpy((uint8_t *) data_buf, (uint8_t *)(buf + offset), trans_points * sizeof(uint16_t));
			}
			_transmit_data((uint8_t *)(data_buf), trans_points * sizeof(uint16_t));
			offset += trans_points;
			point_num -= trans_points;
		}
		free(data_buf);
		data_buf = NULL;
	}
}

static void _fast_send_rep(uint16_t val, int rep_num)
{
	int offset;
	uint16_t *data_buf;
	int trans_points;
	int point_num = rep_num;
	int gap_point = dev_ctl.dma_buf_size;
	gap_point = (gap_point > point_num ? point_num : gap_point);

	data_buf = (uint16_t *) LCD_MALLOC(gap_point * sizeof(uint16_t));
	offset = 0;
	while (point_num > 0) {
		for (int i = 0; i < gap_point; i++) {
			data_buf[i] = val;
		}
		trans_points = point_num > gap_point ? gap_point : point_num;
		_transmit_data((uint8_t *)(data_buf), sizeof(uint16_t) * trans_points);
		offset += trans_points;
		point_num -= trans_points;
	}
	free(data_buf);
	data_buf = NULL;
}

static void _draw_bitmap(int16_t x, int16_t y, const uint16_t *bitmap, int16_t w, int16_t h)
{
	_set_addr_window(x, y, x + w - 1, y + h - 1);
	if (dev_ctl.dma_mode) {
		_fast_send_buff(bitmap, w * h, true);
	} else {
		for (int i = 0; i < w * h; i++) {
			_transmit_16data(SWAPBYTES(bitmap[i]), 1);
		}
	}
}

static void _fill_rect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
	// rudimentary clipping (drawChar w/big text requires this)
	if ((x >= dev_ctl.m_width) || (y >= dev_ctl.m_height)) {
		return;
	}
	if ((x + w - 1) >= dev_ctl.m_width) {
		w = dev_ctl.m_width - x;
	}
	if ((y + h - 1) >= dev_ctl.m_height) {
		h = dev_ctl.m_height - y;
	}

	_set_addr_window(x, y, x + w - 1, y + h - 1);
	if (dev_ctl.dma_mode) {
		_fast_send_rep(SWAPBYTES(color), h * w);
	} else {
		_transmit_16data(SWAPBYTES(color), h * w);
	}
}

/*Write the internal buffer (VDB) to the display. 'lv_flush_ready()' has to be called when finished*/
void esp_lcd_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const uint16_t *color_p)
{
	_draw_bitmap((int16_t)x1, (int16_t)y1, (const uint16_t *)color_p, (int16_t)(x2 - x1 + 1), (int16_t)(y2 - y1 + 1));
}

/*Fill an area with a color on the display*/
void esp_lcd_fill(int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint16_t color)
{
	_fill_rect((int16_t)x1, (int16_t)y1, (int16_t)(x2 - x1 + 1), (int16_t)(y2 - y1 + 1), color);
}

void esp_lcd_set_orientation(uint16_t orientation)
{
	switch (orientation) {
	case 0:
		_board_lcd_write_cmd(ILI9341_MEMACCESS_REG);
		_board_lcd_write_data_byte(0x80 | 0x08);
		break;
	case 90:
		_board_lcd_write_cmd(ILI9341_MEMACCESS_REG);
		_board_lcd_write_data_byte(0x20 | 0x08);
		break;
	case 180:
		_board_lcd_write_cmd(ILI9341_MEMACCESS_REG);
		_board_lcd_write_data_byte(0x40 | 0x08);
		break;
	case 270:
		_board_lcd_write_cmd(ILI9341_MEMACCESS_REG);
		_board_lcd_write_data_byte(0xE0 | 0x08);
		break;
	default:
		break;
	}
}

void up_lcdinitialize(void)
{
	/*Initialize LCD*/
	lcd_conf_t lcd_pins = {
		.lcd_model = LCD_MOD_ILI9341,
		.pin_num_miso = ESP_LCD_MISO_GPIO, //set value to CONFIG_GPIO_SPI2_MISO_PIN
		.pin_num_mosi = ESP_LCD_MOSI_GPIO, //set value to CONFIG_GPIO_SPI2_MOSI_PIN
		.pin_num_clk = ESP_LCD_CLK_GPIO,   //set value to CONFIG_GPIO_SPI2_CLK_PIN
		.pin_num_cs = ESP_LCD_CS_GPIO,     //set value to CONFIG_GPIO_SPI2_CS0_PIN
		.pin_num_dc = ESP_LCD_DC_GPIO,
		.pin_num_rst = ESP_LCD_RESET_GPIO,
		.pin_num_bckl = ESP_LCD_BL_GPIO,
		.clk_freq = ESP_LCD_SPI_CLOCK,
		.rst_active_level = 0,
		.bckl_active_level = 0,//ESP_BCKL_ACTIVE_LEVEL, seems 0 or 1
		.spi_host = ESP_LCD_SPI_NUM, //(spi_host_device_t)ESP_LCD_SPI_NUM, means HSPI, replaced by spi port para ->ESP32  HSPI_PORT
		.init_spi_bus = true,
	};

	if (dev_ctl.m_height == LCD_VER_RES) {
		lldbg("up_lcdinitialize already initialized.\n");
		return;
	}

	_lcd_init(&lcd_pins, LCD_VER_RES, LCD_HOR_RES, true, CONFIG_SPI_DMA_MAX_DATALEN, CONFIG_SPI2_DMA_CHANNEL);

#ifdef ESP_DISP_ROTATE_0
	_board_lcd_write_cmd(ILI9341_MEMACCESS_REG);
	_board_lcd_write_data_byte(0x80 | 0x08);
	lldbg("up_lcdinitialize, ESP_DISP_ROTATE_0\n");
#elif defined(ESP_DISP_ROTATE_90)
	_board_lcd_write_cmd(ILI9341_MEMACCESS_REG);
	_board_lcd_write_data_byte(0x20 | 0x08);
	lldbg("up_lcdinitialize, ESP_DISP_ROTATE_90\n");
#elif defined(ESP_DISP_ROTATE_180)
	_board_lcd_write_cmd(ILI9341_MEMACCESS_REG);
	_board_lcd_write_data_byte(0x40 | 0x08);
	lldbg("up_lcdinitialize, ESP_DISP_ROTATE_180\n");
#elif defined(ESP_DISP_ROTATE_270)
	_board_lcd_write_cmd(ILI9341_MEMACCESS_REG);
	_board_lcd_write_data_byte(0xE0 | 0x08);
	lldbg("up_lcdinitialize, ESP_DISP_ROTATE_270\n");
#endif

	return;
}

