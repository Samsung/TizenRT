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

#ifndef _ESP_SPI_LCD_H_
#define _ESP_SPI_LCD_H_

#include <string.h>
#include <stdio.h>
#include <tinyara/spi/spi.h>
#include <tinyara/gpio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GPIO_NUM_MAX 40

typedef enum {
	LCD_MOD_ILI9341 = 0,
	LCD_MOD_ST7789 = 1,
	LCD_MOD_AUTO_DET = 3,
} lcd_model_t;

/**
 * @brief struct to map GPIO to LCD pins
 */
typedef struct {
	lcd_model_t lcd_model;
	int8_t pin_num_miso;        /*!<MasterIn, SlaveOut pin*/
	int8_t pin_num_mosi;        /*!<MasterOut, SlaveIn pin*/
	int8_t pin_num_clk;         /*!<SPI Clock pin*/
	int8_t pin_num_cs;          /*!<SPI Chip Select Pin*/
	int8_t pin_num_dc;          /*!<Pin to select Data or Command for LCD*/
	int8_t pin_num_rst;         /*!<Pin to hardreset LCD*/
	int8_t pin_num_bckl;        /*!<Pin for adjusting Backlight- can use PWM/DAC too*/
	int clk_freq;                /*!< spi clock frequency */
	uint8_t rst_active_level;    /*!< reset pin active level */
	uint8_t bckl_active_level;   /*!< back-light active level */
	int spi_host;  /*!< spi host index*/
	bool init_spi_bus; //
} lcd_conf_t;

typedef struct {
	uint8_t dc_io;
	uint8_t dc_level;
} lcd_dc_t;

/**
 * @brief struct holding LCD IDs
 */
typedef struct {
	uint8_t mfg_id;         /*!<Manufacturer's ID*/
	uint8_t lcd_driver_id;  /*!<LCD driver Version ID*/
	uint8_t lcd_id;         /*!<LCD Unique ID*/
	uint32_t id;
} lcd_id_t;

typedef struct spi_dev_s *spi_device_handle_t;

/** @brief Initialize the LCD by putting some data in the graphics registers
 *
 * @param pin_conf Pointer to the struct with mandatory pins required for the LCD
 * @param spi_wr_dev Pointer to the SPI handler for sending the data
 * @return lcd id
 */
uint32_t lcd_init(lcd_conf_t *lcd_conf, spi_device_handle_t *spi_wr_dev, lcd_dc_t *dc, int dma_chan);

/*Used by adafruit functions to send data*/
void lcd_send_uint16_r(spi_device_handle_t spi, const uint16_t data, int32_t repeats, lcd_dc_t *dc);

/*Send a command to the ILI9341. Uses spi_device_transmit,
 which waits until the transfer is complete */
void lcd_cmd(spi_device_handle_t spi, const uint8_t cmd, lcd_dc_t *dc);

/*Send data to the ILI9341. Uses spi_device_transmit,
 which waits until the transfer is complete */
void lcd_data(spi_device_handle_t spi, const uint8_t *data, int len, lcd_dc_t *dc);

/**
 * @brief get LCD ID
 */
uint32_t lcd_get_id(spi_device_handle_t spi, lcd_dc_t *dc);

#ifdef __cplusplus
}
#endif
#endif /*_ESP_SPI_LCD_H_*/

