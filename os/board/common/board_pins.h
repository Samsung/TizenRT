/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
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

/************************************************************************************
 * Included Files
 ************************************************************************************/
/* File name: board_pins.h */

#ifndef BOARD_PINS_H
#define BOARD_PINS_H

#include "PinNames.h"
#include "ameba_pinmux.h"
#include "basic_types.h"
#include "ameba_gpio.h"
#include "ameba_pinmap.h"

#ifdef CONFIG_RTL8730E_BOARD_AIL
#include "rtl8730e_ail_board_pins.h"
#endif 

#ifdef CONFIG_RTL8730E_BOARD_AILP
#include "rtl8730e_ailp_board_pins.h"
#endif

#ifdef CONFIG_RTL8730E_BOARD_AID
#include "rtl8730e_aid_board_pins.h"
#endif

#ifdef CONFIG_RTL8730E_BOARD_AILPW
#include "rtl8730e_ailpw_board_pins.h"
#endif


/* Wakeup pins configuration */
#define		WAKEPIN_0	((u32)0x00000000)	/*!< see aon_wakepin */
#define		WAKEPIN_1	((u32)0x00000001)	/*!< see aon_wakepin */
#define		WAKEPIN_2	((u32)0x00000002)	/*!< see aon_wakepin */
#define		WAKEPIN_3	((u32)0x00000003)	/*!< see aon_wakepin */

#define IS_UART0_TX(tx) ((tx == PA_3) || (tx == PA_14) || (tx == PA_29) || (tx == PB_6))
#define IS_UART1_TX(tx) ((tx == PA_5) || (tx == PA_10) || (tx == PA_25) || (tx == PB_11) || (tx == PB_20) || (tx == PB_30))
#define IS_UART2_TX(tx) ((tx == PA_8) || (tx == PA_12) || (tx == PA_23) || (tx == PB_22))
#define IS_UART4_TX(tx) (tx == PB_24)

// ---------------------
// Pin Attributes
// ---------------------
#define PIN_NONE	0xFF    // marker for unused pins
#define GPIO_PIN_SIZE	3

typedef enum {
	PULL_NONE = 0,
	PULL_UP,
	PULL_DOWN
} pin_pull_t;


typedef struct {
	u16	 		PinName;       // physical pin ID (GPIO Px_y)
	u8 			NormalPull;    // pull-up/down/none
	u8 			SleepPull;    // pull-up/down/none
} pin_desc_t;

typedef enum {
	UART_RX = 0,
	UART_TX,
	UART_CTS,
	UART_RTS
} UART_PinRole_t;

typedef enum {
	I2C_SCL = 0,
	I2C_SDA
} I2C_PinRole_t;

typedef enum {
	I2S_MCLK = 0,
	I2S_SCLK,
	I2S_WS,
	I2S_SD_TX,
	I2S_SD_RX
} I2S_PinRole_t;

typedef enum {
	SPI_MOSI = 0,
	SPI_MISO,
	SPI_SCLK,
	SPI_CS0,
	SPI_CS1
} SPI_PinRole_t;

typedef enum {
	IST415_RESET_PIN = 0,
	IST415_I2C_PIN
} IST415_PinRole_t;

typedef enum {
	LCD_ST7789_RESET = 0,
	LCD_ST7789_BACKLIGHT,
	LCD_ST7789_CMDDATA
} LCD_ST7789_PinRole_t;

typedef enum {
	GPIO_MODE_IN = 0,
	GPIO_MODE_OUT,
	GPIO_MODE
} GPIO_PinRole_t;

//==============================================================================
#define  UART_PIN(INDEX, PIN_ROLE)		pin_table[INDEX##_INDEX + PIN_ROLE].PinName
#define  I2C_PIN(INDEX, PIN_ROLE)		pin_table[INDEX##_INDEX + PIN_ROLE].PinName
#define  SPI_PIN(INDEX, PIN_ROLE)		pin_table[INDEX##_INDEX + PIN_ROLE].PinName
#define  IST415_PIN(INDEX, PIN_ROLE)		pin_table[INDEX##_INDEX + PIN_ROLE].PinName
#define  LCD_ST7789_PIN(INDEX, PIN_ROLE)	pin_table[INDEX##_INDEX + PIN_ROLE].PinName
#define  I2S_PIN(INDEX, PIN_ROLE)		pin_table[INDEX##_INDEX + PIN_ROLE].PinName
#define  GPIO_PINS(INDEX, PIN_ROLE)		pin_table[INDEX##_INDEX + PIN_ROLE].PinName

#define  GPIO_PIN_NORMALPULL(INDEX, PIN_ROLE)	pin_table[INDEX##_INDEX + PIN_ROLE].NormalPull
#define  GPIO_PIN_SLEEPPULL(INDEX, PIN_ROLE)	pin_table[INDEX##_INDEX + PIN_ROLE].SleepPull
extern const pin_desc_t pin_table[];

#endif							// BOARD_PINS_H
