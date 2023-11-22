/**
  ******************************************************************************
  * @file    ameba_clk.h
  * @author
  * @version V1.0.0
  * @date    2016-05-17
  * @brief   This file provides firmware functions to manage the following
  *          functionalities of clock control:
  *           - NCO32K clock
  *           - NCO8M clock
  *           - CPU clock
  *           - XTAL clock get
  *           - OSC32K clock
  *           - EXT32K clock
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef _AMEBA_CLK_H_
#define _AMEBA_CLK_H_

/** @addtogroup AmebaD_Platform
  * @{
  */

/** @defgroup CLOCK
  * @brief CLOCK driver modules
  * @{
  */

/** @addtogroup CLOCK
  * @verbatim
  *****************************************************************************************
  * NCO32K
  *****************************************************************************************
  *	-RTC clock in
  *	-TIM0-TIM3 clock in
  *	-WIFI 32K clock in
  *****************************************************************************************
  * OSC32K OSC8M
  *****************************************************************************************
  *	-OSC32K is used to calibration OSC8M
  *	-OSC8M is used for LP UART when SOC suspend and close XTAL
  *****************************************************************************************
  *****************************************************************************************
  * NCO8M
  *****************************************************************************************
  *	-used for LP UART when SOC active
  *	-Clock in is XTAL (40MHz)
  *	-Clock out is 8MHz
  *
  *****************************************************************************************
  * HS CPU clock
  *****************************************************************************************
  *	-CLK_KM4_200M: 200MHz
  *	-CLK_KM4_100M: 100MHz
  *	-CLK_KM4_50M: 50MHz
  *	-CLK_KM4_25M: 25MHz
  *	-CLK_KM4_XTAL: XTAL
  *
  *****************************************************************************************
  * XTAL clock
  *****************************************************************************************
  *	-Get XTAL clock from EFUSE setting:
  *	-40000000
  *	-25000000
  *	-13000000
  *	-19200000
  *	-20000000
  *	-26000000
  *	-38400000
  *	-17664000
  *	-16000000
  *	-14318000
  *	-12000000
  *	-52000000
  *	-48000000
  *	-26000000
  *	-27000000
  *	-24000000

  *****************************************************************************************
  * EXT32K
  *****************************************************************************************
  *	-External 32K: 32K clock from external 32k source
  *	-Internal 32K: 32K clock from internal 32K source: NCO32K
  *
  *****************************************************************************************
  * @endverbatim
  */

/* Exported constants --------------------------------------------------------*/

/** @defgroup CLK_Exported_Constants CLK Exported Constants
  * @{
  */

/** @defgroup LP_CPU_CLK_definitions
  * @{
  */
#define CLK_KM0_ANA_4M				0x00 /* if XTAL OFF, default config */
#define CLK_KM0_XTAL_40M			0x02 /* if XTAL is 40MHz, we should use it */
#define CLK_KM0_XTAL_25M			0x12
#define CLK_KM0_XTAL_13M			0x22
#define CLK_KM0_XTAL_19P2M			0x32
#define CLK_KM0_XTAL_20M			0x42
#define CLK_KM0_XTAL_26M			0x52 /* if XTAL is 26MHz, we can use it */
#define CLK_KM0_XTAL_38P4M			0x62
#define CLK_KM0_XTAL_17P6M			0x72
#define CLK_KM0_XTAL_16M			0x82
#define CLK_KM0_XTAL_14P1M			0x92
#define CLK_KM0_XTAL_12M			0xA2
#define CLK_KM0_XTAL_52M			0xB2
#define CLK_KM0_XTAL_48M			0xC2
#define CLK_KM0_XTAL_27M			0xD2
#define CLK_KM0_XTAL_24M			0xE2
#define CLK_KM0_GET_LSOC_SEL(x)			((u8)((x & 0x0F)))
#define CLK_KM0_GET_XTAL_SEL(x)			((u8)(((x >> 4) & 0x0F)))
/**
  * @}
  */

/** @defgroup HS_CPU_CLK_definitions
  * @{
  */
#define CLK_KM4_100M		7
#define CLK_KM4_114P3M		6
#define CLK_KM4_133P3M		5
#define CLK_KM4_160M		4
#define CLK_KM4_200M		3
#define CLK_KM4_266P7M		2
#define CLK_KM4_400M		1
#define CLK_KM4_XTAL		0
/**
  * @}
  */

/** @defgroup AP_CPU_CLK_definitions
  * @{
  */
#define CLK_CA7_APPLLDIV2_150M		11
#define CLK_CA7_APPLLDIV2_200M		10
#define CLK_CA7_APPLLDIV2_300M		9
#define CLK_CA7_APPLLDIV2_600M		8
#define CLK_CA7_NPPLL_150M			7
#define CLK_CA7_NPPLL_200M			6
#define CLK_CA7_NPPLL_300M			5
#define CLK_CA7_NPPLL_600M			4
#define CLK_CA7_APPLL_300M			3
#define CLK_CA7_APPLL_400M			2
#define CLK_CA7_APPLL_600M			1
#define CLK_CA7_APPLL_1P2G			0
/**
  * @}
  */

/** @defgroup PLL_SEL_definitions
  * @{
  */
#define PLL_I2S		0
#define PLL_PCM		1

/** @defgroup PLL_ClkTune_definitions
  * @{
  */
#define PLL_AUTO		0
#define PLL_FASTER		1
#define PLL_SLOWER		2

/**
  * @}
  */

/** @defgroup CLK_Exported_Constants CLK Exported Constants
  * @{
  */
#define MHZ_TICK_CNT 		(1000000)
/**
  * @}
  */

/** @defgroup PLL_BandGap_Mask_definitions
  * @{
  */
#define PLL_BG_POW_MASK	((u32)0x00000005)
/**
  * @}
  */

/** @defgroup AP_PLL_SDM_DIVN_definitions, SmartSpec is 1.0G ~ 1.6G
  * @{
  */
#define IS_AP_PLL_SDM_DIVN(Option)		(((Option) <= 0x26) && ((Option) >= 0x17))
/**
  * @}
  */

/** @defgroup HBUS_CLK_definitions
  * @{
  */
#define CKSL_HBUS_XTAL  0
#define CKSL_HBUS_PLL        1
/**
  * @}
  */

/** @defgroup CAL_CLK_SEL_definitions
  * @{
  */
#define OSC4M_CAL_CLK		0
#define AON100K_CAL_CLK		1
#define AON128K_CAL_CLK		2
/**
  * @}
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup SDM32K_CLK_Exported_Functions SDM32K_CLK Exported Functions
  * @{
  */
_LONG_CALL_ void SDM32K_Enable(void);
_LONG_CALL_ void SDM32K_TimerCalEnable(u32 RTCCalibration);
/**
  * @}
  */


/** @defgroup CPU_CLK_Exported_Functions CPU_CLK Exported Functions
  * @{
  */
_LONG_CALL_ u32 CPU_ClkGet(void);
_LONG_CALL_ void CPU_ClkSet(u8  CpuType);
/**
  * @}
  */

/** @defgroup XTAL_CLK_Exported_Functions XTAL_CLK Exported Functions
  * @{
  */
_LONG_CALL_ u32 XTAL_ClkGet(void);
/**
  * @}
  */

/** @defgroup PLL_Exported_Functions PLL Exported Functions
  * @{
  */
_LONG_CALL_ void PLL_I2S_Div(int Sportx, u32 div);
_LONG_CALL_ void PLL_I2S_98P304M(u32 NewState);
_LONG_CALL_ void PLL_I2S_45P158M(u32 NewState);
_LONG_CALL_ float PLL_I2S_98P304M_ClkTune(float ppm, u32 action);
_LONG_CALL_ float PLL_I2S_45P158M_ClkTune(float ppm, u32 action);
_LONG_CALL_ void PLL_NP_ClkSet(u32 PllClk);
_LONG_CALL_ void PLL_NP(u32 NewState);
_LONG_CALL_ void PLL_AP_ClkSet(u32 PllClk);
_LONG_CALL_ void PLL_AP(u32 NewState);
_LONG_CALL_ void PLL_AP_ClkTune(u32 Option);
/**
  * @}
  */


/** @defgroup HAPB_CLK_Exported_Functions
  * @{
  */
_LONG_CALL_ void HBUS_ClkSet(u32 Source);
/**
  * @}
  */


/* Registers Definitions --------------------------------------------------------*/
/**************************************************************************//**
 * @defgroup CLK_Register_Definitions CLK Register Definitions
 * @{
 *****************************************************************************/

/**************************************************************************//**
 * @defgroup REG_SYS_SYSPLL_CTRL2
 * @{
 *****************************************************************************/
#define BIT_SYS_SYSPLL_CK_ADC_EN		(0x00000001 << 25) /*!< Set ADC PLL EN */
/** @} */

/** @} */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/* Other definations --------------------------------------------------------*/
void OSC4M_Init(void);
void OSC4M_R_Set(u32 setbit, u32 clearbit);
void OSC4M_VCM_Set(u32 value);
u32 OSC4M_Calibration(u32 ppm_limit);
u32 OSC131K_Calibration(u32 ppm_limit);
void OSC131_R_Set(u32 setbit, u32 clearbit);
u32 OSC_CalResult_Get(u8 cal_clk);

void XTAL_PDCK(void);
void XTAL_INIT(void);
void XTAL_LPSDIG(u32 status);
void CLK_SWITCH_XTAL(u32 State);

#define CA7_GET_CLKSEL(x)		((x >> 2) & 0x3)
#define CA7_GET_CLKDIV(x)		(x & 0x3)

#define CLKDIV(x)			(x)

#define NPPLL_1000M		1000
#define NPPLL_920M		920
#define NPPLL_960M		960
#define NPPLL_800M		800

#define APPLL_800M			800
#define APPLL_1000M			1000
#define APPLL_1080M			1080
#define APPLL_1200M			1200
#define APPLL_1480M			1400	// for Acut Vol when not cal
#define APPLL_NULL			0

#define ISNPPLL				0
#define ISAPPLL				0x80000000
#endif //_AMEBA_CLK_H_
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
