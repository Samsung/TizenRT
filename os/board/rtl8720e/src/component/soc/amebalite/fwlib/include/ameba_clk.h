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
  * SDM32K
  *****************************************************************************************
  *	-RTC clock in
  *	-TIM0-TIM8 clock in
  *	-WIFI 32K clock in

  *****************************************************************************************
  * KM4 CPU clock
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
#define MHZ_TICK_CNT (1000000)

/** @defgroup CPU_CLK_Source definitions
  * @{
  */

#define CLK_CPU_DPLL		3
#define CLK_CPU_MPLL		2
//#define CLK_CPU_LBUS		1 /* Both 0 and 1 are The same in HW*/
#define CLK_CPU_LBUS		0
#define CLK_CPU_XTAL		0 /* Nearly all the Time, Lbus the same with XTAL*/
/**
  * @}
  */



/** @defgroup PLL_SEL_definitions
  * @{
  */
#define PLL_I2S		0
#define PLL_PCM		1

/**
  * @}
  */

/** @defgroup PLL_ClkTune_definitions
  * @{
  */
#define PLL_AUTO		0
#define PLL_FASTER		1
#define PLL_SLOWER		2

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

/** @defgroup XTAL_MODE_SEL_definitions
  * @{
  */
#define XTAL_NM_MODE		2
#define XTAL_LPS_MODE		1
#define XTAL_HP_MODE		3
/**
  * @}
  */

/** @defgroup PLL_CLK_definitions
  * @{
  */
#define PLL_330M		((u32)330000000)	/*This is minimum value*/
#define PLL_442P368M	((u32)442368000)	/* generate 98.304M for I2S */
#define PLL_400M		((u32)400000000)
#define PLL_450M		((u32)450000000)
#define PLL_480M		((u32)480000000)
#define PLL_500M		((u32)500000000)
#define PLL_589P824M	((u32)589824000)	/* generate 98.304M for I2S */
#define PLL_600M		((u32)600000000)
#define PLL_660M		((u32)660000000)	/*This is maximum value*/
/**
  * @}
  */

/** @defgroup Soc_CLK_LIMIT_definitions
  * @{
  */

#define ISPLLM					0
#define ISPLLD					BIT7

#define CLKDIV(x)				(x)
#define CLKDIV_ROUND_UP(X, Y)	( ( ( X ) + ( Y ) - 1 ) / ( Y ) )

#define CPU_0P9V_CLK_LIMIT		(200 * MHZ_TICK_CNT)
#define CPU_1P0V_CLK_LIMIT		(240 * MHZ_TICK_CNT)

#define DSP_0P9V_CLK_LIMIT		(400 * MHZ_TICK_CNT)
#define DSP_1P0V_CLK_LIMIT		(500 * MHZ_TICK_CNT)

#define HBUS_0P9V_CLK_LIMIT		(100 * MHZ_TICK_CNT)
#define HBUS_1P0V_CLK_LIMIT		(150 * MHZ_TICK_CNT)

#define PSRAMC_CLK_LIMIT		(500 * MHZ_TICK_CNT)
#define SPIC_CLK_LIMIT			(240 * MHZ_TICK_CNT)
#define ECDSA_CLK_LIMIT			(100 * MHZ_TICK_CNT)

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
_LONG_CALL_ void CPU_ClkSrc_Set(u8  Source);
_LONG_CALL_  u32 CPU_ClkSrc_Get(void);

/*CPU clk should be sync with sram, can't set CPU by a simple API*/
//_LONG_CALL_ void CPU_ClkSet(u8  CpuType);
/**
  * @}
  */

/** @defgroup XTAL_CLK_Exported_Functions XTAL_CLK Exported Functions
  * @{
  */
_LONG_CALL_ u32 XTAL_ClkGet(void);
_LONG_CALL_ u32 PLL_ClkGet(u8 pll_type);
_LONG_CALL_ void PLL_ClkSet(u8 pll_type, u32 PllClk);
/**
  * @}
  */

/** @defgroup PLL_Exported_Functions PLL Exported Functions
  * @{
  */
_LONG_CALL_ void PLL_I2S_Div(int Sportx, u32 div);
_LONG_CALL_ void PLL_I2S_98P304M(u32 NewState);
_LONG_CALL_ void PLL_I2S_45P158M(u32 NewState);
_LONG_CALL_ void PLL_I2S_98P304M_ClkTune(u32 ppm, u32 action);
_LONG_CALL_ void PLL_I2S_45P158M_ClkTune(u32 ppm, u32 action);
_LONG_CALL_ void PLL_NP(u32 NewState);
_LONG_CALL_ void PLL_AP(u32 Option, u32 NewState);
_LONG_CALL_ void PLL_AP_ClkTune(u32 Option);

/**
  * @}defgroup PLL_Exported_Functions PLL Exported Functions
  */
_LONG_CALL_ void PLL_CPU(u32 status);
_LONG_CALL_ void PLL_DSP(u32 status);
_LONG_CALL_ void PLL_BG(u32 status);
_LONG_CALL_ bool PLL_State(u8 pll_type);
_LONG_CALL_ void PLL_ClkSet(u8 pll_type, u32 PllClk);



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

/** @defgroup PLL_BandGap_Mask_definitions
  * @{
  */
#define PLL_BG_POW_MASK	((u32)0x00000005)

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
u32 OSC4M_Calibration(u32 ppm_limit);
u32 OSC131K_Calibration(u32 ppm_limit);
void OSC4M_Init(void);
void XTAL_AACK(void);
void XTAL_PDCK(void);
void XTAL_INIT(void);
void CPU_ClkSet_NonOS(u32 Source);

#endif //_AMEBA_CLK_H_
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
