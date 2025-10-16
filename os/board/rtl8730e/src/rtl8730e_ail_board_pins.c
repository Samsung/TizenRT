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

#include <board_pins.h>

// -------------------
// Unified Pin Table
// -------------------

const pin_desc_t pin_table[] = {
	// ============ UART ============
#ifdef CONFIG_RTL8730E_UART0
	// UART0
	{_PA_2,		GPIO_PuPd_UP,		GPIO_PuPd_UP},
	{_PA_3,		GPIO_PuPd_UP,		GPIO_PuPd_UP},
#endif
#ifdef CONFIG_RTL8730E_UART1
	// UART1
	{_PA_9,		GPIO_PuPd_UP,		GPIO_PuPd_UP},
	{_PA_10,	GPIO_PuPd_UP,		GPIO_PuPd_UP},
#endif
#ifdef CONFIG_RTL8730E_UART2
	// UART2
	{_PB_21,	GPIO_PuPd_NOPULL,	GPIO_PuPd_NOPULL},
	{_PB_22,	GPIO_PuPd_UP,		GPIO_PuPd_UP},
	{_PB_19,	GPIO_PuPd_DOWN,		GPIO_PuPd_DOWN},
	{_PB_20,	GPIO_PuPd_DOWN,		GPIO_PuPd_DOWN},
#endif
#ifdef CONFIG_RTL8730E_UART4
	//UART4
	{_PB_23,	GPIO_PuPd_UP,		GPIO_PuPd_UP},
	{_PB_24,	GPIO_PuPd_UP,		GPIO_PuPd_UP},
#endif
	// ============ I2C ============
	// I2C
#ifdef CONFIG_AMEBASMART_I2C0
	// I2C0
	{_PB_30,	GPIO_PuPd_NOPULL,	GPIO_PuPd_NOPULL},
	{_PB_29,	GPIO_PuPd_NOPULL,	GPIO_PuPd_NOPULL},
#endif
#ifdef CONFIG_AMEBASMART_I2C1
	// I2C1
	{_PA_4,		GPIO_PuPd_UP,		GPIO_PuPd_UP},
	{_PA_3,		GPIO_PuPd_UP,		GPIO_PuPd_UP},
#endif
#ifdef CONFIG_AMEBASMART_I2C2
	//I2C2
	{_PA_29,	GPIO_PuPd_UP,		GPIO_PuPd_UP},
	{_PA_28,	GPIO_PuPd_UP,		GPIO_PuPd_UP},
#endif
	// ============ SPI ============
#ifdef CONFIG_AMEBASMART_SPI0
	// SPI0
	{_PB_4,		GPIO_PuPd_DOWN,		GPIO_PuPd_DOWN},		// SDA NPU SPI0_MOSI
	{_PB_3,		GPIO_PuPd_DOWN,		GPIO_PuPd_DOWN},		// SDA NPU SPI0_MISO
	{_PB_6,		GPIO_PuPd_DOWN,		GPIO_PuPd_DOWN},
	{_PB_5,		GPIO_PuPd_UP,		GPIO_PuPd_UP},
#if defined(CONFIG_SPI_CS) && defined(CONFIG_AMEBASMART_SPI0_CS)
	{_PB_31,	GPIO_PuPd_DOWN,		GPIO_PuPd_DOWN},		// SDA pull down 4.7k
#endif
#endif
#ifdef CONFIG_AMEBASMART_SPI1
	// SPI1
	{_PB_28,	GPIO_PuPd_UP,		GPIO_PuPd_UP},
	{_PB_27,	GPIO_PuPd_UP,		GPIO_PuPd_UP},
	{_PB_25,	GPIO_PuPd_DOWN,		GPIO_PuPd_DOWN},
	{_PB_26,	GPIO_PuPd_UP,		GPIO_PuPd_UP},
#if defined(CONFIG_SPI_CS) && defined(CONFIG_AMEBASMART_SPI1_CS)
	{_PB_29,	GPIO_PuPd_NOPULL,	GPIO_PuPd_NOPULL},		// SDA pull up 4.7k
#endif
#endif
	// ============ IST415 =========
#if defined(CONFIG_TOUCH_IST415)
	//IST415
	{_PA_5,		GPIO_PuPd_DOWN,		GPIO_PuPd_DOWN},		// SDA pull up 4.7k & pull down 9.1k
	{_PA_4,		GPIO_PuPd_UP,		GPIO_PuPd_UP},			// SDA pull up 4.7k
#endif
#ifdef CONFIG_AMEBASMART_I2S2
	//I2S
	{NULL,		GPIO_PuPd_NOPULL,	GPIO_PuPd_NOPULL},
	{_PB_21,	GPIO_PuPd_NOPULL,	GPIO_PuPd_NOPULL},
	{_PA_16,	GPIO_PuPd_NOPULL,	GPIO_PuPd_NOPULL},		// RTK QA board need pull up
	{_PB_10,	GPIO_PuPd_NOPULL,	GPIO_PuPd_NOPULL},		// SDA AMP-I2S
	{_PB_19,	GPIO_PuPd_DOWN,		GPIO_PuPd_DOWN},		// NA
#endif
#ifdef CONFIG_AMEBASMART_I2S3
	{_PA_15,	GPIO_PuPd_DOWN,		GPIO_PuPd_DOWN},		// SDA pull down 4.7k
	{_PA_14,	GPIO_PuPd_UP,		GPIO_PuPd_UP},			// RTK SWD_CLK
	{_PA_13,	GPIO_PuPd_UP,		GPIO_PuPd_UP},			// RTK SWD_DATA
	{_PB_11,	GPIO_PuPd_UP,		GPIO_PuPd_UP},			// SDA AILite+W LCD PWN
	{_PB_20,	GPIO_PuPd_DOWN,		GPIO_PuPd_DOWN},		// SDA pull down 10k
#endif
	//GPIO
	{_PA_23,	GPIO_MODE_IN,		GPIO_PuPd_UP},			// SDA pull down 4.7k
	{_PB_20,	GPIO_MODE_OUT,		GPIO_PuPd_UP},			// SDA pull down 10k
	{_PB_22,	GPIO_MODE_IN,		GPIO_PuPd_DOWN},		// SDA pull up 10k; RTK TM_DIS
	// ============ PWM ============
	// PWM
	// PWM0
	{_PA_0,		GPIO_PuPd_DOWN,         GPIO_PuPd_DOWN}, 		// RTL8730ELH
	{_PA_1,		GPIO_PuPd_DOWN,         GPIO_PuPd_DOWN}, 		// RTL8730ELH
#ifndef CONFIG_RTL8730E_UART0
	{_PA_2,		GPIO_PuPd_UP,		GPIO_PuPd_UP},
	{_PA_3,		GPIO_PuPd_UP,		GPIO_PuPd_UP},
#endif
#ifndef CONFIG_RTL8730E_UART1
#ifndef CONFIG_RTL8730E_BOARD_AID
	{_PA_4,		GPIO_PuPd_UP,		GPIO_PuPd_UP},
	{_PA_5,		GPIO_PuPd_DOWN,		GPIO_PuPd_DOWN},
#endif
#endif
	{_PA_6,		GPIO_PuPd_DOWN,         GPIO_PuPd_DOWN}, 		// RTL8730ELH
	{_PA_7,		GPIO_PuPd_DOWN,         GPIO_PuPd_DOWN}, 		// RTL8730ELH
	{_PA_8,         GPIO_PuPd_DOWN,         GPIO_PuPd_DOWN},  		// RTL8730ELH
#ifndef CONFIG_RTL8730E_UART1
#endif
	{_PA_12,	GPIO_PuPd_DOWN,         GPIO_PuPd_DOWN}, 		// RTL8730ELH
#ifndef CONFIG_AMEBASMART_I2S3
	{_PA_13,	GPIO_PuPd_UP,		GPIO_PuPd_UP},			// RTK SWD_DATA
	{_PA_14,	GPIO_PuPd_UP,		GPIO_PuPd_UP},			// RTK SWD_CLK
	{_PA_15,	GPIO_PuPd_DOWN,		GPIO_PuPd_DOWN},		// SDA pull down 4.7k
#endif
#ifndef CONFIG_AMEBASMART_I2S2
	{_PA_16,	GPIO_PuPd_NOPULL,	GPIO_PuPd_NOPULL},		// RTK QA board need pull up
#endif
	{_PA_17,	GPIO_PuPd_DOWN,         GPIO_PuPd_DOWN}, 		// NA
	{_PA_18,	GPIO_PuPd_UP,           GPIO_PuPd_DOWN}, 		// NA
	{_PA_19,	GPIO_PuPd_DOWN,         GPIO_PuPd_DOWN}, 		// NA
	{_PA_20,	GPIO_PuPd_DOWN,         GPIO_PuPd_DOWN},  		// NA
	{_PA_21,	GPIO_PuPd_DOWN,         GPIO_PuPd_DOWN}, 		// NA
	{_PA_22,	GPIO_PuPd_NOPULL,       GPIO_PuPd_NOPULL},		// SDA AMP-RESET
	{_PA_24,	GPIO_PuPd_UP,           GPIO_PuPd_UP},  		// SDA pull up 100k
	{_PA_25,	GPIO_PuPd_UP,           GPIO_PuPd_UP},   		// SDA pull up 10k
	{_PA_26,	GPIO_PuPd_DOWN,         GPIO_PuPd_DOWN}, 		// RTL8730ELH
	{_PA_27,	GPIO_PuPd_DOWN,         GPIO_PuPd_DOWN}, 		// RTL8730ELH
#ifndef CONFIG_AMEBASMART_I2C2
	{_PA_28,	GPIO_PuPd_UP,		GPIO_PuPd_UP},
	{_PA_29,	GPIO_PuPd_UP,		GPIO_PuPd_UP},
#endif
	{_PA_30,	GPIO_PuPd_DOWN,         GPIO_PuPd_DOWN}, 		// NA
	{_PA_31,	GPIO_PuPd_DOWN,         GPIO_PuPd_DOWN}, 		// NA
	{_PB_0,		GPIO_PuPd_UP,           GPIO_PuPd_DOWN}, 		// RTL8730ELH
	{_PB_1,		GPIO_PuPd_UP,           GPIO_PuPd_DOWN}, 		// RTL8730ELH
	{_PB_2,		GPIO_PuPd_UP,           GPIO_PuPd_DOWN}, 		// NA
#ifndef CONFIG_AMEBASMART_SPI0
	{_PB_3,		GPIO_PuPd_DOWN,		GPIO_PuPd_DOWN},		// SDA NPU SPI0_MISO
	{_PB_4,		GPIO_PuPd_DOWN,		GPIO_PuPd_DOWN},		// SDA NPU SPI0_MOSI
	{_PB_5,		GPIO_PuPd_UP,		GPIO_PuPd_UP},
	{_PB_6,		GPIO_PuPd_DOWN,		GPIO_PuPd_DOWN},
#endif
	{_PB_7,		GPIO_PuPd_DOWN,         GPIO_PuPd_DOWN}, 		// RTL8730ELH
	{_PB_8,		GPIO_PuPd_UP,           GPIO_PuPd_DOWN}, 		// RTL8730ELH
	{_PB_9,		GPIO_PuPd_UP,           GPIO_PuPd_DOWN}, 		// RTL8730ELH
#ifndef CONFIG_AMEBASMART_I2S2
	{_PB_10,	GPIO_PuPd_NOPULL,	GPIO_PuPd_NOPULL},		// SDA AMP-I2S
#endif
#ifndef CONFIG_AMEBASMART_I2S3
	{_PB_11,	GPIO_PuPd_UP,		GPIO_PuPd_UP},			// SDA AILite+W LCD PWN
#endif
	{_PB_12,	GPIO_PuPd_UP,           GPIO_PuPd_DOWN}, 		// RTL8730ELH
	{_PB_13,	GPIO_PuPd_UP,           GPIO_PuPd_DOWN}, 		// RTL8730ELH
	{_PB_14,	GPIO_PuPd_UP,           GPIO_PuPd_DOWN}, 		// RTL8730ELH
	{_PB_15,	GPIO_PuPd_UP,           GPIO_PuPd_DOWN}, 		// RTL8730ELH
	{_PB_16,	GPIO_PuPd_UP,           GPIO_PuPd_DOWN}, 		// RTL8730ELH
	{_PB_17,	GPIO_PuPd_UP,           GPIO_PuPd_UP}, 			// RTL8730ELH
	{_PB_18,	GPIO_PuPd_DOWN,         GPIO_PuPd_DOWN}, 		// RTL8730ELH
#ifndef CONFIG_RTL8730E_UART2
	{_PB_19,	GPIO_PuPd_DOWN,		GPIO_PuPd_DOWN},
	{_PB_20,	GPIO_PuPd_DOWN,		GPIO_PuPd_DOWN},
	{_PB_21,	GPIO_PuPd_NOPULL,	GPIO_PuPd_NOPULL},
	{_PB_22,	GPIO_PuPd_UP,		GPIO_PuPd_UP},
#endif
#ifndef CONFIG_RTL8730E_UART4
	{_PB_23,	GPIO_PuPd_UP,		GPIO_PuPd_UP},
	{_PB_24,	GPIO_PuPd_UP,		GPIO_PuPd_UP},
#endif
#ifndef CONFIG_AMEBASMART_SPI1
	{_PB_25,	GPIO_PuPd_DOWN,		GPIO_PuPd_DOWN},
	{_PB_26,	GPIO_PuPd_UP,		GPIO_PuPd_UP},
	{_PB_27,	GPIO_PuPd_UP,		GPIO_PuPd_UP},
	{_PB_28,	GPIO_PuPd_UP,		GPIO_PuPd_UP},
#if !defined(CONFIG_SPI_CS) && !defined(CONFIG_AMEBASMART_SPI1_CS)
	{_PB_29,	GPIO_PuPd_NOPULL,	GPIO_PuPd_NOPULL},		// SDA pull up 4.7k
#endif
#endif
#ifndef CONFIG_AMEBASMART_I2C0
	{_PB_29,	GPIO_PuPd_NOPULL,	GPIO_PuPd_NOPULL},
	{_PB_30,	GPIO_PuPd_NOPULL,	GPIO_PuPd_NOPULL},
#endif
#ifndef CONFIG_AMEBASMART_SPI0
#if !defined(CONFIG_SPI_CS) && !defined(CONFIG_AMEBASMART_SPI0_CS)
	{_PB_31,	GPIO_PuPd_DOWN,		GPIO_PuPd_DOWN},		// SDA pull down 4.7k
#endif
#endif
	{_PC_0,		GPIO_PuPd_UP,           GPIO_PuPd_UP},  		// SDA pull up 4.7k
	{_PC_1,		GPIO_PuPd_UP,           GPIO_PuPd_UP}, 			// RTK Int-Flash SPI_FLASH_DATA
	{_PC_2,		GPIO_PuPd_UP,           GPIO_PuPd_DOWN}, 		// RTK Int-Flash SPI_FLASH_CLK pull down for PG or 50uA
	{_PC_3,		GPIO_PuPd_UP,           GPIO_PuPd_UP}, 			// RTK Int-Flash SPI_FLASH_DATA
	{_PC_4,         GPIO_PuPd_UP,           GPIO_PuPd_UP},   		// RTK Int-Flash SPI_FLASH_DATA
	{_PC_5,         GPIO_PuPd_UP,           GPIO_PuPd_UP},   		// RTK Int-Flash SPI_FLASH_DATA
	{_PC_6,         GPIO_PuPd_UP,           GPIO_PuPd_UP},   		// RTK Int-Flash SPI_FLASH_CS
	{_PNC,          GPIO_PuPd_KEEP,         GPIO_PuPd_KEEP},  		// table end
};
