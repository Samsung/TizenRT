/**
  ******************************************************************************
  * @file    hal_platform.h
  * @author
  * @version V1.0.0
  * @date    2016-05-17
  * @brief   This file contains all the definations of platform.
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */
#ifndef _HAL_PLATFORM_
#define _HAL_PLATFORM_

#define ROMVERSION			0x01 /* ACUT is 0x01, BCUT is 0x02 ... */
#define ROMVERSION_SUB		0 /* ROM sub version */
#define ROMINFORMATION		(ROMVERSION)

#define HAL_READ32(base, addr)				rtk_le32_to_cpu(*((volatile u32*)(base + addr)))
#define HAL_WRITE32(base, addr, value32)		((*((volatile u32*)(base + addr))) = rtk_cpu_to_le32(value32))
#define HAL_READ16(base, addr)				rtk_le16_to_cpu(*((volatile u16*)(base + addr)))
#define HAL_WRITE16(base, addr, value)		((*((volatile u16*)(base + addr))) = rtk_cpu_to_le16(value))
#define HAL_READ8(base, addr)				(*((volatile u8*)(base + addr)))
#define HAL_WRITE8(base, addr, value)		((*((volatile u8*)(base + addr))) = value)


#ifdef __cplusplus
#define __I		volatile             /*!< Defines 'read only' permissions                 */
#else
#define __I		volatile const       /*!< Defines 'read only' permissions                 */
#endif
#define __O		volatile             /*!< Defines 'write only' permissions                */
#define __IO	 	volatile             /*!< Defines 'read / write' permissions              */

/**************************************************************************//**
 * @defgroup AmebaD_Outline
 * @{
 *****************************************************************************/

/** @addtogroup AmebaD_Outline
  * @verbatim
  *****************************************************************************************
  * CPU
  *****************************************************************************************
  *	-Clock speed:		125MHz
  *	-Core:				CM4F
  *	-Working votage:	3.3V
  *
  *****************************************************************************************
  * Memory
  *****************************************************************************************
  *	-ROM:				512KB
  *	-RAM:				256KB
  *	-FLash:				128MB
  *
  *****************************************************************************************
  * GPIO
  *****************************************************************************************
  *	-GPIOA:				0~31, IN/OUT/INT
  *	-GPIOB:				0~31, IN/OUT/INT
  *
  *****************************************************************************************
  * Flash
  *****************************************************************************************
  *	-Max. speed:		100MHz
  *	-SPI Mode:			Quad IO mode
  *	-cache:				32K I/D read cache
  *	-XIP:				support executed in place
  *
  *****************************************************************************************
  * Firmware Protection
  *****************************************************************************************
  *	-RSIP(OTF):(Realsil Image Protection)
  *		-Flash Encryption
  *		-key store in OTP EFUSE Area, and can not read
  *	-RDP
  *		-4K RAM read protection
  *		-key store in OTP EFUSE Area, and can not read
  *
  *****************************************************************************************
  * UART
  *****************************************************************************************
  *	-Set count:			2
  *	-IrDA:				support
  *	-Low Power:			support
  *	-Sleep wakeup:		support
  *	-Baud rate:			110~6000000
  *	-GDMA:				support
  *
  *****************************************************************************************
  * UART LOG
  *****************************************************************************************
  *	-Set count:			1
  *	-IrDA:				support
  *	-Low Power:			not support
  *	-Sleep wakeup:		not support
  *	-Baud rate:			110~6000000
  *
  *****************************************************************************************
  * SPI Master
  *****************************************************************************************
  *	-Set count:			1
  *	-Max. Speed:		31.25MHz
  *	-GDMA:				support
  *
  *****************************************************************************************
  * SPI Slave
  *****************************************************************************************
  *	-Set count:			1
  *	-Max. Speed:		31.25MHz
  *	-GDMA:				support
  *
  *****************************************************************************************
  * I2C
  *****************************************************************************************
  *	-Set count:			1
  *	-Speed:				Standard (up to 100 kHz) and Fast (up to 400 kHz) Modes
  *	-Role:				Master & Slave
  *	-Sleep wakeup:		support
  *	-GDMA:				support
  *
  *****************************************************************************************
  * RTC
  *****************************************************************************************
  *	-Set count:			1
  *	-H:					12/24 hour mode
  *	-M:					0~59
  *	-S:					0~59
  *	-D:					0~511
  *	-Alarm:				support D/H/M/S
  *	-Daylight:			support
  *	-Calibration:		support
  *
  *****************************************************************************************
  * PWM
  *****************************************************************************************
  *	-Channel count:		6(KM0)/18(KM4)
  *	-Max. Resolution:	16bits (customizable)
  *	-Max. Frequency:	40MHz@1-bit mode, 40/(2^x))MHz@x-bit
  *
  *****************************************************************************************
  * Capture
  *****************************************************************************************
  *	-Channel count:		1
  *	-Max. Resolution:	16bits (customizable)
  *	-Max. Frequency:	40MHz@1-bit mode, 40/(2^x))MHz@x-bit
  *	-capture width:		supoort
  *	-capture number:	support
  *
  *****************************************************************************************
  * Timer
  *****************************************************************************************
  *	-Set count:			12
  *	-32KHz timer:		8
  *	-40MHz timer:		4
  *
  *****************************************************************************************
  * I2S
  *****************************************************************************************
  *	-Set count:			1
  *	-Source clk:		45.1584MHz or 98.304MHz(default)
  *	-Sample rate:		8K, 12K, 16K, 24K, 32K, 48K, 64K, 96K, 192K, 384K, 7.35K, 11.025K, 14.7K, 22.05K, 29.4K, 44.1K, 58.8K, 88.2K, 176.4K
  *	-Sample bit:		16 bit, 24 bit
  *	-DMA:				support
  *
  *****************************************************************************************
  * WIFI
  *****************************************************************************************
  *	-Set count:			1
  *	-Mode:				B/G/N(2.4G)
  *	-BW:				20MHz/40MHz
  *	-Ant:				1T/1R
  *
  *****************************************************************************************
  * USB device
  *****************************************************************************************
  *	-Set count:			1
  *	-DMA:				support
  *
  *****************************************************************************************
  * SDIO device
  *****************************************************************************************
  *	-Set count:			1
  *	-GSPI:				support
  *	-DMA:				support
  *
  *****************************************************************************************
  * BACKUP REG
  *****************************************************************************************
  *	-Set count:			4 dwords
  *
  *****************************************************************************************
  * CRYPTO
  *****************************************************************************************
  *	-MD5
  *	-SHA1
  *	-SHA2
  *	-HMAC-MD5
  *	-HMAC-SHA1
  *	-HMAC-SHA2
  *
  *	-AES-CBC
  *	-AES-ECB
  *	-AES-CTR
  *	-AES-CFB
  *	-AES-OFB
  *	-AES-GCM
  *	-3DES-CBC
  *	-3DES-ECB
  *	-DES-CBC
  *	-DES-ECB
  *
  *****************************************************************************************
  * ADC
  *****************************************************************************************
  *	-Set count:			1
  *	-Channel:			4
  *	-Sample rate:		max frequency up to 1MHz per channel, configurable
  *	-Resolution:		12 bit
  *	-signal sampling:	CH1 & CH3: 0 ~ 3V, CH2: 0~5V
  *	-power save:		Support one shot mode
  *	-GDMA:				support
  *
  *****************************************************************************************
  * GDMA
  *****************************************************************************************
  *	-Set count:			2
  *	-Channel:			2*6
  *
  *****************************************************************************************
  * @endverbatim
  */

/* TBD AutoGen */
#define SYSTEM_CTRL_BASE		0x4100C000


/**************************************************************************//**
 * @defgroup AmebaD2_Address_Map AmebaD2 HS/LP Address Map
 * @{
 *****************************************************************************/
#define SPI_FLASH_BASE           0x08000000        /* ID:S1, Inter. Type:AXI4, Top Address:0xFFFFFFF, Size(KB):128M, Clk Domain:SPIC_CLK */
#define HP_SRAM0_BASE            0x20000000        /* ID:S2, Inter. Type:AXI4, Top Address:0x2003FFFF, Size(KB):256K, Clk Domain:SRAM_CLK */
#define HP_SRAM_EXT_BASE         0x22000000        /* ID:S4, Inter. Type:AXI4, Top Address:0x22FFFFFF, Size(KB):16M, Clk Domain:SRAM2_CLK */
#define WIFI_REG_BASE            0x40000000        /* ID:S5-0, Inter. Type:AHB, Top Address:0x4007FFFF, Size(KB):512K, Clk Domain:PERI_HCLK */
#define WIFI_RF_REG_BASE         0x40003C00        /* ID:S5-0, Inter. Type:AHB, Top Address:0x4007FFFF, Size(KB):512K, Clk Domain:PERI_HCLK */
#define AES_REG_BASE             0x40100000        /* ID:S5-2, Inter. Type:APB4, Top Address:0x40107FFF, Size(KB):32K, Clk Domain:PERI_HCLK */
#define SHA_REG_BASE             0x40108000        /* ID:S5-3, Inter. Type:APB4, Top Address:0x4010FFFF, Size(KB):32K, Clk Domain:PERI_HCLK */
#define GDMA0_REG_BASE           0x40110000        /* ID:S5-4, Inter. Type:APB4, Top Address:0x40117FFF, Size(KB):32K, Clk Domain:PERI_HCLK */
#define SPI0_REG_BASE            0x40118000        /* ID:S5-5, Inter. Type:APB4, Top Address:0x40118FFF, Size(KB):4K, Clk Domain:PERI_HCLK */
#define SPI1_REG_BASE            0x40119000        /* ID:S5-6, Inter. Type:APB4, Top Address:0x40119FFF, Size(KB):4K, Clk Domain:PERI_HCLK */
#define I2C0_REG_BASE            0x4011A000        /* ID:S5-7, Inter. Type:APB4, Top Address:0x4011AFFF, Size(KB):4K, Clk Domain:PERI_HCLK */
#define I2C1_REG_BASE            0x4011B000        /* ID:S5-8, Inter. Type:APB4, Top Address:0x4011BFFF, Size(KB):4K, Clk Domain:PERI_HCLK */
#define SPORT0_REG_BASE          0x4011C000        /* ID:S5-9, Inter. Type:AHB, Top Address:0x4011CFFF, Size(KB):4K, Clk Domain:PERI_HCLK */
#define SPORT1_REG_BASE          0x4011D000        /* ID:S5-10, Inter. Type:AHB, Top Address:0x4011DFFF, Size(KB):4K, Clk Domain:PERI_HCLK */
#define DEBUGTIMER_REG_BASE      0x4011E000        /* ID:S5-11, Inter. Type:APB4, Top Address:0x4011EFFF, Size(KB):4K, Clk Domain:PERI_HCLK */
#define ECDSA_REG_BASE           0x4011F000        /* ID:S5-12, Inter. Type:APB4, Top Address:0x4011FFFF, Size(KB):4K, Clk Domain:PERI_HCLK */
#define OTPC_REG_BASE            0x41000000        /* ID:S6-0, Inter. Type:APB4, Top Address:0x41007FFF, Size(KB):32K, Clk Domain:PERI_LCLK */
#define PSRAM_PHY_REG_BASE       0x41008000        /* ID:S6-1, Inter. Type:APB4, Top Address:0x41008FFF, Size(KB):4K, Clk Domain:PSRAM_CLK */
#define PSRAM_REG_BASE           0x41009000        /* ID:S6-2, Inter. Type:AXI4, Top Address:0x41009FFF, Size(KB):4K, Clk Domain:PSRAM_CLK */
#define SPI_FLASH_CTRL_BASE      0x4100A000        /* ID:S6-3, Inter. Type:AXI4, Top Address:0x4100AFFF, Size(KB):4K, Clk Domain:SPIC_CLK */
#define SYSTEM_CTRL_BASE_HP      0x4100C000        /* ID:S6-4, Inter. Type:APB4, Top Address:0x4100DFFF, Size(KB):8K, Clk Domain:PERI_LCLK */
#define RETENTION_RAM_BASE       0x4100CE00        /* ID:S6-4, Inter. Type:APB4, Top Address:0x4100CFFF, Size(KB):8K, Clk Domain:PERI_LCLK */
#define RXI300M4_REG_BASE        0x4100E000        /* ID:S6-5, Inter. Type:APB4, Top Address:0x4100FFFF, Size(KB):8K, Clk Domain:PERI_LCLK */
#define UART0_REG_BASE           0x41010000        /* ID:S6-6, Inter. Type:APB4, Top Address:0x41010FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define UART1_REG_BASE           0x41011000        /* ID:S6-7, Inter. Type:APB4, Top Address:0x41011FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define UART2_REG_BASE           0x41012000        /* ID:S6-8, Inter. Type:APB4, Top Address:0x41012FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define UART3_REG_BASE           0x41013000        /* ID:S6-9, Inter. Type:APB4, Top Address:0x41013FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define UARTLOG_REG_BASE         0x41014000        /* ID:S6-10, Inter. Type:APB4, Top Address:0x41014FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define LEDC_REG_BASE            0x41015000        /* ID:S6-11, Inter. Type:APB4, Top Address:0x41015FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TRNG_REG_BASE            0x41016000        /* ID:S6-12, Inter. Type:APB4, Top Address:0x41016FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define AUDIO_REG_BASE           0x41017000        /* ID:S6-13, Inter. Type:APB4, Top Address:0x41017FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER0_REG_BASE          0x41018000        /* ID:S6-14, Inter. Type:APB4, Top Address:0x41018FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER1_REG_BASE          0x41018200        /* ID:S6-14, Inter. Type:APB4, Top Address:0x41018FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER2_REG_BASE          0x41018400        /* ID:S6-14, Inter. Type:APB4, Top Address:0x41018FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER3_REG_BASE          0x41018600        /* ID:S6-14, Inter. Type:APB4, Top Address:0x41018FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER4_REG_BASE          0x41018800        /* ID:S6-14, Inter. Type:APB4, Top Address:0x41018FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER5_REG_BASE          0x41018A00        /* ID:S6-14, Inter. Type:APB4, Top Address:0x41018FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER6_REG_BASE          0x41018C00        /* ID:S6-14, Inter. Type:APB4, Top Address:0x41018FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER7_REG_BASE          0x41018E00        /* ID:S6-14, Inter. Type:APB4, Top Address:0x41018FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER8_REG_BASE          0x41019000        /* ID:S6-15, Inter. Type:APB4, Top Address:0x41019FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER9_REG_BASE          0x41019200        /* ID:S6-15, Inter. Type:APB4, Top Address:0x41019FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER10_REG_BASE         0x41019400        /* ID:S6-15, Inter. Type:APB4, Top Address:0x41019FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER11_REG_BASE         0x41019600        /* ID:S6-15, Inter. Type:APB4, Top Address:0x41019FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER12_REG_BASE         0x41019800        /* ID:S6-15, Inter. Type:APB4, Top Address:0x41019FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER13_REG_BASE         0x41019A00        /* ID:S6-15, Inter. Type:APB4, Top Address:0x41019FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER14_REG_BASE         0x41019C00        /* ID:S6-15, Inter. Type:APB4, Top Address:0x41019FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define GPIO_REG_BASE            0x4101A000        /* ID:S6-16, Inter. Type:APB4, Top Address:0x4101AFFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define RTC_BASE                 0x4101B000        /* ID:S6-17, Inter. Type:APB4, Top Address:0x4101BFFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define ADC_REG_BASE             0x4101C000        /* ID:S6-18, Inter. Type:APB4, Top Address:0x4101CFFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define CMP_REG_BASE             0x4101C800        /* ID:S6-18, Inter. Type:APB4, Top Address:0x4101CFFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define THERMAL_REG_BASE         0x4101D000        /* ID:S6-19, Inter. Type:APB4, Top Address:0x4101DFFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define CTC_REG_BASE             0x4101E000        /* ID:S6-20, Inter. Type:APB4, Top Address:0x4101EFFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define WDG0_REG_BASE            0x4100CC00        /* ID:S6-21, Inter. Type:APB4, Top Address:0x4101FFFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define WDG1_REG_BASE            0x4101F000        /* ID:S6-21, Inter. Type:APB4, Top Address:0x4101FFFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define WDG2_REG_BASE            0x4101F040        /* ID:S6-21, Inter. Type:APB4, Top Address:0x4101FFFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define WDG3_REG_BASE            0x4101F080        /* ID:S6-21, Inter. Type:APB4, Top Address:0x4101FFFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define WDG4_REG_BASE            0x4101F0C0        /* ID:S6-21, Inter. Type:APB4, Top Address:0x4101FFFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define IPC0_REG_BASE            0x41020000        /* ID:S6-22, Inter. Type:APB4, Top Address:0x41020FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define IPC1_REG_BASE            0x41020080        /* ID:S6-22, Inter. Type:APB4, Top Address:0x41020FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define IPC2_REG_BASE            0x41020100        /* ID:S6-22, Inter. Type:APB4, Top Address:0x41020FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define SDM_REG_BASE             0x41021000        /* ID:S6-23, Inter. Type:APB4, Top Address:0x41021FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define BT_REG_BASE              0x41080000        /* ID:S6-25, Inter. Type:AHB, Top Address:0x410FFFFF, Size(KB):512K, Clk Domain:BT_SYS_CLK */
#define PSRAM_BASE               0x60000000        /* ID:S7, Inter. Type:AXI4, Top Address:0x6FFFFFFF, Size(KB):256M, Clk Domain:PSRAM_CLK */
#define PINMUX_REG_BASE          0x4100C800
#define RSIP_REG_BASE            0x4100CA00
#define RSA_REG_BASE             0x400EB000        /* ID:HSLV8-10, Inter. Type:APB4, Top Address:0x400EBFFF, Size(KB):4K, Clk Domain:HS_AHB_CLK */

#define SPI_FLASH_BASE_S         0x18000000        /* ID:S1, Inter. Type:AXI4, Top Address:0xFFFFFFF, Size(KB):128M, Clk Domain:SPIC_CLK */
#define HP_SRAM0_BASE_S          0x30000000        /* ID:S2, Inter. Type:AXI4, Top Address:0x2003FFFF, Size(KB):256K, Clk Domain:SRAM_CLK */
#define HP_SRAM_EXT_BASE_S       0x32000000        /* ID:S4, Inter. Type:AXI4, Top Address:0x22FFFFFF, Size(KB):16M, Clk Domain:SRAM2_CLK */
#define WIFI_REG_BASE_S          0x50000000        /* ID:S5-0, Inter. Type:AHB, Top Address:0x4007FFFF, Size(KB):512K, Clk Domain:PERI_HCLK */
#define WIFI_RF_REG_BASE_S       0x50003C00        /* ID:S5-0, Inter. Type:AHB, Top Address:0x4007FFFF, Size(KB):512K, Clk Domain:PERI_HCLK */
#define AES_REG_BASE_S           0x50100000        /* ID:S5-2, Inter. Type:APB4, Top Address:0x40107FFF, Size(KB):32K, Clk Domain:PERI_HCLK */
#define SHA_REG_BASE_S           0x50108000        /* ID:S5-3, Inter. Type:APB4, Top Address:0x4010FFFF, Size(KB):32K, Clk Domain:PERI_HCLK */
#define GDMA0_REG_BASE_S         0x50110000        /* ID:S5-4, Inter. Type:APB4, Top Address:0x40117FFF, Size(KB):32K, Clk Domain:PERI_HCLK */
#define SPI0_REG_BASE_S          0x50118000        /* ID:S5-5, Inter. Type:APB4, Top Address:0x40118FFF, Size(KB):4K, Clk Domain:PERI_HCLK */
#define SPI1_REG_BASE_S          0x50119000        /* ID:S5-6, Inter. Type:APB4, Top Address:0x40119FFF, Size(KB):4K, Clk Domain:PERI_HCLK */
#define I2C0_REG_BASE_S          0x5011A000        /* ID:S5-7, Inter. Type:APB4, Top Address:0x4011AFFF, Size(KB):4K, Clk Domain:PERI_HCLK */
#define I2C1_REG_BASE_S          0x5011B000        /* ID:S5-8, Inter. Type:APB4, Top Address:0x4011BFFF, Size(KB):4K, Clk Domain:PERI_HCLK */
#define SPORT0_REG_BASE_S        0x5011C000        /* ID:S5-9, Inter. Type:AHB, Top Address:0x4011CFFF, Size(KB):4K, Clk Domain:PERI_HCLK */
#define SPORT1_REG_BASE_S        0x5011D000        /* ID:S5-10, Inter. Type:AHB, Top Address:0x4011DFFF, Size(KB):4K, Clk Domain:PERI_HCLK */
#define DEBUGTIMER_REG_BASE_S    0x5011E000        /* ID:S5-11, Inter. Type:APB4, Top Address:0x4011EFFF, Size(KB):4K, Clk Domain:PERI_HCLK */
#define ECDSA_REG_BASE_S         0x5011F000        /* ID:S5-12, Inter. Type:APB4, Top Address:0x4011FFFF, Size(KB):4K, Clk Domain:PERI_HCLK */
#define OTPC_REG_BASE_S          0x51000000        /* ID:S6-0, Inter. Type:APB4, Top Address:0x41007FFF, Size(KB):32K, Clk Domain:PERI_LCLK */
#define PSRAM_PHY_REG_BASE_S     0x51008000        /* ID:S6-1, Inter. Type:APB4, Top Address:0x41008FFF, Size(KB):4K, Clk Domain:PSRAM_CLK */
#define PSRAM_REG_BASE_S         0x51009000        /* ID:S6-2, Inter. Type:AXI4, Top Address:0x41009FFF, Size(KB):4K, Clk Domain:PSRAM_CLK */
#define SPI_FLASH_CTRL_BASE_S    0x5100A000        /* ID:S6-3, Inter. Type:AXI4, Top Address:0x4100AFFF, Size(KB):4K, Clk Domain:SPIC_CLK */
#define SYSTEM_CTRL_BASE_HP_S    0x5100C000        /* ID:S6-4, Inter. Type:APB4, Top Address:0x4100DFFF, Size(KB):8K, Clk Domain:PERI_LCLK */
#define RXI300M4_REG_BASE_S      0x5100E000        /* ID:S6-5, Inter. Type:APB4, Top Address:0x4100FFFF, Size(KB):8K, Clk Domain:PERI_LCLK */
#define KM4_MPC1_BASE_S          0x5100E200        /* ID:S6-5, Inter. Type:APB4, Top Address:0x41001FFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define KM4_MPC2_BASE_S          0x5100E600        /* ID:S6-5, Inter. Type:APB4, Top Address:0x41001FFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define KM4_MPC3_BASE_S          0x5100EA00        /* ID:S6-5, Inter. Type:APB4, Top Address:0x41001FFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define KM4_PPC_BASE_S           0x5100E300        /* ID:S6-5, Inter. Type:APB4, Top Address:0x41001FFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define KM4_PPC2_BASE_S          0x5100EB00        /* ID:S6-5, Inter. Type:APB4, Top Address:0x41001FFF, Size(KB):4K, Clk Domain:HS_APB_CLK */

#define UART0_REG_BASE_S         0x51010000        /* ID:S6-6, Inter. Type:APB4, Top Address:0x41010FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define UART1_REG_BASE_S         0x51011000        /* ID:S6-7, Inter. Type:APB4, Top Address:0x41011FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define UART2_REG_BASE_S         0x51012000        /* ID:S6-8, Inter. Type:APB4, Top Address:0x41012FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define UART3_REG_BASE_S         0x51013000        /* ID:S6-9, Inter. Type:APB4, Top Address:0x41013FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define UARTLOG_REG_BASE_S       0x51014000        /* ID:S6-10, Inter. Type:APB4, Top Address:0x41014FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define LEDC_REG_BASE_S          0x51015000        /* ID:S6-11, Inter. Type:APB4, Top Address:0x41015FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TRNG_REG_BASE_S          0x51016000        /* ID:S6-12, Inter. Type:APB4, Top Address:0x41016FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define AUDIO_REG_BASE_S         0x51017000        /* ID:S6-13, Inter. Type:APB4, Top Address:0x41017FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER0_REG_BASE_S        0x51018000        /* ID:S6-14, Inter. Type:APB4, Top Address:0x41018FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER1_REG_BASE_S        0x51018200        /* ID:S6-14, Inter. Type:APB4, Top Address:0x41018FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER2_REG_BASE_S        0x51018400        /* ID:S6-14, Inter. Type:APB4, Top Address:0x41018FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER3_REG_BASE_S        0x51018600        /* ID:S6-14, Inter. Type:APB4, Top Address:0x41018FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER4_REG_BASE_S        0x51018800        /* ID:S6-14, Inter. Type:APB4, Top Address:0x41018FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER5_REG_BASE_S        0x51018A00        /* ID:S6-14, Inter. Type:APB4, Top Address:0x41018FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER6_REG_BASE_S        0x51018C00        /* ID:S6-14, Inter. Type:APB4, Top Address:0x41018FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER7_REG_BASE_S        0x51018E00        /* ID:S6-14, Inter. Type:APB4, Top Address:0x41018FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER8_REG_BASE_S        0x51019000        /* ID:S6-15, Inter. Type:APB4, Top Address:0x41019FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER9_REG_BASE_S        0x51019200        /* ID:S6-15, Inter. Type:APB4, Top Address:0x41019FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER10_REG_BASE_S       0x51019400        /* ID:S6-15, Inter. Type:APB4, Top Address:0x41019FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER11_REG_BASE_S       0x51019600        /* ID:S6-15, Inter. Type:APB4, Top Address:0x41019FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER12_REG_BASE_S       0x51019800        /* ID:S6-15, Inter. Type:APB4, Top Address:0x41019FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER13_REG_BASE_S       0x51019A00        /* ID:S6-15, Inter. Type:APB4, Top Address:0x41019FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER14_REG_BASE_S       0x51019C00        /* ID:S6-15, Inter. Type:APB4, Top Address:0x41019FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define GPIO_REG_BASE_S          0x5101A000        /* ID:S6-16, Inter. Type:APB4, Top Address:0x4101AFFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define RTC_BASE_S               0x5101B000        /* ID:S6-17, Inter. Type:APB4, Top Address:0x4101BFFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define ADC_REG_BASE_S           0x5101C000        /* ID:S6-18, Inter. Type:APB4, Top Address:0x4101CFFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define CMP_REG_BASE_S           0x5101C800        /* ID:S6-18, Inter. Type:APB4, Top Address:0x4101CFFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define THERMAL_REG_BASE_S       0x5101D000        /* ID:S6-19, Inter. Type:APB4, Top Address:0x4101DFFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define CTC_REG_BASE_S           0x5101E000        /* ID:S6-20, Inter. Type:APB4, Top Address:0x4101EFFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define WDG0_REG_BASE_S          0x5100CC00        /* ID:S6-21, Inter. Type:APB4, Top Address:0x4101FFFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define WDG1_REG_BASE_S          0x5101F000        /* ID:S6-21, Inter. Type:APB4, Top Address:0x4101FFFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define WDG2_REG_BASE_S          0x5101F040        /* ID:S6-21, Inter. Type:APB4, Top Address:0x4101FFFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define WDG3_REG_BASE_S          0x5101F080        /* ID:S6-21, Inter. Type:APB4, Top Address:0x4101FFFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define WDG4_REG_BASE_S          0x5101F0C0        /* ID:S6-21, Inter. Type:APB4, Top Address:0x4101FFFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define IPC0_REG_BASE_S          0x51020000        /* ID:S6-22, Inter. Type:APB4, Top Address:0x41020FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define IPC1_REG_BASE_S          0x51020080        /* ID:S6-22, Inter. Type:APB4, Top Address:0x41020FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define IPC2_REG_BASE_S          0x510200F0        /* ID:S6-22, Inter. Type:APB4, Top Address:0x41020FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define SDM_REG_BASE_S           0x51021000        /* ID:S6-23, Inter. Type:APB4, Top Address:0x41021FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define BT_REG_BASE_S            0x51080000        /* ID:S6-25, Inter. Type:AHB, Top Address:0x410FFFFF, Size(KB):512K, Clk Domain:BT_SYS_CLK */
#define PSRAM_BASE_S             0x70000000        /* ID:S7, Inter. Type:AXI4, Top Address:0x6FFFFFFF, Size(KB):256M, Clk Domain:PSRAM_CLK */
#define RSA_REG_BASE_S           0x500EB000        /* ID:HSLV8-10, Inter. Type:APB4, Top Address:0x400EBFFF, Size(KB):4K, Clk Domain:HS_AHB_CLK */
#define PINMUX_REG_BASE_S        0x5100C800

/** @} End of group AmebaD_Address_Map */

#define AON_BACKUP_BASE          0x4100C0E0
#define PMC_BASE                 0x4100C300

/**************************************************************************//**
 * @defgroup AmebaD_Peripheral_Declaration  AmebaD HS/LP Peripheral Declarations
 * @{
 *****************************************************************************/
#define RRAM_DEV			((RRAM_TypeDef			*) RETENTION_RAM_BASE)		/*retention mem */

#define UART0_DEV			((UART_TypeDef			*) UART0_REG_BASE)		/*hp uart0 */
#define UART1_DEV			((UART_TypeDef			*) UART1_REG_BASE)		/*hp uart1 */
#define UART2_DEV			((UART_TypeDef			*) UART2_REG_BASE)		/*hp uart2 */
#define UART3_DEV			((UART_TypeDef			*) UART3_REG_BASE)		/*hp uart3_bt*/
#define UART0_DEV_S			((UART_TypeDef			*) UART0_REG_BASE_S)		/*hp uart0 secure */
#define UART1_DEV_S			((UART_TypeDef			*) UART1_REG_BASE_S)		/*hp uart1 secure*/
#define UART2_DEV_S			((UART_TypeDef			*) UART2_REG_BASE_S)		/*hp uart2 secrue*/
#define UART3_DEV_S			((UART_TypeDef			*) UART3_REG_BASE_S)		/*hp uart3_bt secure*/

#define LOGUART_DEV			((LOGUART_TypeDef		*) UARTLOG_REG_BASE)
#define LOGUART_DEV_S			((LOGUART_TypeDef		*) UARTLOG_REG_BASE_S)

#define SPI0_DEV			((SPI_TypeDef			*) SPI0_REG_BASE)		/*hp spi0 */
#define SPI1_DEV			((SPI_TypeDef			*) SPI1_REG_BASE)		/*hp spi1 */
#define SPI0_DEV_S			((SPI_TypeDef			*) SPI0_REG_BASE_S)		/*hp spi0 secure */
#define SPI1_DEV_S			((SPI_TypeDef			*) SPI1_REG_BASE_S)		/*hp spi1 secure */

#define PSRAMC_DEV			((SPIC_TypeDef			*) PSRAM_REG_BASE)				/*KM4 */
#define PSRAMPHY_DEV		((PSPHY_TypeDef			*) PSRAM_PHY_REG_BASE)				/*KM4 */

#define SPIC				((SPIC_TypeDef			*) SPI_FLASH_CTRL_BASE)
#define ADC					((ADC_TypeDef			*) ADC_REG_BASE)
#define ADC_S				((ADC_TypeDef			*) ADC_REG_BASE_S)
#define CMP_DEV				((CMP_TypeDef			*) CMP_REG_BASE)
#define CMP_DEV_S			((CMP_TypeDef			*) CMP_REG_BASE_S)
#define I2C0_DEV			((I2C_TypeDef			*) I2C0_REG_BASE)		/*lp i2c0 */
#define I2C1_DEV			((I2C_TypeDef			*) I2C1_REG_BASE)		/*hp i2c1 */
#define I2C2_DEV			((I2C_TypeDef			*) I2C2_REG_BASE)		/*hp i2c2 */
#define I2C1_DEV_S			((I2C_TypeDef			*) I2C1_REG_BASE_S)		/*hp i2c1 secure */
#define TIM0				((RTIM_TypeDef			*) TIMER0_REG_BASE)
#define TIM1				((RTIM_TypeDef			*) TIMER1_REG_BASE)
#define TIM2				((RTIM_TypeDef			*) TIMER2_REG_BASE)
#define TIM3				((RTIM_TypeDef			*) TIMER3_REG_BASE)
#define TIM4				((RTIM_TypeDef			*) TIMER4_REG_BASE)
#define TIM5				((RTIM_TypeDef			*) TIMER5_REG_BASE)
#define TIM6				((RTIM_TypeDef			*) TIMER6_REG_BASE)
#define TIM7				((RTIM_TypeDef			*) TIMER7_REG_BASE)
#define TIM8				((RTIM_TypeDef			*) TIMER8_REG_BASE)
#define TIM9				((RTIM_TypeDef			*) TIMER9_REG_BASE)
#define TIM10				((RTIM_TypeDef			*) TIMER10_REG_BASE)
#define TIM11				((RTIM_TypeDef			*) TIMER11_REG_BASE)
#define TIM12				((RTIM_TypeDef			*) TIMER12_REG_BASE)
#define TIM13				((RTIM_TypeDef			*) TIMER13_REG_BASE)
#define TIM14				((RTIM_TypeDef			*) TIMER14_REG_BASE)

#define TIM0_S				((RTIM_TypeDef			*) TIMER0_REG_BASE_S)
#define TIM1_S				((RTIM_TypeDef			*) TIMER1_REG_BASE_S)
#define TIM2_S				((RTIM_TypeDef			*) TIMER2_REG_BASE_S)
#define TIM3_S				((RTIM_TypeDef			*) TIMER3_REG_BASE_S)
#define TIM4_S				((RTIM_TypeDef			*) TIMER4_REG_BASE_S)
#define TIM5_S				((RTIM_TypeDef			*) TIMER5_REG_BASE_S)
#define TIM6_S				((RTIM_TypeDef			*) TIMER6_REG_BASE_S)
#define TIM7_S				((RTIM_TypeDef			*) TIMER7_REG_BASE_S)
#define TIM8_S				((RTIM_TypeDef			*) TIMER8_REG_BASE_S)
#define TIM9_S				((RTIM_TypeDef 			*) TIMER9_REG_BASE_S)
#define TIM10_S				((RTIM_TypeDef			*) TIMER10_REG_BASE_S)
#define TIM11_S				((RTIM_TypeDef			*) TIMER11_REG_BASE_S)
#define TIM12_S				((RTIM_TypeDef			*) TIMER12_REG_BASE_S)
#define TIM13_S				((RTIM_TypeDef			*) TIMER13_REG_BASE_S)
#define TIM14_S				((RTIM_TypeDef			*) TIMER14_REG_BASE_S)

#define LEDC_DEV			((LEDC_TypeDef			*) LEDC_REG_BASE)
#define LEDC_DEV_S			((LEDC_TypeDef			*) LEDC_REG_BASE_S)

#define CAPTOUCH_DEV		((CAPTOUCH_TypeDef		*) CTC_REG_BASE)				/*KM0 */
#define RTC_DEV				((RTC_TypeDef			*) RTC_BASE)					/*KM0 */
#define RTC_DEV_S			((RTC_TypeDef			*) RTC_BASE_S)
#define PINMUX				((PINMUX_TypeDef		*) PINMUX_REG_BASE)			/*KM0 */
#define GPIOA_BASE			((GPIO_TypeDef			*) GPIO_REG_BASE)			/*KM0 */
#define GPIOB_BASE			((GPIO_TypeDef			*) (GPIO_REG_BASE + 0x400))	/*KM0 */
#define GPIOA_BASE_S		((GPIO_TypeDef			*) GPIO_REG_BASE_S)
#define GPIOB_BASE_S		((GPIO_TypeDef			*) (GPIO_REG_BASE_S + 0x400))
#define BACKUP_REG			((BACKUP_REG_TypeDef	*) AON_BACKUP_BASE)			/*AON */
#define PINMUX_S			((PINMUX_TypeDef		*) PINMUX_REG_BASE_S)			/*KM0 */

#define KM4_MPC1			((MPC_TypeDef			*) KM4_MPC1_BASE_S)			/*KM4 */
#define KM4_MPC2			((MPC_TypeDef			*) KM4_MPC2_BASE_S)			/*KM4 */
#define KM4_MPC3			((MPC_TypeDef			*) KM4_MPC3_BASE_S)			/*KM4 */

#define KM4_PPC				((PPC_TypeDef			*) KM4_PPC_BASE_S)			/*KM4 */
#define KM4_PPC2			((PPC_TypeDef			*) KM4_PPC2_BASE_S)			/*KM4 */

#define IPCKR4_DEV 			((IPC_TypeDef 			*) IPC0_REG_BASE)				/*KR4 */
#define IPCKM4_DEV 			((IPC_TypeDef 			*) IPC1_REG_BASE)				/*KM4 */
#define IPCDSP_DEV 			((IPC_TypeDef			*) IPC2_REG_BASE)				/*DSP */
#define IPCKR4_DEV_S 			((IPC_TypeDef 			*) IPC0_REG_BASE_S)				/*KR4 Secure */
#define IPCKM4_DEV_S 			((IPC_TypeDef 			*) IPC1_REG_BASE_S)				/*KM4 Secure*/
#define IPCDSP_DEV_S 			((IPC_TypeDef			*) IPC2_REG_BASE_S)				/*DSP Secure*/

#define GDMA_BASE			((GDMA_TypeDef			*) (GDMA0_REG_BASE))

#define IWDG_DEV			((WDG_TypeDef			*) (WDG0_REG_BASE))
#define WDG1_DEV			((WDG_TypeDef			*) (WDG1_REG_BASE_S))
#define WDG2_DEV			((WDG_TypeDef			*) (WDG2_REG_BASE))
#define WDG3_DEV			((WDG_TypeDef			*) (WDG3_REG_BASE))
#define WDG4_DEV			((WDG_TypeDef			*) (WDG4_REG_BASE))
#define SDM_DEV				((SDM_TypeDef			*) (SDM_REG_BASE))
#define SDM_DEV_S			((SDM_TypeDef			*) (SDM_REG_BASE_S))
#define OTPC_DEV			((OTPC_TypeDef			*) (OTPC_REG_BASE))
#define OTPC_DEV_S			((OTPC_TypeDef			*) (OTPC_REG_BASE_S))
#define DTIMER_DEV			((DEBUGTIMER_TypeDef	*) (DEBUGTIMER_REG_BASE))
#define DTIMER_DEV_S		((DEBUGTIMER_TypeDef	*) (DEBUGTIMER_REG_BASE_S))
#define TRNG_DEV			((TRNG_TypeDef			*) (TRNG_REG_BASE))
#define TRNG_SDEV			((TRNG_TypeDef			*) (TRNG_REG_BASE_S))

#define RSIP_BASE			((RSIP_REG_TypeDef		*) (RSIP_REG_BASE))
#define RSA_BASE			((RSA_TypeDef			*) (RSA_REG_BASE + 0xF00))
#define RSA_MEM_BASE		((RSA_MEM_TypeDef		*) (RSA_REG_BASE))
#define RSA_S_BASE			((RSA_TypeDef			*) (RSA_REG_BASE_S + 0xF00))
#define RSA_S_MEM_BASE		((RSA_MEM_TypeDef		*) (RSA_REG_BASE_S))

#define ECDSA_BASE			((ECDSA_TypeDef			*) (ECDSA_REG_BASE))
#define ECDSA_S_BASE		((ECDSA_TypeDef			*) (ECDSA_REG_BASE_S))

#define TM_DEV				((THERMAL_TypeDef		*) THERMAL_REG_BASE)			/*KM0 */
#define TM_DEV_S			((THERMAL_TypeDef		*) THERMAL_REG_BASE_S)

#define AUDIO_SPORT0_DEV	((AUDIO_SPORT_TypeDef	*) SPORT0_REG_BASE)
#define AUDIO_SPORT1_DEV	((AUDIO_SPORT_TypeDef	*) SPORT1_REG_BASE)
#define AUDIO_SPORT2_DEV	((AUDIO_SPORT_TypeDef	*) SPORT2_REG_BASE)
#define AUDIO_SPORT3_DEV	((AUDIO_SPORT_TypeDef	*) SPORT3_REG_BASE)
#define AUDIO_SPORT0_DEV_S	((AUDIO_SPORT_TypeDef	*) SPORT0_REG_BASE_S)	/*SPORT0 secure */
#define AUDIO_SPORT1_DEV_S	((AUDIO_SPORT_TypeDef	*) SPORT1_REG_BASE_S)	/*SPORT1 secure */

/** @} End of group AmebaD_Peripheral_Declaration */

#define HS_SRAM_S_ADDR_START		HP_SRAM0_BASE_S 				/* rom used, can not change */
#define HS_SRAM_S_ADDR_END			(HP_SRAM0_BASE_S + 0x00FFFFFF)	/* rom used, can not change */

#define HS_BOOT_ADDR_START		(HP_SRAM0_BASE_S +  0x0000A000)		/* rom used, can not change */
#define HS_BOOT_ADDR_END			(HS_BOOT_ADDR_START + 0x00017FFF)	/* rom used, can not change */

#define LS_SRAM_ADDR_START			LP_SRAM_BASE
#define LS_SRAM_ADDR_END			(LP_SRAM_BASE + 0x00020000)


/** @} End of group AmebaD_Outline */
#endif //_HAL_PLATFORM_
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
