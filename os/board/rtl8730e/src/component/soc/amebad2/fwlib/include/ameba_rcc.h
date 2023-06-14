/**
  ******************************************************************************
  * @file    ameba_rcc.h
  * @author
  * @version V1.0.0
  * @date    2016-05-17
  * @brief   This file contains all the functions prototypes for peripheral reset and clock control driver.
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef _AMEBA_HP_RCC_H_
#define _AMEBA_HP_RCC_H_

/** @addtogroup AmebaD_Platform
  * @{
  */

/** @defgroup RCC
  * @brief RCC driver modules
  * @{
  */


/** @defgroup HS_RCC
  * @verbatim
  *****************************************************************************************
  * Introduction
  *****************************************************************************************
  *		- functions prototypes for peripheral reset and clock control driver.
  *
  *****************************************************************************************
  *
  *****************************************************************************************
  * how to use
  *****************************************************************************************
  * use UART0 as example:
  *		RCC_PeriphClockCmd(APBPeriph_UART0, APBPeriph_UART0_CLOCK, ENABLE);
  *
  *****************************************************************************************
  * @endverbatim
  */

/** @addtogroup HS_RCC
  * @brief HS_RCC driver modules
  * @{
  */

/* Exported constants --------------------------------------------------------*/

#define LS_FUNC_EN0						0x00U //LP syson: 0x210
#define LS_FUNC_EN1						0x01U //LP syson: 0x214
#define LS_FUNC_EN2						0x02U //LP syson: 0x218
#define LS_FUNC_EN3						0x03U //LP AON: 0x008

#define LS_CLK_CTRL0					0x00U //LP syson: 0x220
#define LS_CLK_CTRL1					0x01U //LP syson: 0x224
#define LS_CLK_CTRL2					0x02U //LP syson: 0x228
#define LS_CLK_CTRL3					0x03U //LP AON: 0x00C

#define APBPeriph_NULL					0x0
#define APBPeriph_CLOCK_NULL			0x0



/** @defgroup SPIC_Clock_Source_define
  * @{
  */
#define BIT_LSYS_CKSL_SPIC_OSC4M		0x00
#define BIT_LSYS_CKSL_SPIC_XTAL		0x01
#define BIT_LSYS_CKSL_SPIC_PLL			0x03
/**
  * @}
  */

/** @defgroup PSRAM_Clock_Source_define
  * @{
  */
#define CKSL_PSRAM_XTAL				0x00
#define CKSL_PSRAM_PLL					0x01
/**
  * @}
  */


/** @defgroup SDM_Clock_Source_define
  * @{
  */
#define CKSL_SDM_OSC131K				0x00
#define CKSL_SDM_XTAL_LPS312P5K			0x01
/**
  * @}
  */


/** @defgroup VADMEM_Clock_Source_define
  * @{
  */
#define CKSL_VADM_HS_PLFM				0x00
#define CKSL_VADM_VAD					0x01
/**
  * @}
  */

/** @defgroup AUDIOCODEC_Clock_Source_define
  * @{
  */
#define CKSL_AC_SYSPLL					0x00
#define CKSL_AC_XTAL					0x01
/**
  * @}
  */

/** @defgroup CTC_Clock_Source_define
  * @{
  */
#define CKSL_CTC_131K					0x00
#define CKSL_CTC_LS_APB_CLK			0x01
/**
  * @}
  */


/** @defgroup ADC_Clock_Source_define
  * @{
  */
#define CKSL_ADC_LS_APB				0x00
#define CKSL_ADC_OSC2M					0x01
/**
  * @}
  */

/** @defgroup GPIO_Clock_Source_define
  * @{
  */
#define CKSL_GPIO_LS_APB				0x00
#define CKSL_GPIO_32K					0x01
/**
  * @}
  */


/** @defgroup I2S_Clock_Source_define
  * @{
  */
#define CKSL_I2S_PLL98M				0x00
#define CKSL_I2S_PLL45M				0x01
#define CKSL_I2S_PLL24M				0x03
#define CKSL_I2S_XTAL40M			0x04
/**
  * @}
  */

/** @defgroup UART_RX_Clock_Source_define
  * @{
  */

#define UART_RX_CLK_XTAL_40M	((u32)0x00000000)
#define UART_RX_CLK_OSC_LP		((u32)0x00000001) //(2M)
#define UART_RX_CLK_XTAL_LP		((u32)0x00000002) //(2M)
/**
  * @}
  */

/** @defgroup 32K_Clock_Source_define
  * @{
  */
#define CKSL_32K_SDM32K			0x00
#define CKSL_32K_EXTERNAL_32K		0x03

/**
  * @}
  */

/** @defgroup UARTLOG_Clock_Source_define
  * @{
  */

#define UARTLOG_CLK_XTAL_40M	((u32)0x00000000)
#define UARTLOG_CLK_OSC_LP		((u32)0x00000001) //(2M)
#define UARTLOG_CLK_XTAL_LP		((u32)0x00000002) //(2M)
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup HS_RCC_Exported_Functions HS_RCC Exported Functions
  * @{
  */
_LONG_CALL_ void RCC_PeriphClockCmd(u32 APBPeriph, u32 APBPeriph_Clock, u8 NewState);
_LONG_CALL_ void RCC_WIFIClockCmd(u8 NewState);
_LONG_CALL_ void RCC_PeriphClockSource_UART(UART_TypeDef *UARTx, u32 Source);
_LONG_CALL_ void RCC_PeriphClockSource_SPIC(u32 Source);
_LONG_CALL_ void RCC_PeriphClockSource_PSRAM(u32 Source);
_LONG_CALL_ void RCC_PeriphClockSource_SPORT(int Sportx, u32 Source);
_LONG_CALL_ void RCC_PeriphClockSource_UART2M(u32 Source);
_LONG_CALL_ void RCC_PeriphClockSource_SDM(u32 Source);
_LONG_CALL_ void RCC_PeriphClockSource_VADMEM(u32 Source);
_LONG_CALL_ void RCC_PeriphClockSource_CTC(u32 Source);
_LONG_CALL_ void RCC_PeriphClockSource_GPIO(u32 Source);
_LONG_CALL_ void RCC_PeriphClockSource_AUDIOCODEC(u32 Source);
_LONG_CALL_ void RCC_PeriphClockSource_ADC(u32 Source);
_LONG_CALL_ void RCC_PeriphClockSource_32K(u32 Source);
_LONG_CALL_ void RCC_PeriphClockSource_LOGUART(u32 Source);


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/* Registers Definitions --------------------------------------------------------*/
/* Other definations --------------------------------------------------------*/

#endif /* _AMEBA_HP_RCC_H_ */
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/

