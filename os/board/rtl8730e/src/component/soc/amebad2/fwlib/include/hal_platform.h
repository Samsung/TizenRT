/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
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
#if defined (ARM_CORE_CM4)
#define SYSTEM_CTRL_BASE		0x41000000
#else
#define SYSTEM_CTRL_BASE		0x42008000
#endif

/* 512B Retention Ram base address */
#define RETENTION_RAM_BASE				0x23020000
#define PCR_BASE						0x23020180
#define PCR3_OFFSET						0x0
#define PCR2_OFFSET						0x20
#define PCR1_OFFSET						0x40
#define PCR0_OFFSET						0x60
/* TBD AutoGen */

/**************************************************************************//**
 * @defgroup AmebaD2_Address_Map AmebaD2 HS/LP Address Map
 * @{
 *****************************************************************************/
#define DDR_BASE                 0x60000000        /* ID:HSLV-2, Inter. Type:AXI-128, Top Address:0x6FFFFFFF, Size(KB):256M, Clk Domain:DDR_CLK */
#define PSRAM_BASE               0x60000000        /* ID:HSLV-2, Inter. Type:AXI-32(psram_spic_automode), Top Address:0x6FFFFFFF, Size(KB):256M, Clk Domain:PSRAM_CLK */
#define HP_SRAM0_BASE            0x20000000        /* ID:HSLV-3, Inter. Type:AXI-64, Top Address:0x20FFFFFF, Size(KB):16M, Clk Domain:HS_AXI_CLK */
#define HP_SRAM_EXT_BASE         0x22000000        /* ID:HSLV-5, Inter. Type:AXI, Top Address:0x22FFFFFF, Size(KB):16M, Clk Domain:HS_APB_CLK */
#define SPI_FLASH_CTRL_BASE      0x44000000        /* ID:HSLV-7, Inter. Type:AXI, Top Address:0x440FFFFF, Size(KB):1M, Clk Domain:SPIC_CLK */
#define SPI_FLASH_BASE           0x08000000        /* ID:HSLV-11, Inter. Type:AXI, Top Address:0x0FFFFFFF, Size(KB):128M, Clk Domain:SPIC_CLK */
#if defined(ARM_CORE_CM0) && ARM_CORE_CM0
#define WIFI_REG_BASE            0x43000000
#else
#define WIFI_REG_BASE            0x40000000        /* ID:HSLV8-0, Inter. Type:AHB, Top Address:0x4007FFFF, Size(KB):512K, Clk Domain:HS_AHB_CLK */
#endif
#define WIFI_RF_REG_BASE         0x40003C00        /* ID:HSLV8-0, Inter. Type:AHB, Top Address:0x4007FFFF, Size(KB):512K, Clk Domain:HS_AHB_CLK */
#define USB_OTG_REG_BASE         0x40080000        /* ID:HSLV8-1, Inter. Type:AHB, Top Address:0x400BFFFF, Size(KB):256K, Clk Domain:HS_AHB_CLK */
#define AES_REG_BASE             0x400C0000        /* ID:HSLV8-2, Inter. Type:APB4, Top Address:0x400C7FFF, Size(KB):32K, Clk Domain:HS_AHB_CLK */
#define SHA_REG_BASE             0x400C8000        /* ID:HSLV8-3, Inter. Type:APB4, Top Address:0x400CFFFF, Size(KB):32K, Clk Domain:HS_AHB_CLK */
#define SDIOH_REG_BASE           0x400D0000        /* ID:HSLV8-4, Inter. Type:APB4, Top Address:0x400D7FFF, Size(KB):32K, Clk Domain:HS_AHB_CLK */
#define LCDC_REG_BASE            0x400D8000        /* ID:HSLV8-5, Inter. Type:APB4, Top Address:0x400DFFFF, Size(KB):32K, Clk Domain:HS_AHB_CLK */
#define GDMA0_REG_BASE           0x400E0000        /* ID:HSLV8-6, Inter. Type:APB4, Top Address:0x400E7FFF, Size(KB):32K, Clk Domain:HS_AHB_CLK */
#define SPI0_REG_BASE            0x400E8000        /* ID:HSLV8-7, Inter. Type:APB4, Top Address:0x400E8FFF, Size(KB):4K, Clk Domain:HS_AHB_CLK */
#define SPI1_REG_BASE            0x400E9000        /* ID:HSLV8-8, Inter. Type:APB4, Top Address:0x400E9FFF, Size(KB):4K, Clk Domain:HS_AHB_CLK */
#define MIPI_DSI_REG_BASE        0x400EA000        /* ID:HSLV8-9, Inter. Type:APB4, Top Address:0x400EAFFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define RSA_REG_BASE             0x400EB000        /* ID:HSLV8-10, Inter. Type:APB4, Top Address:0x400EBFFF, Size(KB):4K, Clk Domain:HS_AHB_CLK */
#define ED25519_REG_BASE         0x400EC000        /* ID:HSLV8-11, Inter. Type:APB4, Top Address:0x400ECFFF, Size(KB):4K, Clk Domain:HS_AHB_CLK */
#define ECDSA_REG_BASE           0x400ED000        /* ID:HSLV8-12, Inter. Type:APB4, Top Address:0x400EDFFF, Size(KB):4K, Clk Domain:HS_AHB_CLK */
#define IR_REG_BASE              0x400EE000        /* ID:HSLV8-13, Inter. Type:APB4, Top Address:0x400EEFFF, Size(KB):4K, Clk Domain:HS_AHB_CLK */
#define I2C1_REG_BASE            0x400EF000        /* ID:HSLV8-14, Inter. Type:APB4, Top Address:0x400EFFFF, Size(KB):4K, Clk Domain:HS_AHB_CLK */
#define I2C2_REG_BASE            0x400F0000        /* ID:HSLV8-15, Inter. Type:APB4, Top Address:0x400F0FFF, Size(KB):4K, Clk Domain:HS_AHB_CLK */
#define ZIGBEE_REG_BASE          0x400F2000        /* ID:HSLV8-16, Inter. Type:AHB, Top Address:0x400F3FFF, Size(KB):8K, Clk Domain:HS_AHB_CLK */
#define BT_REG_BASE              0x40100000        /* ID:HSLV8-20, Inter. Type:AHB, Top Address:0x4017FFFF, Size(KB):512K, Clk Domain:BT_SYS_CLK */
#define SYSTEM_CTRL_BASE_HP      0x41000000        /* ID:HSLV9-0, Inter. Type:APB4, Top Address:0x41000FFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define RXI300M4_REG_BASE        0x41001000        /* ID:HSLV9-1, Inter. Type:APB4, Top Address:0x41001FFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define PSRAM_PHY_REG_BASE       0x41002000        /* ID:HSLV9-2, Inter. Type:APB4, Top Address:0x41002FFF, Size(KB):4K, Clk Domain:PSRAM_CLK */
#define UART0_REG_BASE           0x41004000        /* ID:HSLV9-4, Inter. Type:APB4, Top Address:0x41004FFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define UART1_REG_BASE           0x41005000        /* ID:HSLV9-5, Inter. Type:APB4, Top Address:0x41005FFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define UART2_REG_BASE           0x41006000        /* ID:HSLV9-6, Inter. Type:APB4, Top Address:0x41006FFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define UART3_REG_BASE           0x41007000        /* ID:HSLV9-7, Inter. Type:APB4, Top Address:0x41007FFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define LEDC_REG_BASE            0x41008000        /* ID:HSLV9-8, Inter. Type:APB4, Top Address:0x41008FFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define TRNG_REG_BASE            0x41009000        /* ID:HSLV9-9, Inter. Type:APB4, Top Address:0x41009FFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define TIMER8_REG_BASE          0x4100A000        /* ID:HSLV9-10, Inter. Type:APB4, Top Address:0x4100AFFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define TIMER9_REG_BASE          0x4100A200        /* ID:HSLV9-10, Inter. Type:APB4, Top Address:0x4100AFFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define TIMER10_REG_BASE         0x4100A400        /* ID:HSLV9-10, Inter. Type:APB4, Top Address:0x4100AFFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define TIMER11_REG_BASE         0x4100A600        /* ID:HSLV9-10, Inter. Type:APB4, Top Address:0x4100AFFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define TIMER12_REG_BASE         0x4100A800        /* ID:HSLV9-10, Inter. Type:APB4, Top Address:0x4100AFFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define TIMER13_REG_BASE         0x4100AA00        /* ID:HSLV9-10, Inter. Type:APB4, Top Address:0x4100AFFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define AUDIO_REG_BASE           0x4100B000        /* ID:HSLV9-11, Inter. Type:APB4, Top Address:0x4100BFFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define VAD_REG_BASE             0x4100C000        /* ID:HSLV9-12, Inter. Type:AHB, Top Address:0x4100CFFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define SPORT0_REG_BASE          0x4100D000        /* ID:HSLV9-13, Inter. Type:AHB, Top Address:0x4100DFFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define SPORT1_REG_BASE          0x4100E000        /* ID:HSLV9-14, Inter. Type:AHB, Top Address:0x4100EFFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define SPORT2_REG_BASE          0x4100F000        /* ID:HSLV9-15, Inter. Type:AHB, Top Address:0x4100FFFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define SPORT3_REG_BASE          0x41010000        /* ID:HSLV9-16, Inter. Type:AHB, Top Address:0x41010FFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define DDRPHY_REG_BASE          0x41011000        /* ID:HSLV9-17, Inter. Type:APB4, Top Address:0x41011FFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define PSRAM_REG_BASE           0x41012000        /* ID:HSLV9-18, Inter. Type:AXI, Top Address:0x41012FFF, Size(KB):4K, Clk Domain:PSRAM_CLK */
#define DDRC_REG_BASE            0x41100000        /* ID:HSLV9-20, Inter. Type:APB4, Top Address:0x41103FFF, Size(KB):16K, Clk Domain:DDR_CLK */
#define DDRC_BSTC_REG_BASE       0x41120000        /* ID:HSLV9-21, Inter. Type:APB4, Top Address:0x4113FFFF, Size(KB):128K, Clk Domain:DDR_CLK */
#define CA32_ROM_BASE            0x00000000        /* ID:HSLAV10, Inter. Type:AXI-128, Top Address:0x000003FF, Size(KB):1K, Clk Domain:HS_AXI_CLK */
#define CA32_DBG_REG_BASE        0xA0000000        /* ID:HSLAV12-0, Inter. Type:APB, Top Address:0xA003FFFF, Size(KB):256K, Clk Domain:HS_APB_CLK */
#define CA32_TIMESTAMP_BASE      0xB0002000        /* ID:HSLAV12-1, Inter. Type:APB, Top Address:0xB0002FFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define CA32_GIC_BASE            0xA0100000        /* ID:HSLAV13, Inter. Type:AXI, Top Address:0xA0107FFF, Size(KB):32K, Clk Domain:AP_GIC_CLK */
#define LP_SRAM_BASE             0x23000000        /* ID:LSLV-0, Inter. Type:AXI, Top Address:0x23FFFFFF, Size(KB):16M, Clk Domain:LP_CLK */
#define OTPC_REG_BASE            0x42000000        /* ID:LSLV-1-0, Inter. Type:APB4, Top Address:0x42007FFF, Size(KB):32K, Clk Domain:LP_CLK */
#define SYSTEM_CTRL_BASE_LP      0x42008000        /* ID:LSLV-1-1, Inter. Type:APB4, Top Address:0x42009FFF, Size(KB):8K, Clk Domain:LP_APB_CLK */
#define RXI300M0_REG_BASE        0x4200A000        /* ID:LSLV-1-2, Inter. Type:APB4, Top Address:0x4200AFFF, Size(KB):4K, Clk Domain:LP_APB_CLK */
#define TIMER0_REG_BASE          0x4200B000        /* ID:LSLV-1-3, Inter. Type:APB4, Top Address:0x4200BFFF, Size(KB):4K, Clk Domain:LP_APB_CLK */
#define TIMER1_REG_BASE          0x4200B200        /* ID:LSLV-1-3, Inter. Type:APB4, Top Address:0x4200BFFF, Size(KB):4K, Clk Domain:LP_APB_CLK */
#define TIMER2_REG_BASE          0x4200B400        /* ID:LSLV-1-3, Inter. Type:APB4, Top Address:0x4200BFFF, Size(KB):4K, Clk Domain:LP_APB_CLK */
#define TIMER3_REG_BASE          0x4200B600        /* ID:LSLV-1-3, Inter. Type:APB4, Top Address:0x4200BFFF, Size(KB):4K, Clk Domain:LP_APB_CLK */
#define TIMER4_REG_BASE          0x4200B800        /* ID:LSLV-1-3, Inter. Type:APB4, Top Address:0x4200BFFF, Size(KB):4K, Clk Domain:LP_APB_CLK */
#define TIMER5_REG_BASE          0x4200BA00        /* ID:LSLV-1-3, Inter. Type:APB4, Top Address:0x4200BFFF, Size(KB):4K, Clk Domain:LP_APB_CLK */
#define TIMER6_REG_BASE          0x4200BC00        /* ID:LSLV-1-3, Inter. Type:APB4, Top Address:0x4200BFFF, Size(KB):4K, Clk Domain:LP_APB_CLK */
#define TIMER7_REG_BASE          0x4200BE00        /* ID:LSLV-1-3, Inter. Type:APB4, Top Address:0x4200BFFF, Size(KB):4K, Clk Domain:LP_APB_CLK */
#define UARTLOG_REG_BASE         0x4200C000        /* ID:LSLV-1-4, Inter. Type:APB4, Top Address:0x4200CFFF, Size(KB):4K, Clk Domain:LP_APB_CLK */
#define GPIO_REG_BASE            0x4200D000        /* ID:LSLV-1-5, Inter. Type:APB4, Top Address:0x4200DFFF, Size(KB):4K, Clk Domain:LP_APB_CLK */
#define RTC_BASE                 0x4200E000        /* ID:LSLV-1-6, Inter. Type:APB4, Top Address:0x4200EFFF, Size(KB):4K, Clk Domain:LP_APB_CLK */
#define I2C0_REG_BASE            0x4200F000        /* ID:LSLV-1-7, Inter. Type:APB4, Top Address:0x4200FFFF, Size(KB):4K, Clk Domain:LP_APB_CLK */
#define CTC_REG_BASE             0x42010000        /* ID:LSLV-1-8, Inter. Type:APB4, Top Address:0x42010FFF, Size(KB):4K, Clk Domain:LP_APB_CLK */
#define ADC_REG_BASE             0x42012000        /* ID:LSLV-1-10, Inter. Type:APB4, Top Address:0x42012FFF, Size(KB):4K, Clk Domain:LP_APB_CLK */
#define CMP_REG_BASE             0x42012800        /* ID:LSLV-1-10, Inter. Type:APB4, Top Address:0x42012FFF, Size(KB):4K, Clk Domain:LP_APB_CLK */
#define THERMAL_REG_BASE         0x42013000        /* ID:LSLV-1-11, Inter. Type:APB4, Top Address:0x42013FFF, Size(KB):4K, Clk Domain:LP_APB_CLK */
#define WIFI_FW_REG_BASE         0x43000000        /* ID:LSLV-3, Inter. Type:AHB, Top Address:0x43FFFFFF, Size(KB):16M, Clk Domain:LP_CLK */
#define WDG0_REG_BASE            0x42008400
#define IPCLP_REG_BASE           0x42008600
#define PINMUX_REG_BASE          0x42008A00
#define RSIP_REG_BASE            0x42008C00
#define SDM_REG_BASE             0x42008E00
#define DBGTIM_REG_BASE          0x42009000
#define WDG1_REG_BASE            0x51000400
#define WDG2_REG_BASE            0x41000440
#define WDG3_REG_BASE            0x51000480
#define WDG4_REG_BASE            0x410004C0
#define IPCNP_REG_BASE           0x41000500
#define IPCAP_REG_BASE           0x41000580

#define DDR_BASE_S               0x70000000        /* ID:HSLV-2, Inter. Type:AXI-128, Top Address:0x6FFFFFFF, Size(KB):256M, Clk Domain:DDR_CLK */
#define PSRAM_BASE_S             0x70000000        /* ID:HSLV-2, Inter. Type:AXI-32(psram_spic_automode), Top Address:0x6FFFFFFF, Size(KB):256M, Clk Domain:PSRAM_CLK */
#define HP_SRAM0_BASE_S          0x30000000        /* ID:HSLV-3, Inter. Type:AXI-64, Top Address:0x20FFFFFF, Size(KB):16M, Clk Domain:HS_AXI_CLK */
#define HP_SRAM_EXT_BASE_S       0x32000000        /* ID:HSLV-5, Inter. Type:AXI, Top Address:0x22FFFFFF, Size(KB):16M, Clk Domain:HS_APB_CLK */
#define SPI_FLASH_CTRL_BASE_S    0x54000000        /* ID:HSLV-7, Inter. Type:AXI, Top Address:0x440FFFFF, Size(KB):1M, Clk Domain:SPIC_CLK */
#define SPI_FLASH_BASE_S         0x18000000        /* ID:HSLV-11, Inter. Type:AXI, Top Address:0x0FFFFFFF, Size(KB):128M, Clk Domain:SPIC_CLK */
#define WIFI_REG_BASE_S          0x50000000        /* ID:HSLV8-0, Inter. Type:AHB, Top Address:0x4007FFFF, Size(KB):512K, Clk Domain:HS_AHB_CLK */
#define WIFI_BB_REG_BASE_S       0x50002800        /* ID:HSLV8-0, Inter. Type:AHB, Top Address:0x4007FFFF, Size(KB):512K, Clk Domain:HS_AHB_CLK */
#define WIFI_RF_REG_BASE_S       0x50003800        /* ID:HSLV8-0, Inter. Type:AHB, Top Address:0x4007FFFF, Size(KB):512K, Clk Domain:HS_AHB_CLK */
#define USB_OTG_REG_BASE_S       0x50080000        /* ID:HSLV8-1, Inter. Type:AHB, Top Address:0x400BFFFF, Size(KB):256K, Clk Domain:HS_AHB_CLK */
#define AES_REG_BASE_S           0x500C0000        /* ID:HSLV8-2, Inter. Type:APB4, Top Address:0x400C7FFF, Size(KB):32K, Clk Domain:HS_AHB_CLK */
#define SHA_REG_BASE_S           0x500C8000        /* ID:HSLV8-3, Inter. Type:APB4, Top Address:0x400CFFFF, Size(KB):32K, Clk Domain:HS_AHB_CLK */
#define SDIOH_REG_BASE_S         0x500D0000        /* ID:HSLV8-4, Inter. Type:APB4, Top Address:0x400D7FFF, Size(KB):32K, Clk Domain:HS_AHB_CLK */
#define LCDC_REG_BASE_S          0x500D8000        /* ID:HSLV8-5, Inter. Type:APB4, Top Address:0x400DFFFF, Size(KB):32K, Clk Domain:HS_AHB_CLK */
#define GDMA0_REG_BASE_S         0x500E0000        /* ID:HSLV8-6, Inter. Type:APB4, Top Address:0x400E7FFF, Size(KB):32K, Clk Domain:HS_AHB_CLK */
#define SPI0_REG_BASE_S          0x500E8000        /* ID:HSLV8-7, Inter. Type:APB4, Top Address:0x400E8FFF, Size(KB):4K, Clk Domain:HS_AHB_CLK */
#define SPI1_REG_BASE_S          0x500E9000        /* ID:HSLV8-8, Inter. Type:APB4, Top Address:0x400E9FFF, Size(KB):4K, Clk Domain:HS_AHB_CLK */
#define MIPI_DSI_REG_BASE_S      0x500EA000        /* ID:HSLV8-9, Inter. Type:APB4, Top Address:0x400EAFFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define RSA_REG_BASE_S           0x500EB000        /* ID:HSLV8-10, Inter. Type:APB4, Top Address:0x400EBFFF, Size(KB):4K, Clk Domain:HS_AHB_CLK */
#define ED25519_REG_BASE_S       0x500EC000        /* ID:HSLV8-11, Inter. Type:APB4, Top Address:0x400ECFFF, Size(KB):4K, Clk Domain:HS_AHB_CLK */
#define ECDSA_REG_BASE_S         0x500ED000        /* ID:HSLV8-12, Inter. Type:APB4, Top Address:0x400EDFFF, Size(KB):4K, Clk Domain:HS_AHB_CLK */
#define IR_REG_BASE_S            0x500EE000        /* ID:HSLV8-13, Inter. Type:APB4, Top Address:0x400EEFFF, Size(KB):4K, Clk Domain:HS_AHB_CLK */
#define I2C1_REG_BASE_S          0x500EF000        /* ID:HSLV8-14, Inter. Type:APB4, Top Address:0x400EFFFF, Size(KB):4K, Clk Domain:HS_AHB_CLK */
#define I2C2_REG_BASE_S          0x500F0000        /* ID:HSLV8-15, Inter. Type:APB4, Top Address:0x400F0FFF, Size(KB):4K, Clk Domain:HS_AHB_CLK */
#define ZIGBEE_REG_BASE_S        0x500F2000        /* ID:HSLV8-16, Inter. Type:AHB, Top Address:0x400F3FFF, Size(KB):8K, Clk Domain:HS_AHB_CLK */
#define BT_REG_BASE_S            0x50100000        /* ID:HSLV8-20, Inter. Type:AHB, Top Address:0x4017FFFF, Size(KB):512K, Clk Domain:BT_SYS_CLK */
#define SYSTEM_CTRL_BASE_HP_S    0x51000000        /* ID:HSLV9-0, Inter. Type:APB4, Top Address:0x41000FFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define RXI300M4_REG_BASE_S      0x51001000        /* ID:HSLV9-1, Inter. Type:APB4, Top Address:0x41001FFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define KM4_MPC1_BASE_S          0x51001A00        /* ID:HSLV9-1, Inter. Type:APB4, Top Address:0x41001FFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define KM4_MPC2_BASE_S          0x51001200        /* ID:HSLV9-1, Inter. Type:APB4, Top Address:0x41001FFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define KM4_PPC_BASE_S           0x51001B00        /* ID:HSLV9-1, Inter. Type:APB4, Top Address:0x41001FFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define PSRAM_PHY_REG_BASE_S     0x51002000        /* ID:HSLV9-2, Inter. Type:APB4, Top Address:0x41002FFF, Size(KB):4K, Clk Domain:PSRAM_CLK */
#define UART0_REG_BASE_S         0x51004000        /* ID:HSLV9-4, Inter. Type:APB4, Top Address:0x41004FFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define UART1_REG_BASE_S         0x51005000        /* ID:HSLV9-5, Inter. Type:APB4, Top Address:0x41005FFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define UART2_REG_BASE_S         0x51006000        /* ID:HSLV9-6, Inter. Type:APB4, Top Address:0x41006FFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define UART3_REG_BASE_S         0x51007000        /* ID:HSLV9-7, Inter. Type:APB4, Top Address:0x41007FFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define LEDC_REG_BASE_S          0x51008000        /* ID:HSLV9-8, Inter. Type:APB4, Top Address:0x41008FFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define TRNG_REG_BASE_S          0x51009000        /* ID:HSLV9-9, Inter. Type:APB4, Top Address:0x41009FFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define TIMER8_REG_BASE_S        0x5100A000        /* ID:HSLV9-10, Inter. Type:APB4, Top Address:0x4100AFFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define TIMER9_REG_BASE_S        0x5100A200        /* ID:HSLV9-10, Inter. Type:APB4, Top Address:0x4100AFFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define TIMER10_REG_BASE_S       0x5100A400        /* ID:HSLV9-10, Inter. Type:APB4, Top Address:0x4100AFFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define TIMER11_REG_BASE_S       0x5100A600        /* ID:HSLV9-10, Inter. Type:APB4, Top Address:0x4100AFFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define TIMER12_REG_BASE_S       0x5100A800        /* ID:HSLV9-10, Inter. Type:APB4, Top Address:0x4100AFFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define TIMER13_REG_BASE_S       0x5100AA00        /* ID:HSLV9-10, Inter. Type:APB4, Top Address:0x4100AFFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define AUDIO_REG_BASE_S         0x5100B000        /* ID:HSLV9-11, Inter. Type:APB4, Top Address:0x4100BFFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define VAD_REG_BASE_S           0x5100C000        /* ID:HSLV9-12, Inter. Type:AHB, Top Address:0x4100CFFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define SPORT0_REG_BASE_S        0x5100D000        /* ID:HSLV9-13, Inter. Type:AHB, Top Address:0x4100DFFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define SPORT1_REG_BASE_S        0x5100E000        /* ID:HSLV9-14, Inter. Type:AHB, Top Address:0x4100EFFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define SPORT2_REG_BASE_S        0x5100F000        /* ID:HSLV9-15, Inter. Type:AHB, Top Address:0x4100FFFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define SPORT3_REG_BASE_S        0x51010000        /* ID:HSLV9-16, Inter. Type:AHB, Top Address:0x41010FFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define DDRPHY_REG_BASE_S        0x51011000        /* ID:HSLV9-17, Inter. Type:APB4, Top Address:0x41011FFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define PSRAM_REG_BASE_S         0x51012000        /* ID:HSLV9-18, Inter. Type:AXI, Top Address:0x41012FFF, Size(KB):4K, Clk Domain:PSRAM_CLK */
#define DDRC_REG_BASE_S          0x51100000        /* ID:HSLV9-20, Inter. Type:APB4, Top Address:0x41103FFF, Size(KB):16K, Clk Domain:DDR_CLK */
#define DDRC_BSTC_REG_BASE_S     0x51120000        /* ID:HSLV9-21, Inter. Type:APB4, Top Address:0x4113FFFF, Size(KB):128K, Clk Domain:DDR_CLK */
#define CA32_ROM_BASE_S          0x10000000        /* ID:HSLAV10, Inter. Type:AXI-128, Top Address:0x000003FF, Size(KB):1K, Clk Domain:HS_AXI_CLK */
#define CA32_DBG_REG_BASE_S      0xB0000000        /* ID:HSLAV12-0, Inter. Type:APB, Top Address:0xA003FFFF, Size(KB):256K, Clk Domain:HS_APB_CLK */
#define CA32_TIMESTAMP_BASE_S    0xC0002000        /* ID:HSLAV12-1, Inter. Type:APB, Top Address:0xB0002FFF, Size(KB):4K, Clk Domain:HS_APB_CLK */
#define CA32_GIC_BASE_S          0xB0100000        /* ID:HSLAV13, Inter. Type:AXI, Top Address:0xA0107FFF, Size(KB):32K, Clk Domain:AP_GIC_CLK */

/** @} End of group AmebaD_Address_Map */

#define TIMER0_REG_BASE_S          0x5200B000
#define TIMER1_REG_BASE_S          0x5200B200
#define TIMER2_REG_BASE_S          0x5200B400
#define TIMER3_REG_BASE_S          0x5200B600
#define TIMER4_REG_BASE_S          0x5200B800
#define TIMER5_REG_BASE_S          0x5200BA00
#define TIMER6_REG_BASE_S          0x5200BC00
#define TIMER7_REG_BASE_S          0x5200BE00
#define OTPC_REG_BASE_S            0x52000000
#define PMC_BASE                   0x42008300
#define IPC_SEMA_BASE              0x420083c0
#define IPC_IPC_SEMA_BASE          0x420082F4




/**************************************************************************//**
 * @defgroup AmebaD_Peripheral_Registers_Structures  AmebaD Peripheral_Registers_Structures
 * @{
 *****************************************************************************/

/**************************************************************************//**
 * @defgroup AMEBAD_PINMUX
 * @{
 * @brief AMEBAD_PINMUX Register Declaration
 *****************************************************************************/
#ifndef __ASSEMBLER__

typedef struct {
	__IO uint32_t PADCTR[72];			/*!< Pad control register */
} PINMUX_TypeDef;


/** @} */

/** @} End of group AmebaD_Peripheral_Registers_Structures */

/**************************************************************************//**
 * @defgroup AmebaD_Peripheral_Declaration  AmebaD HS/LP Peripheral Declarations
 * @{
 *****************************************************************************/
#define UART0_DEV			((UART_TypeDef			*) UART0_REG_BASE)		/*hp uart0 */
#define UART1_DEV			((UART_TypeDef			*) UART1_REG_BASE)		/*hp uart1 */
#define UART2_DEV			((UART_TypeDef			*) UART2_REG_BASE)		/*hp uart2 */
#define UART3_DEV			((UART_TypeDef			*) UART3_REG_BASE)		/*hp uart3_bt*/
#define UART0_DEV_S			((UART_TypeDef			*) UART0_REG_BASE_S)		/*hp uart0 secure */
#define UART1_DEV_S			((UART_TypeDef			*) UART1_REG_BASE_S)		/*hp uart1 secure*/
#define UART2_DEV_S			((UART_TypeDef			*) UART2_REG_BASE_S)		/*hp uart2 secrue*/
#define UART3_DEV_S			((UART_TypeDef			*) UART3_REG_BASE_S)		/*hp uart3_bt secure*/

#define LOGUART_DEV			((LOGUART_TypeDef		*) UARTLOG_REG_BASE)		/*KM0 log uart */

#define SPI0_DEV				((SPI_TypeDef			*) SPI0_REG_BASE)		/*hp spi0 */
#define SPI1_DEV				((SPI_TypeDef			*) SPI1_REG_BASE)		/*hp spi1 */
#define SPI0_DEV_S				((SPI_TypeDef			*) SPI0_REG_BASE_S)		/*hp spi0 secure */
#define SPI1_DEV_S				((SPI_TypeDef			*) SPI1_REG_BASE_S)		/*hp spi1 secure */

#define LCDC					((LCDC_TypeDef 			*) LCDC_REG_BASE)				/*KM4 */
#define MIPI					((MIPI_TypeDef 			*) MIPI_DSI_REG_BASE)			/*KM4 */
#define PSRAMC_DEV			((SPIC_TypeDef 			*) PSRAM_REG_BASE)				/*KM4 */
#define PSRAMPHY_DEV			((PSPHY_TypeDef 	*) PSRAM_PHY_REG_BASE)				/*KM4 */

#define DDRPHY_DEV			((DDRPHY_TypeDef 		*) DDRPHY_REG_BASE)				/*KM4 */
#define DDRC_DEV			((DDRC_TypeDef 			*) DDRC_REG_BASE)				/*KM4 */
#define DDRCF_DEV			((DDRCF_TypeDef 		*) (DDRC_REG_BASE + 0x1000))	/*KM4 */

#define SPIC					((SPIC_TypeDef			*) SPI_FLASH_CTRL_BASE)
#define ADC					((ADC_TypeDef			*) ADC_REG_BASE)
#define COMPARATOR				((CMP_TypeDef			*) CMP_REG_BASE)
#define I2C0_DEV				((I2C_TypeDef			*) I2C0_REG_BASE)		/*lp i2c0 */
#define I2C1_DEV				((I2C_TypeDef			*) I2C1_REG_BASE)		/*hp i2c1 */
#define I2C2_DEV				((I2C_TypeDef			*) I2C2_REG_BASE)		/*hp i2c2 */
#define I2C1_DEV_S				((I2C_TypeDef			*) I2C1_REG_BASE_S)		/*hp i2c1 secure */
#define I2C2_DEV_S				((I2C_TypeDef			*) I2C2_REG_BASE_S)		/*hp i2c2 secure */

#define TIM0					((RTIM_TypeDef			*) TIMER0_REG_BASE)
#define TIM1					((RTIM_TypeDef			*) TIMER1_REG_BASE)
#define TIM2					((RTIM_TypeDef			*) TIMER2_REG_BASE)
#define TIM3					((RTIM_TypeDef			*) TIMER3_REG_BASE)
#define TIM4					((RTIM_TypeDef			*) TIMER4_REG_BASE)
#define TIM5					((RTIM_TypeDef			*) TIMER5_REG_BASE)
#define TIM6					((RTIM_TypeDef			*) TIMER6_REG_BASE)
#define TIM7					((RTIM_TypeDef			*) TIMER7_REG_BASE)
#define TIM8					((RTIM_TypeDef			*) TIMER8_REG_BASE)
#define TIM9					((RTIM_TypeDef			*) TIMER9_REG_BASE)
#define TIM10					((RTIM_TypeDef			*) TIMER10_REG_BASE)
#define TIM11					((RTIM_TypeDef			*) TIMER11_REG_BASE)
#define TIM12					((RTIM_TypeDef			*) TIMER12_REG_BASE)
#define TIM13					((RTIM_TypeDef			*) TIMER13_REG_BASE)

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
#define TIM10_S					((RTIM_TypeDef			*) TIMER10_REG_BASE_S)
#define TIM11_S					((RTIM_TypeDef			*) TIMER11_REG_BASE_S)
#define TIM12_S					((RTIM_TypeDef			*) TIMER12_REG_BASE_S)
#define TIM13_S					((RTIM_TypeDef			*) TIMER13_REG_BASE_S)

#define LEDC_DEV			((LEDC_TypeDef			*) LEDC_REG_BASE)
#define LEDC_DEV_S			((LEDC_TypeDef			*) LEDC_REG_BASE_S)

#define IR_DEV				((IR_TypeDef			*) IR_REG_BASE)
#define IR_DEV_S			((IR_TypeDef			*) IR_REG_BASE_S)

#define CAPTOUCH_DEV		((CAPTOUCH_TypeDef		*) CTC_REG_BASE)				/*KM0 */
#define RTC_DEV				((RTC_TypeDef			*) RTC_BASE)				 	/*KM0 */
#define PINMUX				((PINMUX_TypeDef			*) PINMUX_REG_BASE)			/*KM0 */
#define GPIOA_BASE			((GPIO_TypeDef			*) GPIO_REG_BASE)			/*KM0 */
#define GPIOB_BASE			((GPIO_TypeDef			*) (GPIO_REG_BASE + 0x400))	/*KM0 */
#define GPIOC_BASE			((GPIO_TypeDef			*) (GPIO_REG_BASE + 0x800))	/*KM0 */
#define BACKUP_REG			((BACKUP_REG_TypeDef	*) 0x420080E0)			/*AON */

#define KM4_MPC1			((MPC_TypeDef			*) KM4_MPC1_BASE_S)			/*KM4 */
#define KM4_MPC2			((MPC_TypeDef			*) KM4_MPC2_BASE_S)			/*KM4 */
#define KM4_PPC				((PPC_TypeDef			*) KM4_PPC_BASE_S)			/*KM4 */

#define IPCAP_DEV 			((IPC_TypeDef 			*) IPCAP_REG_BASE)				/*CA7 */
#define IPCNP_DEV 			((IPC_TypeDef 			*) IPCNP_REG_BASE)				/*KM4 */
#define IPCLP_DEV 			((IPC_TypeDef			*) IPCLP_REG_BASE)				/*KM0 */

#define GDMA_BASE			((GDMA_TypeDef			*) (GDMA0_REG_BASE))

#define IWDG_DEV			((WDG_TypeDef			*) (WDG0_REG_BASE))
#define WDG1_DEV			((WDG_TypeDef			*) (WDG1_REG_BASE))
#define WDG2_DEV			((WDG_TypeDef			*) (WDG2_REG_BASE))
#define WDG3_DEV			((WDG_TypeDef			*) (WDG3_REG_BASE))
#define WDG4_DEV			((WDG_TypeDef			*) (WDG4_REG_BASE))
#define SDM_DEV				((SDM_TypeDef			*) (SDM_REG_BASE))
#define OTPC_DEV			((OTPC_TypeDef			*) (OTPC_REG_BASE))
#define OTPC_DEV_S			((OTPC_TypeDef			*) (OTPC_REG_BASE_S))
#define DTIMER_DEV			((DEBUGTIMER_TypeDef	*) (DBGTIM_REG_BASE))
#define TRNG_DEV			((TRNG_TypeDef			*) (TRNG_REG_BASE))
#define TRNG_SDEV			((TRNG_TypeDef			*) (TRNG_REG_BASE_S))




#define RSIP_BASE			((RSIP_REG_TypeDef		*) (RSIP_REG_BASE))
#define SDIOH_BASE			((SDIOH_TypeDef			*) (SDIOH_REG_BASE))
#define RSA_BASE			((RSA_TypeDef			*) (RSA_REG_BASE + 0xF00))
#define RSA_MEM_BASE		((RSA_MEM_TypeDef		*) (RSA_REG_BASE))
#define RSA_S_BASE			((RSA_TypeDef			*) (RSA_REG_BASE_S + 0xF00))
#define RSA_S_MEM_BASE		((RSA_MEM_TypeDef		*) (RSA_REG_BASE_S))

#define ECDSA_BASE			((ECDSA_TypeDef			*) (ECDSA_REG_BASE))
#define ECDSA_S_BASE		((ECDSA_TypeDef			*) (ECDSA_REG_BASE_S))

#define ED25519_BASE		((EDDSA_TypeDef			*) (ED25519_REG_BASE))
#define ED25519_S_BASE		((EDDSA_TypeDef			*) (ED25519_REG_BASE_S))

#define THERMAL				((THERMAL_TypeDef				*) THERMAL_REG_BASE)			/*KM0 */

#define AUDIO_SPORT0_DEV	((AUDIO_SPORT_TypeDef		*) SPORT0_REG_BASE)
#define AUDIO_SPORT1_DEV	((AUDIO_SPORT_TypeDef		*) SPORT1_REG_BASE)
#define AUDIO_SPORT2_DEV	((AUDIO_SPORT_TypeDef		*) SPORT2_REG_BASE)
#define AUDIO_SPORT3_DEV	((AUDIO_SPORT_TypeDef		*) SPORT3_REG_BASE)
#define AUDIO_SPORT0_DEV_S	((AUDIO_SPORT_TypeDef		*) SPORT0_REG_BASE_S)	/*SPORT0 secure */
#define AUDIO_SPORT1_DEV_S	((AUDIO_SPORT_TypeDef		*) SPORT1_REG_BASE_S)	/*SPORT1 secure */
#define AUDIO_SPORT2_DEV_S	((AUDIO_SPORT_TypeDef		*) SPORT2_REG_BASE_S)	/*SPORT2 secure */
#define AUDIO_SPORT3_DEV_S	((AUDIO_SPORT_TypeDef		*) SPORT3_REG_BASE_S)	/*SPORT3 secure */

#define RRAM				((RRAM_TypeDef			*) RETENTION_RAM_BASE)

/** @} End of group AmebaD_Peripheral_Declaration */

#define HS_SRAM_S_ADDR_START		0x30000000 /* rom used, can not change */
#define HS_SRAM_S_ADDR_END			0x30FFFFFF /* rom used, can not change */

#define HS_BOOT_ADDR_START			(HS_SRAM_S_ADDR_START +  0x00003000)	/* rom used, can not change */
#define HS_BOOT_ADDR_END			(HS_SRAM_S_ADDR_START + 0x0001AFFF)	/* rom used, can not change */

#define LS_SRAM_ADDR_START			LP_SRAM_BASE
#define LS_SRAM_ADDR_END			(LP_SRAM_BASE + 0x00FFFFFF)
#endif

/* margin 512 for lite and 1024 for CA32 */
#if defined(RSICV_CORE_KR4)
#define CONTEXT_SAVE_SIZE	(320)	/* portCONTEXT_SIZE:66*4 = 288 roundup to 64B aligned */
#elif defined(ARM_CORE_CA32)
#define CONTEXT_SAVE_SIZE	(320 + 1024) /* 15*4 + 32*8: general reg and floating reg */
#elif defined(ARM_CORE_CM4)
#define CONTEXT_SAVE_SIZE	192 /* 15*4 + 16*8: s16~s31 if use float */
#elif defined(ARM_CORE_CM0)
#define CONTEXT_SAVE_SIZE	64	/* not support hw float, 15*4 */
#endif

/** @} End of group AmebaD_Outline */
#endif //_HAL_PLATFORM_