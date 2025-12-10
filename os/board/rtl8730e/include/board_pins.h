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

/* UART pins configuration */
#ifdef CONFIG_RTL8730E_UART0
#define		UART0_TX	PA_3
#define		UART0_RX	PA_2
#endif

#ifdef CONFIG_RTL8730E_UART1
#if defined(CONFIG_RTL8730E_BOARD_AIL) || defined(CONFIG_RTL8730E_BOARD_AILP) || defined(CONFIG_RTL8730E_BOARD_AILPW)
#define		UART1_TX	PA_10
#define		UART1_RX	PA_9
#elif defined(CONFIG_RTL8730E_BOARD_AID)
#define		UART1_TX	PA_5
#define		UART1_RX	PA_4
#else
#error Not Supported, Please check the board type configure
#endif
#endif							// CONFIG_RTL8730E_UART1

#ifdef CONFIG_RTL8730E_UART2
#define		UART2_TX	PB_22
#define		UART2_RX	PB_21
#define		UART2_RTS	PB_20
#define		UART2_CTS	PB_19
#endif

#ifdef CONFIG_RTL8730E_UART4
#define		UART4_TX	PB_24
#define		UART4_RX	PB_23
#endif

/* SPI pins configuration */
#ifdef CONFIG_AMEBASMART_SPI
#ifdef CONFIG_AMEBASMART_SPI0
#define		SPI0_MOSI	PB_4
#define		SPI0_MISO	PB_3
#define		SPI0_SCLK	PB_6
#define		SPI0_CS0	PB_5
#if defined(CONFIG_SPI_CS) && defined(CONFIG_AMEBASMART_SPI0_CS)
#define		SPI0_CS1	PB_31
#endif
#endif							// CONFIG_AMEBASMART_SPI0

#ifdef CONFIG_AMEBASMART_SPI1
#define		SPI1_MOSI	PB_28
#define		SPI1_MISO	PB_27
#define		SPI1_SCLK	PB_25
#define		SPI1_CS0	PB_26
#if defined(CONFIG_SPI_CS) && defined(CONFIG_AMEBASMART_SPI0_CS)
#define		SPI1_CS1	PB_29
#endif
#endif							// CONFIG_AMEBASMART_SPI1
#endif							// CONFIG_AMEBASMART_SPI

/* I2C pins configuration */
#ifdef CONFIG_AMEBASMART_I2C
#ifdef CONFIG_AMEBASMART_I2C0
#if defined(CONFIG_RTL8730E_BOARD_AIL) || defined(CONFIG_RTL8730E_BOARD_AILP) || defined(CONFIG_RTL8730E_BOARD_AILPW)
#define		I2C0_SCL	PB_30
#define		I2C0_SDA	PB_29
#else
#error Not Supported, Please check the board type configure
#endif
#endif							// CONFIG_AMEBASMART_I2C0

#ifdef CONFIG_AMEBASMART_I2C1
#if defined(CONFIG_RTL8730E_BOARD_AIL) || defined(CONFIG_RTL8730E_BOARD_AILP) || defined(CONFIG_RTL8730E_BOARD_AILPW)
#define		I2C1_SCL	PA_4
#define		I2C1_SDA	PA_3
#else
#error Not Supported, Please check the board type configure
#endif
#endif							// CONFIG_AMEBASMART_I2C1

#ifdef CONFIG_AMEBASMART_I2C2
#define		I2C2_SCL	PA_29
#define		I2C2_SDA	PA_28
#endif							// CONFIG_AMEBASMART_I2C2
#endif							// CONFIG_AMEBASMART_I2C

/* IST415 pins configuration */
#if defined(CONFIG_TOUCH_IST415)
#if defined(CONFIG_RTL8730E_BOARD_AIL) || defined(CONFIG_RTL8730E_BOARD_AILP) || defined(CONFIG_RTL8730E_BOARD_AILPW)
#define		IST415_GPIO_RESET_PIN	PA_5
#define		IST415_GPIO_I2C_PIN	PA_4
#else
#error Not Supported, Please check the board type configure
#endif
#endif

/* GPIO pins configuration */
#define 	GPIO_PIN_LIST   { 						\
					{PA_23, PIN_INPUT, PullDown }, 		\
										\
					/* PB_20 is gpio pin number for LED */ 	\
					{PB_20, PIN_OUTPUT, PullDown },		\
					{PB_22, PIN_INPUT, PullUp } 		\
				}

/* Power control pins configuration */
#if defined(CONFIG_RTL8730E_BOARD_AIL) || defined(CONFIG_RTL8730E_BOARD_AILP) || defined(CONFIG_RTL8730E_BOARD_AILPW)
#define         PWR_PIN_00	{_PA_0,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// RTL8730ELH
#define         PWR_PIN_01	{_PA_1,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// RTL8730ELH
#define         PWR_PIN_02	{_PA_2,			GPIO_PuPd_UP,			GPIO_PuPd_UP}	// SDA pull up 10k
#define         PWR_PIN_03	{_PA_3,			GPIO_PuPd_UP,			GPIO_PuPd_UP}	// SDA pull up 10k
#define         PWR_PIN_04	{_PA_4,			GPIO_PuPd_UP,			GPIO_PuPd_UP}	// SDA pull up 4.7k
#define         PWR_PIN_05	{_PA_5,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// SDA pull up 4.7k & pull down 9.1k
#define         PWR_PIN_06	{_PA_6,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// RTL8730ELH
#define         PWR_PIN_07	{_PA_7,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// RTL8730ELH
#define         PWR_PIN_08	{_PA_8,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// RTL8730ELH
#define         PWR_PIN_09	{_PA_9,			GPIO_PuPd_UP,			GPIO_PuPd_UP}	// SDA pull up 10k
#define         PWR_PIN_10	{_PA_10,		GPIO_PuPd_UP,			GPIO_PuPd_UP}	// SDA pull up 10k
#define         PWR_PIN_11	{_PA_11,		GPIO_PuPd_UP,			GPIO_PuPd_DOWN}	// RTL8730ELH
#define         PWR_PIN_12	{_PA_12,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// RTL8730ELH
#define         PWR_PIN_13	{_PA_13,		GPIO_PuPd_UP,			GPIO_PuPd_UP}	// RTK SWD_DATA
#define         PWR_PIN_14	{_PA_14,		GPIO_PuPd_UP,			GPIO_PuPd_UP}	// RTK SWD_CLK
#define         PWR_PIN_15	{_PA_15,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// SDA pull down 4.7k
#define         PWR_PIN_16	{_PA_16,		GPIO_PuPd_NOPULL,		GPIO_PuPd_NOPULL}	// RTK QA board need pull up
#define         PWR_PIN_17	{_PA_17,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// NA
#define         PWR_PIN_18	{_PA_18,		GPIO_PuPd_UP,			GPIO_PuPd_DOWN}	// NA
#define         PWR_PIN_19	{_PA_19,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// NA
#define         PWR_PIN_20	{_PA_20,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// NA
#define         PWR_PIN_21	{_PA_21,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// NA
#define         PWR_PIN_22	{_PA_22,		GPIO_PuPd_NOPULL,		GPIO_PuPd_NOPULL}	// SDA AMP-RESET
#define         PWR_PIN_23	{_PA_23,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// SDA pull down 4.7k
#define         PWR_PIN_24	{_PA_24,		GPIO_PuPd_UP,			GPIO_PuPd_UP}	// SDA pull up 100k
#define         PWR_PIN_25	{_PA_25,		GPIO_PuPd_UP,			GPIO_PuPd_UP}	// SDA pull up 10k
#define         PWR_PIN_26	{_PA_26,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// RTL8730ELH
#define         PWR_PIN_27	{_PA_27,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// RTL8730ELH
#define         PWR_PIN_28	{_PA_28,		GPIO_PuPd_UP,			GPIO_PuPd_UP}	// SDA pull up 4.7k
#define         PWR_PIN_29	{_PA_29,		GPIO_PuPd_UP,			GPIO_PuPd_UP}	// SDA pull up 4.7k
#define         PWR_PIN_30	{_PA_30,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// NA
#define         PWR_PIN_31	{_PA_31,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// NA
#define         PWR_PIN_32	{_PB_0,			GPIO_PuPd_UP,			GPIO_PuPd_DOWN}	// RTL8730ELH
#define         PWR_PIN_33	{_PB_1,			GPIO_PuPd_UP,			GPIO_PuPd_DOWN}	// RTL8730ELH
#define         PWR_PIN_34	{_PB_2,			GPIO_PuPd_UP,			GPIO_PuPd_DOWN}	// NA
#define         PWR_PIN_35	{_PB_3,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// SDA NPU SPI0_MISO
#define         PWR_PIN_36	{_PB_4,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// SDA NPU SPI0_MOSI
#define         PWR_PIN_37	{_PB_5,			GPIO_PuPd_UP,			GPIO_PuPd_UP}	// SDA NPU SPI0_CS pull up 4.7k
#define         PWR_PIN_38	{_PB_6,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// SDA NPU SPI0_CLK
#define         PWR_PIN_39	{_PB_7,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// RTL8730ELH
#define         PWR_PIN_40	{_PB_8,			GPIO_PuPd_UP,			GPIO_PuPd_DOWN}	// RTL8730ELH
#define         PWR_PIN_41	{_PB_9,			GPIO_PuPd_UP,			GPIO_PuPd_DOWN}	// RTL8730ELH
#define         PWR_PIN_42	{_PB_10,		GPIO_PuPd_NOPULL,		GPIO_PuPd_NOPULL}	// SDA AMP-I2S
#if defined(CONFIG_LCD_PWM_INVERSION)
#define PWR_PIN_43 {_PB_11, GPIO_PuPd_UP, GPIO_PuPd_UP} // SDA AILite+W LCD PWN
#else
#define PWR_PIN_43 {_PB_11, GPIO_PuPd_DOWN, GPIO_PuPd_DOWN} // SDA AILite+W LCD PWN
#endif
#define         PWR_PIN_44	{_PB_12,		GPIO_PuPd_UP,			GPIO_PuPd_DOWN}	// RTL8730ELH
#define         PWR_PIN_45	{_PB_13,		GPIO_PuPd_UP,			GPIO_PuPd_DOWN}	// RTL8730ELH
#define         PWR_PIN_46	{_PB_14,		GPIO_PuPd_UP,			GPIO_PuPd_DOWN}	// RTL8730ELH
#define         PWR_PIN_47	{_PB_15,		GPIO_PuPd_UP,			GPIO_PuPd_DOWN}	// RTL8730ELH
#define         PWR_PIN_48	{_PB_16,		GPIO_PuPd_UP,			GPIO_PuPd_DOWN}	// RTL8730ELH
#define         PWR_PIN_49	{_PB_17,		GPIO_PuPd_UP,			GPIO_PuPd_UP}	// RTL8730ELH
#define         PWR_PIN_50	{_PB_18,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// RTL8730ELH
#define         PWR_PIN_51	{_PB_19,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// NA
#define         PWR_PIN_52	{_PB_20,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// SDA pull down 10k
#define         PWR_PIN_53	{_PB_21,		GPIO_PuPd_NOPULL,		GPIO_PuPd_NOPULL}	// SDA AMP-I2S
#define         PWR_PIN_54	{_PB_22,		GPIO_PuPd_UP,			GPIO_PuPd_UP}	// SDA pull up 10k; RTK TM_DIS
#define         PWR_PIN_55	{_PB_23,		GPIO_PuPd_UP,			GPIO_PuPd_UP}	// RTK UART_LOG_RXD
#define         PWR_PIN_56	{_PB_24,		GPIO_PuPd_UP,			GPIO_PuPd_UP}	// RTK UART_LOG_TXD internal pull up 80k
#define         PWR_PIN_57	{_PB_25,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// SDA Ext-Flash SPI1_CLK
#define         PWR_PIN_58	{_PB_26,		GPIO_PuPd_UP,			GPIO_PuPd_UP}	// SDA Ext-Flash SPI1_CS pull up 4.7k
#define         PWR_PIN_59	{_PB_27,		GPIO_PuPd_UP,			GPIO_PuPd_UP}	// SDA Ext-Flash SPI1_MISO
#define         PWR_PIN_60	{_PB_28,		GPIO_PuPd_UP,			GPIO_PuPd_UP}	// SDA Ext-Flash SPI0_MOSI
#define         PWR_PIN_61	{_PB_29,		GPIO_PuPd_NOPULL,		GPIO_PuPd_NOPULL}	// SDA pull up 4.7k
#define         PWR_PIN_62	{_PB_30,		GPIO_PuPd_NOPULL,		GPIO_PuPd_NOPULL}	// SDA pull up 4.7k
#define         PWR_PIN_63	{_PB_31,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// SDA pull down 4.7k
#define         PWR_PIN_64	{_PC_0,			GPIO_PuPd_UP,			GPIO_PuPd_UP}	// SDA pull up 4.7k
#define         PWR_PIN_65	{_PC_1,			GPIO_PuPd_UP,			GPIO_PuPd_UP}	// RTK Int-Flash SPI_FLASH_DATA
#define         PWR_PIN_66	{_PC_2,			GPIO_PuPd_UP,			GPIO_PuPd_DOWN}	// RTK Int-Flash SPI_FLASH_CLK pull down for PG or 50uA
#define         PWR_PIN_67	{_PC_3,			GPIO_PuPd_UP,			GPIO_PuPd_UP}	// RTK Int-Flash SPI_FLASH_DATA
#define         PWR_PIN_68	{_PC_4,			GPIO_PuPd_UP,			GPIO_PuPd_UP}	// RTK Int-Flash SPI_FLASH_DATA
#define         PWR_PIN_69	{_PC_5,			GPIO_PuPd_UP,			GPIO_PuPd_UP}	// RTK Int-Flash SPI_FLASH_DATA
#define         PWR_PIN_70	{_PC_6,			GPIO_PuPd_UP,			GPIO_PuPd_UP}	// RTK Int-Flash SPI_FLASH_CS
#define         PWR_PIN_71	{_PNC,			GPIO_PuPd_KEEP,			GPIO_PuPd_KEEP}	// table end
#elif defined(CONFIG_RTL8730E_BOARD_AID)
#define         PWR_PIN_00	{_PA_0,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// RTL8730ELH
#define         PWR_PIN_01	{_PA_1,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// RTL8730ELH
#define         PWR_PIN_02	{_PA_2,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// SDA pull up 10k
#define         PWR_PIN_03	{_PA_3,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// SDA pull up 10k
#define         PWR_PIN_04	{_PA_4,			GPIO_PuPd_UP,			GPIO_PuPd_UP}	// SDA pull up 4.7k
#define         PWR_PIN_05	{_PA_5,			GPIO_PuPd_UP,			GPIO_PuPd_UP}	// SDA pull up 4.7k & pull down 9.1k
#define         PWR_PIN_06	{_PA_6,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// RTL8730ELH
#define         PWR_PIN_07	{_PA_7,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// RTL8730ELH
#define         PWR_PIN_08	{_PA_8,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// RTL8730ELH
#define         PWR_PIN_09	{_PA_9,			GPIO_PuPd_NOPULL,		GPIO_PuPd_NOPULL}	// SDA pull up 10k
#define         PWR_PIN_10	{_PA_10,		GPIO_PuPd_NOPULL,		GPIO_PuPd_NOPULL}	// SDA pull up 10k
#define         PWR_PIN_11	{_PA_11,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// RTL8730ELH
#define         PWR_PIN_12	{_PA_12,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// RTL8730ELH
#define         PWR_PIN_13	{_PA_13,		GPIO_PuPd_UP,			GPIO_PuPd_UP}	// RTK SWD_DATA
#define         PWR_PIN_14	{_PA_14,		GPIO_PuPd_UP,			GPIO_PuPd_UP}	// RTK SWD_CLK
#define         PWR_PIN_15	{_PA_15,		GPIO_PuPd_NOPULL,		GPIO_PuPd_NOPULL}	// SDA pull down 4.7k
#define         PWR_PIN_16	{_PA_16,		GPIO_PuPd_NOPULL,		GPIO_PuPd_NOPULL}	// RTK QA board need pull up
#define         PWR_PIN_17	{_PA_17,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// NA
#define         PWR_PIN_18	{_PA_18,		GPIO_PuPd_UP,			GPIO_PuPd_DOWN}	// NA
#define         PWR_PIN_19	{_PA_19,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// NA
#define         PWR_PIN_20	{_PA_20,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// NA
#define         PWR_PIN_21	{_PA_21,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// NA
#define         PWR_PIN_22	{_PA_22,		GPIO_PuPd_NOPULL,		GPIO_PuPd_NOPULL}	// SDA AMP-RESET
#define         PWR_PIN_23	{_PA_23,		GPIO_PuPd_NOPULL,		GPIO_PuPd_NOPULL}	// SDA pull down 4.7k
#define         PWR_PIN_24	{_PA_24,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// SDA pull up 100k
#define         PWR_PIN_25	{_PA_25,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// SDA pull up 10k
#define         PWR_PIN_26	{_PA_26,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// RTL8730ELH
#define         PWR_PIN_27	{_PA_27,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// RTL8730ELH
#define         PWR_PIN_28	{_PA_28,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// SDA pull up 4.7k
#define         PWR_PIN_29	{_PA_29,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// SDA pull up 4.7k
#define         PWR_PIN_30	{_PA_30,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// NA
#define         PWR_PIN_31	{_PA_31,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// NA
#define         PWR_PIN_32	{_PB_0,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// RTL8730ELH
#define         PWR_PIN_33	{_PB_1,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// RTL8730ELH
#define         PWR_PIN_34	{_PB_2,			GPIO_PuPd_UP,			GPIO_PuPd_DOWN}	// NA
#define         PWR_PIN_35	{_PB_3,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// SDA NPU SPI0_MISO
#define         PWR_PIN_36	{_PB_4,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// SDA NPU SPI0_MOSI
#define         PWR_PIN_37	{_PB_5,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// SDA NPU SPI0_CS pull up 4.7k
#define         PWR_PIN_38	{_PB_6,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// SDA NPU SPI0_CLK
#define         PWR_PIN_39	{_PB_7,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// RTL8730ELH
#define         PWR_PIN_40	{_PB_8,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// RTL8730ELH
#define         PWR_PIN_41	{_PB_9,			GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// RTL8730ELH
#define         PWR_PIN_42	{_PB_10,		GPIO_PuPd_UP,			GPIO_PuPd_UP}	// SDA AMP-I2S
#define         PWR_PIN_43	{_PB_11,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}
#define         PWR_PIN_44	{_PB_12,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// RTL8730ELH
#define         PWR_PIN_45	{_PB_13,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// RTL8730ELH
#define         PWR_PIN_46	{_PB_14,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// RTL8730ELH
#define         PWR_PIN_47	{_PB_15,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// RTL8730ELH
#define         PWR_PIN_48	{_PB_16,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// RTL8730ELH
#define         PWR_PIN_49	{_PB_17,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// RTL8730ELH
#define         PWR_PIN_50	{_PB_18,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// RTL8730ELH
#define         PWR_PIN_51	{_PB_19,		GPIO_PuPd_NOPULL,		GPIO_PuPd_NOPULL}	// NA
#define         PWR_PIN_52	{_PB_20,		GPIO_PuPd_NOPULL,		GPIO_PuPd_NOPULL}	// SDA pull down 10k
#define         PWR_PIN_53	{_PB_21,		GPIO_PuPd_NOPULL,		GPIO_PuPd_NOPULL}	// SDA AMP-I2S
#define         PWR_PIN_54	{_PB_22,		GPIO_PuPd_NOPULL,		GPIO_PuPd_NOPULL}	// SDA pull up 10k; RTK TM_DIS
#define         PWR_PIN_55	{_PB_23,		GPIO_PuPd_UP,			GPIO_PuPd_UP}	// RTK UART_LOG_RXD
#define         PWR_PIN_56	{_PB_24,		GPIO_PuPd_UP,			GPIO_PuPd_UP}	// RTK UART_LOG_TXD internal pull up 80k
#define         PWR_PIN_57	{_PB_25,		GPIO_PuPd_NOPULL,		GPIO_PuPd_NOPULL}	// SDA Ext-Flash SPI1_CLK
#define         PWR_PIN_58	{_PB_26,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// SDA Ext-Flash SPI1_CS pull up 4.7k
#define         PWR_PIN_59	{_PB_27,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// SDA Ext-Flash SPI1_MISO
#define         PWR_PIN_60	{_PB_28,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// SDA Ext-Flash SPI0_MOSI
#define         PWR_PIN_61	{_PB_29,		GPIO_PuPd_NOPULL,		GPIO_PuPd_NOPULL}	// SDA pull up 4.7k
#define         PWR_PIN_62	{_PB_30,		GPIO_PuPd_NOPULL,		GPIO_PuPd_NOPULL}	// SDA pull up 4.7k
#define         PWR_PIN_63	{_PB_31,		GPIO_PuPd_DOWN,			GPIO_PuPd_DOWN}	// SDA pull down 4.7k
#define         PWR_PIN_64	{_PC_0,			GPIO_PuPd_UP,			GPIO_PuPd_UP}	// SDA pull up 4.7k
#define         PWR_PIN_65	{_PC_1,			GPIO_PuPd_UP,			GPIO_PuPd_UP}	// RTK Int-Flash SPI_FLASH_DATA
#define         PWR_PIN_66	{_PC_2,			GPIO_PuPd_UP,			GPIO_PuPd_DOWN}	// RTK Int-Flash SPI_FLASH_CLK pull down for PG or 50uA
#define         PWR_PIN_67	{_PC_3,			GPIO_PuPd_UP,			GPIO_PuPd_UP}	// RTK Int-Flash SPI_FLASH_DATA
#define         PWR_PIN_68	{_PC_4,			GPIO_PuPd_UP,			GPIO_PuPd_UP}	// RTK Int-Flash SPI_FLASH_DATA
#define         PWR_PIN_69	{_PC_5,			GPIO_PuPd_UP,			GPIO_PuPd_UP}	// RTK Int-Flash SPI_FLASH_DATA
#define         PWR_PIN_70	{_PC_6,			GPIO_PuPd_UP,			GPIO_PuPd_UP}	// RTK Int-Flash SPI_FLASH_CS
#define         PWR_PIN_71	{_PNC,			GPIO_PuPd_KEEP,			GPIO_PuPd_KEEP}	// table end
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

/* I2S2 pins configuration */
#ifdef CONFIG_AMEBASMART_I2S
#ifdef CONFIG_AMEBASMART_I2S2
#define		I2S2_MCLK	NULL
#define		I2S2_SCLK	PB_21
#define		I2S2_WS		PA_16
#define		I2S2_SD_TX	PB_10
#define		I2S2_SD_RX	PB_19
#endif							// CONFIG_AMEBASMART_I2S2

#ifdef CONFIG_AMEBASMART_I2S3
#define		I2S3_MCLK	PA_15
#define		I2S3_SCLK	PA_14
#define		I2S3_WS		PA_13
#define		I2S3_SD_TX	PB_11
#define		I2S3_SD_RX	PB_20
#endif							// CONFIG_AMEBASMART_I2S3
#endif							// CONFIG_AMEBASMART_I2S

/* LCD ST7789 pins configuration */
#ifdef CONFIG_LCD_ST7789
#if defined(CONFIG_RTL8730E_BOARD_AIL) || defined(CONFIG_RTL8730E_BOARD_AILP) || defined(CONFIG_RTL8730E_BOARD_AILPW)
#error "ERROR, Not supported this revision"
#else
#define		LCD_ST7789_GPIO_PIN_RESET	PB_11
#define		LCD_ST7789_GPIO_PIN_BACKLIGHT	PA_9
#define		LCD_ST7789_GPIO_PIN_CMDDATA	PA_10
#endif
#endif							// CONFIG_LCD_ST7789

#endif							// BOARD_PINS_H
