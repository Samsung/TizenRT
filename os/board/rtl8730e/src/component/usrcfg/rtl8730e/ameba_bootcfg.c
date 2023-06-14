/**
  ******************************************************************************
  * @file    rtl8721d_bootcfg.c
  * @author
  * @version V1.0.0
  * @date    2018-09-12
  * @brief   This file provides firmware functions to manage the following
  *          functionalities:
  *           - memory layout config
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#include "ameba_soc.h"

/*
* @brif	MMU Configuration.
*	There are 8 MMU entries totally. Entry 0 & Entry 1 are already used by OTA, Entry 2~7 can be used by Users.
*/
BOOT_RAM_DATA_SECTION
MMU_ConfDef Flash_MMU_Config[] = {
	/*VAddrStart,	VAddrEnd,			PAddrStart,		PAddrEnd*/
	{0xFFFFFFFF,	0xFFFFFFFF,			0xFFFFFFFF,		0xFFFFFFFF},	//Entry 2
	{0xFFFFFFFF,	0xFFFFFFFF,			0xFFFFFFFF,		0xFFFFFFFF},	//Entry 3
	{0xFFFFFFFF,	0xFFFFFFFF,			0xFFFFFFFF,		0xFFFFFFFF},	//Entry 4
	{0xFFFFFFFF,	0xFFFFFFFF,			0xFFFFFFFF,		0xFFFFFFFF},	//Entry 5
	{0xFFFFFFFF,	0xFFFFFFFF,			0xFFFFFFFF,		0xFFFFFFFF},	//Entry 6
	{0xFFFFFFFF,	0xFFFFFFFF,			0xFFFFFFFF,		0xFFFFFFFF},	//Entry 7
	{0xFFFFFFFF,	0xFFFFFFFF,			0xFFFFFFFF,		0xFFFFFFFF},
};

/*
* @brif	RCC Configuration.
*	 by Users.
*/
BOOT_RAM_DATA_SECTION
RCC_ConfDef RCC_Config[] = {
	/*Function,				Clock,							State*/
	/* AON */
	{APBPeriph_SDM32K,		APBPeriph_SDM32K_CLOCK,			ENABLE},
	{APBPeriph_RTC,			APBPeriph_RTC_CLOCK,			DISABLE},
	{APBPeriph_ATIM,			APBPeriph_ATIM_CLOCK,			ENABLE},
	{APBPeriph_BOR,			APBPeriph_CLOCK_NULL,			DISABLE},

	{APBPeriph_I2C2,			APBPeriph_I2C2_CLOCK,			DISABLE},
	{APBPeriph_I2C1,			APBPeriph_I2C1_CLOCK,			DISABLE},
	{APBPeriph_I2C0,			APBPeriph_I2C0_CLOCK,			DISABLE},
	{APBPeriph_GPIO,			APBPeriph_GPIO_CLOCK,			ENABLE},
	{APBPeriph_CTC,			APBPeriph_CTC_CLOCK,			DISABLE},
	{APBPeriph_ADC,			APBPeriph_ADC_CLOCK,			DISABLE},
	{APBPeriph_IPC_LP,		APBPeriph_IPC_LP_CLOCK,			ENABLE},
	{APBPeriph_BTON,			APBPeriph_BTON_CLOCK,			DISABLE},
	{APBPeriph_THM,			APBPeriph_THM_CLOCK,			DISABLE},
	{APBPeriph_SCE,			APBPeriph_CLOCK_NULL,		ENABLE},
	{APBPeriph_FLASH,		APBPeriph_FLASH_CLOCK,			DISABLE},
	{APBPeriph_WLON,		APBPeriph_WLON_CLOCK,			DISABLE},
	{APBPeriph_NP,			APBPeriph_NP_CLOCK,			DISABLE},
	{APBPeriph_LP,			APBPeriph_LP_CLOCK,			DISABLE},
	{APBPeriph_HPLFM,		APBPeriph_HPLFM_CLOCK,			DISABLE},
	{APBPeriph_LPLFM,		APBPeriph_LPLFM_CLOCK,			DISABLE},
	{APBPeriph_SIC,			APBPeriph_SIC_CLOCK,			DISABLE},
	{APBPeriph_HPON,			APBPeriph_HPON_CLOCK,			DISABLE},

	{APBPeriph_PSRAM,		APBPeriph_PSRAM_CLOCK,			DISABLE},
	{APBPeriph_AUDIO,			APBPeriph_CLOCK_NULL,			DISABLE},
	{APBPeriph_AC,			APBPeriph_AC_CLOCK,			DISABLE},
	{APBPeriph_DDRP,			APBPeriph_DDRP_CLOCK,			ENABLE},
	{APBPeriph_DDRC,			APBPeriph_DDRC_CLOCK,			ENABLE},
	{APBPeriph_IRDA,			APBPeriph_IRDA_CLOCK,			DISABLE},
	{APBPeriph_ECDSA,		APBPeriph_ECDSA_CLOCK,			DISABLE},
	{APBPeriph_ED25519,		APBPeriph_ED25519_CLOCK,			DISABLE},
	{APBPeriph_RSA,			APBPeriph_CLOCK_NULL,			DISABLE},
	{APBPeriph_IPC_HP,		APBPeriph_IPC_HP_CLOCK,			ENABLE},
	{APBPeriph_LEDC,			APBPeriph_LEDC_CLOCK,			DISABLE},
	{APBPeriph_TRNG,			APBPeriph_TRNG_CLOCK,			DISABLE},
	{APBPeriph_USB,			APBPeriph_USB_CLOCK,			DISABLE},
	{APBPeriph_SDH,			APBPeriph_SDH_CLOCK,			DISABLE},
	{APBPeriph_SPI1,			APBPeriph_SPI1_CLOCK,			DISABLE},
	{APBPeriph_SPI0,			APBPeriph_SPI0_CLOCK,			DISABLE},
	{APBPeriph_GDMA,		APBPeriph_GDMA_CLOCK,			ENABLE},
	{APBPeriph_LCDC,			APBPeriph_LCDCMIPI_CLOCK,			DISABLE},
	{APBPeriph_IPSEC,		APBPeriph_IPSEC_CLOCK,			ENABLE},
	{APBPeriph_LX1,			APBPeriph_LX1_CLOCK,				ENABLE},
	{APBPeriph_NULL,			APBPeriph_HPERI_CLOCK,			DISABLE},

	{APBPeriph_TIM0,			APBPeriph_TIM0_CLOCK,			ENABLE},
	{APBPeriph_TIM1,			APBPeriph_TIM1_CLOCK,			DISABLE},
	{APBPeriph_TIM2,			APBPeriph_TIM2_CLOCK,			DISABLE},
	{APBPeriph_TIM3,			APBPeriph_TIM3_CLOCK,			DISABLE},
	{APBPeriph_TIM4,			APBPeriph_TIM4_CLOCK,			DISABLE},
	{APBPeriph_TIM5,			APBPeriph_TIM5_CLOCK,			DISABLE},
	{APBPeriph_TIM6,			APBPeriph_TIM6_CLOCK,			DISABLE},
	{APBPeriph_TIM7,			APBPeriph_TIM7_CLOCK,			DISABLE},
	{APBPeriph_TIM8,			APBPeriph_TIM8_CLOCK,			DISABLE},
	{APBPeriph_TIM9,			APBPeriph_TIM9_CLOCK,			DISABLE},
	{APBPeriph_TIM10,		APBPeriph_TIM10_CLOCK,			DISABLE},
	{APBPeriph_TIM11,		APBPeriph_TIM11_CLOCK,			DISABLE},
	{APBPeriph_TIM12,		APBPeriph_TIM12_CLOCK,			DISABLE},
	{APBPeriph_TIM13,		APBPeriph_TIM13_CLOCK,			DISABLE},
	{APBPeriph_SPORT0,		APBPeriph_SPORT0_CLOCK,			DISABLE},
	{APBPeriph_SPORT1,		APBPeriph_SPORT1_CLOCK,			DISABLE},
	{APBPeriph_SPORT2,		APBPeriph_SPORT2_CLOCK,			DISABLE},
	{APBPeriph_SPORT3,		APBPeriph_SPORT3_CLOCK,			DISABLE},
	{APBPeriph_UART0,		APBPeriph_UART0_CLOCK,			DISABLE},
	{APBPeriph_UART1,		APBPeriph_UART1_CLOCK,			DISABLE},
	{APBPeriph_UART2,		APBPeriph_UART2_CLOCK,			DISABLE},
	{APBPeriph_UART3,		APBPeriph_UART3_CLOCK,			DISABLE},

	{0xFFFFFFFF,			NULL,							ENABLE},
};

/*
* @brif	CACHE and TCM share memory setting
*/
CACHETCM_TypeDef CACHETCM_Def[7] = {
//		TCM_Size		ICACHE_EN		DCACHE_EN			IWRR				DWRR		TCM_BASE		TCM_TOP
	{TCM_SIZE_0KB,		ENABLE,			ENABLE,		CACHE_WWR_4WAY,		CACHE_WWR_4WAY,			0,			0},
	{TCM_SIZE_16KB,		ENABLE, 		ENABLE, 	CACHE_WWR_3WAY,		CACHE_WWR_4WAY,		0x80000,		0x83fff},
	{TCM_SIZE_32KB,		ENABLE, 		ENABLE, 	CACHE_WWR_3WAY,		CACHE_WWR_2WAY,		0x80000,		0x87fff},
	{TCM_SIZE_48KB,		ENABLE, 		ENABLE, 	CACHE_WWR_2WAY,		CACHE_WWR_2WAY,		0x80000,		0x8bfff},
	{TCM_SIZE_64KB,		ENABLE, 		ENABLE, 	CACHE_WWR_1WAY,		CACHE_WWR_2WAY,		0x80000,		0x8ffff},
	{TCM_SIZE_80KB,		ENABLE, 		DISABLE, 	CACHE_WWR_1WAY,		CACHE_WWR_4WAY,		0x80000,		0x93fff},
	{TCM_SIZE_96KB,		DISABLE, 		DISABLE, 	CACHE_WWR_4WAY,		CACHE_WWR_4WAY,		0x80000,		0x97fff},
};

/*
* @brif	Global Reset Configuration.
*	 by Users.
*/
BOOT_RAM_DATA_SECTION
GRST_ConfDef GRST_Config[] = {
	/*rst,					State*/
	{LSYS_BIT_POR_HP_DDRPHY,		ENABLE},
	//{LSYS_BIT_POR_LP_BTON,		DISABLE},	/* not to enable BTON by default, and wait 50us before release iso */
	{LSYS_BIT_POR_HP_PLAT,			ENABLE},
	{LSYS_BIT_POR_LP_PLAT,			ENABLE},

	{0xFFFFFFFF,					ENABLE},
};

/*
* @brif	RSIP Mask Configuration.
*	There are 8 RSIP OTF entries totally. Entry 0 is already used by System Data, Entry 3 is reserved by Realtek.
*	Only Entry 1 & Entry 2 can be used by Users.
*	BaseAddr: start address for RSIP Mask, should be 4KB aligned
*	EndAddr: size of the mask area, unit is 4KB
*	OTFType: OTF dec type: ctr or mix mode
*	OTFKeySize: size of the key: 128,192 or 256
*	IVNum: entry Iv num: 0-7
*/
BOOT_RAM_DATA_SECTION
RSIP_OTFDef RSIP_OTF_Config[] = {
	/*BaseAddr,		EndAddr*/
	{0x08001000,	0x08001000,		OTF_XTS_MODE,	OTF_KEY_256,	0},  	//Entry 0: 4K system data & 4K backup & DPK

	/* customer can set here */
	{0xFFFFFFFF,	0xFFFFFFFF,		0,				0,				0},  	//Entry 1: can be used by users
	{0xFFFFFFFF,	0xFFFFFFFF,		0,				0,				0}, 	//Entry 2: can be used by users
	{0xFFFFFFFF, 	0xFFFFFFFF,		0,				0,				0}, 	//Entry 3: can be used by users
	{0xFFFFFFFF,	0xFFFFFFFF, 	0,				0,				0}, 	//Entry 4: can be used by users
	{0xFFFFFFFF,	0xFFFFFFFF, 	0,				0,				0}, 	//Entry 5: can be used by users
	{0xFFFFFFFF,	0xFFFFFFFF, 	0,				0,				0}, 	//Entry 6: can be used by users
	{0xFFFFFFFF,	0xFFFFFFFF, 	0,				0,				0}, 	//Entry 7: Reserved by Realtek. If RDP is not used, this entry can be used by users.

	/* End */
	{0xFFFFFFFF, 	0xFFFFFFFF,		0,				0,				0},
};
/**
* @brif  GPIO force OTA1 as image2. 0xFF means force OTA1 trigger is disabled.
* 	BIT[7]: active level, 0 is low level active, 1 is high level active,
*	BIT[6:5]: port, 0 is PORT_A, 1 is PORT_B
*	BIT[4:0]: pin num 0~31
*/
BOOT_RAM_DATA_SECTION
u8 Force_OTA1_GPIO = 0xFF;

/**
* @brif  boot log enable or disable.
* 	FALSE: disable
*	TRUE: enable
*/
BOOT_RAM_DATA_SECTION
u8 Boot_Log_En = TRUE;

/**
* @brif  boot psram enable or disable.
* 	FALSE: disable
*	TRUE: enable
*/
//BOOT_RAM_DATA_SECTION
//u8 Boot_PSRAM_En = FALSE;

/**
* @brif  WB or APM PSRAM.
* 	FALSE: WB
*	TRUE: APM
*/
BOOT_RAM_DATA_SECTION
u8 Boot_PSRAM_APM = TRUE;

/**
* @brif  Firmware verify callback handler.
*	If users need to verify checksum/hash for image2, implement the function and assign the address
*	to this function pointer.
* @param  None
* @retval 1: Firmware verify successfully, 0: verify failed
*/
BOOT_RAM_DATA_SECTION
FuncPtr FwCheckCallback = NULL;

/**
* @brif  Firmware select hook.
*	If users need to implement own OTA select method, implement the function and assign the address
*	to this function pointer.
* @param  None
* @retval 0: both firmwares are invalid, select none, 1: boot from OTA1, 2: boot from OTA2
*/
BOOT_RAM_DATA_SECTION
FuncPtr OTASelectHook = NULL;


BOOT_RAM_DATA_SECTION
u32 Cert_PKHash_OTP_ADDR = SEC_PKKEY_PK1_0;

/**
* @brif  TCM size select.
*	If users need to set the TCM size share from cache.
*	TCM_SIZE_96KB: 96KB TCM, no cache
*	TCM_SIZE_80KB: 80KB TCM,  0KB D-Cache, 16KB I-Cache
*	TCM_SIZE_64KB: 64KB TCM, 16KB D-Cache, 16KB I-Cache
*	TCM_SIZE_48KB: 48KB TCM, 16KB D-Cache, 32KB I-Cache
*	TCM_SIZE_32KB: 32KB TCM, 16KB D-Cache, 48KB I-Cache
*	TCM_SIZE_16KB: 16KB TCM, 32KB D-Cache, 48KB I-Cache
*	TCM_SIZE_0KB :  0KB TCM, 32KB D-Cache, 64KB I-Cache
* @note: cache way restrict setting and TCM memory setting can only choose one
*/
BOOT_RAM_DATA_SECTION
u32 Boot_TCM_Size = TCM_SIZE_0KB;

/*
* @brif	CACHE way restrict setting
* @param  WRR: ways can be used for restrict range.
*		   This parameter can be one of the following values:
*			 CACHE_WWR_1WAY: 1 way for restrict range.
*			 CACHE_WWR_2WAY: 2 way for restrict range.
*			 CACHE_WWR_3WAY: 3 way for restrict range.
*			 CACHE_WWR_4WAY: 4 way for restrict range.
* @param WRR_BASE: way restrict base address, 64KB align.
* @param WRR_TOP: way restrict top address, 64KB align.
* @note: if MPU set the range non-cacheable, WRR setting is invalid
* @note: cache way restrict setting and TCM memory setting can only choose one
*/
CACHEWRR_TypeDef CACHEWRR_Def[2] = {
//	WRR_EN		WRR				WRR_BASE		WRR_TOP
	{DISABLE,	CACHE_WWR_4WAY,		0x00000000,		0xffff0000},	//for ICache Setting
	{DISABLE,	CACHE_WWR_4WAY,		0x00000000,		0xffff0000},	//for DCache Setting
};

BOOT_RAM_DATA_SECTION
u8 Boot_AP_Enbale = ENABLE;

BOOT_RAM_DATA_SECTION
u8 Boot_MemSwr_Only = DISABLE;

// for km4, max 360MHz under 1.0v, max 280MHz under 0.9v
// for AP, max 1480MHz under 1.0v, max 1080MHz under 0.9v
// NP PLL can be 800MHz~1000MHz
SocClk_Info_TypeDef SocClk_Info[] = {
	/* NPPLL_CLK,	APPLL_Clk,			Vol_Type,	KM4_CPU_CKD,	AP_CPU_CKD*/
#if defined(CONFIG_CLINTWOOD ) && CONFIG_CLINTWOOD
	{NPPLL_1000M,	APPLL_NULL,		VOL_09,			CLKDIV(4),		CLKDIV(1) | ISNPPLL},	// keep a balance between power and performance
#else
	{NPPLL_1000M,	APPLL_1480M,	VOL_10,			CLKDIV(3),		CLKDIV(1) | ISAPPLL},	//best porformance
#endif
	//{NPPLL_800M,	APPLL_NULL,		VOL_09,			CLKDIV(3),		CLKDIV(1) | ISNPPLL},

};

/**
* @brif  Loguart AGG enable or disable
* 	FALSE: disable
*	TRUE: enable
*/
BOOT_RAM_DATA_SECTION
u8 Boot_Agg_En = FALSE;

