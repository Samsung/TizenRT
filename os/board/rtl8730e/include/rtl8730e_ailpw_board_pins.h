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
/* File name: rtl8730e_ailpw_board_pins.h */

#ifndef RTL8730E_AILPW_BOARD_PINS_H
#define RTL8730E_AILPW_BOARD_PINS_H

//========================================UART=======================================
#define		UART0_INDEX			0
#ifdef CONFIG_RTL8730E_UART0
#define		NUM_OF_PINS_UART0		2
#else
#define		NUM_OF_PINS_UART0		0
#endif

#define		UART1_INDEX			(UART0_INDEX +  NUM_OF_PINS_UART0)

#ifdef CONFIG_RTL8730E_UART1
#define		NUM_OF_PINS_UART1		2
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
#define		I2C0_INDEX                      UART_NEXT

#ifdef CONFIG_AMEBASMART_I2C

#ifdef CONFIG_AMEBASMART_I2C0
#define		NUM_OF_PINS_I2C0                2
#else
#define		NUM_OF_PINS_I2C0                0
#endif

#define		I2C1_INDEX                      (I2C0_INDEX + NUM_OF_PINS_I2C0)

#ifdef CONFIG_AMEBASMART_I2C1
#define		NUM_OF_PINS_I2C1                2
#else
#define		NUM_OF_PINS_I2C1                0
#endif

#define		I2C2_INDEX                      (I2C1_INDEX + NUM_OF_PINS_I2C1)

#ifdef CONFIG_AMEBASMART_I2C2
#define		NUM_OF_PINS_I2C2                2
#else
#define		NUM_OF_PINS_I2C2                0
#endif

#define		I2C_NEXT                        (I2C2_INDEX + NUM_OF_PINS_I2C2)

#else
#define		I2C_NEXT                        UART_NEXT

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
#define		NUM_OF_PINS_IST415		2
#else
#define		NUM_OF_PINS_IST415		0
#endif

#define		IST415_NEXT			(IST415_INDEX + NUM_OF_PINS_IST415)

//=====================================LCD_ST7789=========================================
#define		LCD_ST7789_INDEX		IST415_NEXT

#ifdef CONFIG_LCD_ST7789
#error  Not supported revision
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

#endif							// RTL8730E_AIL_BOARD_PINS_H
