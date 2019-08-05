/******************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
 ******************************************************************/

#ifndef __CONFIGS_ESP32_SRC_LCD_H
#define __CONFIGS_ESP32_SRC_LCD_H

/**
 * @brief struct to map GPIO to LCD pins
 */
typedef struct {
	int8_t pin_num_dc;          /*!<Pin to select Data or Command for LCD*/
	int8_t pin_num_rst;         /*!<Pin to hardreset LCD*/
	int8_t pin_num_bckl;        /*!<Pin for adjusting Backlight- can use PWM/DAC too*/
	int clk_freq;                /*!< spi clock frequency */
	uint8_t rst_active_level;    /*!< reset pin active level */
	uint8_t bckl_active_level;   /*!< back-light active level */
	int spi_host;  /*!< spi host index*/
} lcd_conf_t;

#endif // __CONFIGS_ESP32_SRC_LCD_H

