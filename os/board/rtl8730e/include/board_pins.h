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
#define PIN_NONE    0xFF    // marker for unused pins

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
//========================================UART=======================================
#define		UART0_INDEX			0	
#ifdef CONFIG_RTL8730E_UART0
#define		NUM_OF_PINS_UART0		2
#else
#define		NUM_OF_PINS_UART0		0
#endif

#define		UART1_INDEX			(UART0_INDEX +  NUM_OF_PINS_UART0)

#ifdef CONFIG_RTL8730E_UART1
#if defined(CONFIG_RTL8730E_BOARD_AIL) || defined(CONFIG_RTL8730E_BOARD_AILP) || defined(CONFIG_RTL8730E_BOARD_AILPW) || defined(CONFIG_RTL8730E_BOARD_AID)
#define		NUM_OF_PINS_UART1		2
#else
#error Not Supported, Please check the board type configure
#endif
#else
#define		NUM_OF_PINS_UART1		0
#endif

#define		UART2_INDEX			(UART1_INDEX +  NUM_OF_PINS_UART1)

#ifdef CONFIG_RTL8730E_UART2
#define		NUM_OF_PINS_UART2		4
#else
#define		NUM_OF_PINS_UART2		0
#endif

#define		UART4_INDEX			(UART2_INDEX +  NUM_OF_PINS_UART2)

#ifdef CONFIG_RTL8730E_UART4
#define 	NUM_OF_PINS_UART4		2
#else
#define 	NUM_OF_PINS_UART4		0
#endif

#define		UART_NEXT			(UART4_INDEX + NUM_OF_PINS_UART4)

//========================================I2C=======================================
#define                I2C0_INDEX                      UART_NEXT

#ifdef CONFIG_AMEBASMART_I2C

#ifdef CONFIG_AMEBASMART_I2C0
#if defined(CONFIG_RTL8730E_BOARD_AIL) || defined(CONFIG_RTL8730E_BOARD_AILP) || defined(CONFIG_RTL8730E_BOARD_AILPW)
#define                NUM_OF_PINS_I2C0                2
#else
#error Not Supported, Please check the board type configure
#endif
#else
#define                NUM_OF_PINS_I2C0                0
#endif

#define                I2C1_INDEX                      (I2C0_INDEX + NUM_OF_PINS_I2C0)

#ifdef CONFIG_AMEBASMART_I2C1
#if defined(CONFIG_RTL8730E_BOARD_AIL) || defined(CONFIG_RTL8730E_BOARD_AILP) || defined(CONFIG_RTL8730E_BOARD_AILPW)
#define                NUM_OF_PINS_I2C1                2
#else
#error Not Supported, Please check the board type configure
#endif
#else
#define                NUM_OF_PINS_I2C1                0
#endif

#define                I2C2_INDEX                      (I2C1_INDEX + NUM_OF_PINS_I2C1)

#ifdef CONFIG_AMEBASMART_I2C2
#define                NUM_OF_PINS_I2C2                2
#else
#define                NUM_OF_PINS_I2C2                0
#endif

#define                I2C_NEXT                        (I2C2_INDEX + NUM_OF_PINS_I2C2)

#else
#define                I2C_NEXT                        UART_NEXT

#endif                                                 // CONFIG_AMEBASMART_I2C

//=====================================SPI=========================================
#define		SPI0_INDEX			I2C_NEXT

#ifdef CONFIG_AMEBASMART_SPI0
#if defined(CONFIG_SPI_CS) && defined(CONFIG_AMEBASMART_SPI0_CS)
#define		NUM_OF_PINS_SPI0		5
#else
#define		NUM_OF_PINS_SPI0		4
#endif
#else
#define		NUM_OF_PINS_SPI0		0
#endif

#define		SPI1_INDEX			(SPI0_INDEX + NUM_OF_PINS_SPI0)

#ifdef CONFIG_AMEBASMART_SPI1
#if defined(CONFIG_SPI_CS) && defined(CONFIG_AMEBASMART_SPI1_CS)
#define		NUM_OF_PINS_SPI1		5
#else
#define		NUM_OF_PINS_SPI1		4
#endif
#else
#define		NUM_OF_PINS_SPI1		0
#endif

#define		SPI_NEXT			(SPI1_INDEX + NUM_OF_PINS_SPI1)

//=====================================IST415=========================================
#define		IST415_INDEX			SPI_NEXT

#if defined(CONFIG_TOUCH_IST415)
#if defined(CONFIG_RTL8730E_BOARD_AIL) || defined(CONFIG_RTL8730E_BOARD_AILP) || defined(CONFIG_RTL8730E_BOARD_AILPW)
#define		NUM_OF_PINS_IST415		2
#else
#error Not Supported, Please check the board type configure
#endif
#else
#define		NUM_OF_PINS_IST415		0
#endif

#define		IST415_NEXT			(IST415_INDEX + NUM_OF_PINS_IST415)

//=====================================LCD_ST7789=========================================
#define		LCD_ST7789_INDEX		IST415_NEXT

#ifdef CONFIG_LCD_ST7789
#if defined(CONFIG_RTL8730E_BOARD_AIL) || defined(CONFIG_RTL8730E_BOARD_AILP) || defined(CONFIG_RTL8730E_BOARD_AILPW)
#error  Not supported revision
#else
#define		NUM_OF_PINS_LCD_ST7789		3
#endif
#else
#define		NUM_OF_PINS_LCD_ST7789		0
#endif

#define		LCD_ST7789_NEXT			(LCD_ST7789_INDEX + NUM_OF_PINS_LCD_ST7789)

//=====================================I2S=========================================
/* I2S2 pins configuration */

#define		I2S2_INDEX			LCD_ST7789_NEXT

#ifdef CONFIG_AMEBASMART_I2S
#ifdef CONFIG_AMEBASMART_I2S2
#define		NUM_OF_PINS_I2S2		5
#else
#define		NUM_OF_PINS_I2S2		0
#endif

#define		I2S3_INDEX			(I2S2_INDEX + NUM_OF_PINS_I2S2)

#ifdef CONFIG_AMEBASMART_I2S3
#define		NUM_OF_PINS_I2S3		5
#else
#define		NUM_OF_PINS_I2S3		0
#endif

#define		I2S_NEXT			(I2S3_INDEX + NUM_OF_PINS_I2S3)
#else
#define		I2S_NEXT			I2S2_INDEX
#endif

//=====================================GPIO=========================================
#define		GPIO_INDEX			I2S_NEXT
#define		NUM_OF_PINS_GPIO		3
#define		GPIO_NEXT			(GPIO_INDEX + NUM_OF_PINS_GPIO)

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
