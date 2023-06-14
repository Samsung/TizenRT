/**
  ******************************************************************************
  * @file    rtl8721d_rcc_rom.c
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


#include "ameba_soc.h"

/**
  * @brief  Enables or disables the APB peripheral clock and function
  * @param  APBPeriph: specifies the APB peripheral to gates its clock.
  *      this parameter can be one of @ref UART_IRDA_PULSE_SHIFT_define
  * @param  APBPeriph_Clock: specifies the APB peripheral clock config.
  *      this parameter can be one of @ref RCC_FUNC_Exported_Constants
  * @param  NewState: new state of the specified peripheral clock.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
__weak HAL_ROM_TEXT_SECTION
void RCC_PeriphClockCmd(u32 APBPeriph_in, u32 APBPeriph_Clock_in, u8 NewState)
{
	u32 ClkRegIndx = (APBPeriph_Clock_in >> 30) & 0x03;
	u32 APBPeriph_Clock = APBPeriph_Clock_in & (~(BIT(31) | BIT(30)));

	u32 FuncRegIndx = (APBPeriph_in >> 30) & 0x03;
	u32 APBPeriph = APBPeriph_in & (~(BIT(31) | BIT(30)));

	u32 Reg;
	u32 TempVal = 0;

	//DBG_8195A("RCC_PeriphClockCmd %x %x %x\n", APBPeriph_in, APBPeriph_Clock_in, NewState);

	//clock
	switch (ClkRegIndx) {
	case 0x0:
		Reg = REG_LSYS_CKE_GRP0;
		break;
	case 0x1:
		Reg = REG_LSYS_CKE_GRP1;
		break;
	case 0x2:
		Reg = REG_LSYS_CKE_GRP2;
		break;
	case 0x3:
		Reg = REG_AON_CLK;
		break;
	}

	if (APBPeriph_Clock_in != APBPeriph_CLOCK_NULL) {
		if (NewState == ENABLE) {
			TempVal = HAL_READ32(SYSTEM_CTRL_BASE_LP, Reg);
			TempVal |= APBPeriph_Clock;
			HAL_WRITE32(SYSTEM_CTRL_BASE_LP, Reg, TempVal);
		} else {
			TempVal = HAL_READ32(SYSTEM_CTRL_BASE_LP, Reg);
			TempVal &= ~APBPeriph_Clock;
			HAL_WRITE32(SYSTEM_CTRL_BASE_LP, Reg, TempVal);
		}
	}

	//function
	switch (FuncRegIndx) {
	case 0x0:
		Reg = REG_LSYS_FEN_GRP0;
		break;
	case 0x1:
		Reg = REG_LSYS_FEN_GRP1;
		break;
	case 0x2:
		Reg = REG_LSYS_FEN_GRP2;
		break;
	case 0x3:
		Reg = REG_AON_FEN;
		break;
	}

	if (APBPeriph_in != APBPeriph_NULL) {
		if (NewState == ENABLE) {
			TempVal = HAL_READ32(SYSTEM_CTRL_BASE_LP, Reg);
			TempVal |= APBPeriph;
			HAL_WRITE32(SYSTEM_CTRL_BASE_LP, Reg, TempVal);
		} else {
			TempVal = HAL_READ32(SYSTEM_CTRL_BASE_LP, Reg);
			TempVal &= ~APBPeriph;
			HAL_WRITE32(SYSTEM_CTRL_BASE_LP, Reg, TempVal);
		}
	}

	DelayUs(10);

	return;
}

/**
  * @brief    Configure UART RX Clock.
  * @param  UARTx: where x can be 0/1/2.
  * @param  Source:  This parameter can be one of the following values:
  *                            @arg UART_RX_CLK_XTAL_40M
  *                            @arg UART_RX_CLK_OSC_LP(2M)
  *                            @arg UART_RX_CLK_XTAL_LP(2M)
  * @retval   None UART_LPRxIPClockSet
  */
__weak HAL_ROM_TEXT_SECTION
void RCC_PeriphClockSource_UART(UART_TypeDef *UARTx, u32 Source)
{
	u32 RegValue = 0;
	u32 UartClk = 0;
	u32 BitIdx = 0;
	u32 RegValue2 = 0;

	/* select 40M or 2M */
	RegValue = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0);
	UartClk = LSYS_GET_CKSL_UART(RegValue);
	RegValue2 = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKD_GRP1);

	if (UARTx == UART0_DEV) {
		BitIdx = 0;
	} else if (UARTx == UART1_DEV) {
		BitIdx = 1;
	} else if (UARTx == UART2_DEV) {
		BitIdx = 2;
	}


	if (Source == UART_RX_CLK_XTAL_40M) {
		UartClk &= ~(BIT(BitIdx));//40MHz
	} else {
		UartClk |= (BIT(BitIdx));//XTAL2M or OSC2M

		if (Source == UART_RX_CLK_XTAL_LP) {
			RegValue |= (LSYS_BIT_CKSL_UART2M);//XTAL2M
			RegValue2 |= (LSYS_BIT_CKD_XTAL_2M_EN);//XTAL2M DIVISION ENABLE
		} else {
			RegValue &= ~(LSYS_BIT_CKSL_UART2M);//OSC2M
			RegValue2 &= ~(LSYS_BIT_CKD_XTAL_2M_EN);//XTAL2M DIVISION DISABLE
		}
	}

	RegValue &= ~LSYS_MASK_CKSL_UART;
	RegValue |= LSYS_CKSL_UART(UartClk);

	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0, RegValue);
	DelayUs(3); // delay for xtal2M --> osc2M, xtal2M is needed for sync
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKD_GRP1, RegValue2);
}

/**
  * @brief    Configure SPIC Clock.
  * @param  Source:  This parameter can be one of the following values:
  *                            @arg BIT_LSYS_CKSL_SPIC_OSC4M
  *                            @arg BIT_LSYS_CKSL_SPIC_XTAL
  *                            @arg BIT_LSYS_CKSL_SPIC_PLL
  * @retval   None
  * @note    Used to switch SPIC clock
  */
__weak HAL_ROM_TEXT_SECTION
void RCC_PeriphClockSource_SPIC(u32 Source)
{
	u32 Temp = 0;

	Temp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0);
	Temp &= ~(LSYS_MASK_CKSL_SPIC);
	Temp |= LSYS_CKSL_SPIC(Source);
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0, Temp);
}


/**
  * @brief    Configure PSRAM Clock.
  * @param  Source:  This parameter can be one of the following values:
  *                            @arg CKSL_PSRAM_XTAL
  *                            @arg CKSL_PSRAM_PLL
  * @retval   None
  * @note    Used to switch PSRAM clock
  */
__weak HAL_ROM_TEXT_SECTION
void RCC_PeriphClockSource_PSRAM(u32 Source)
{
	u32 Temp = 0;

	Temp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0);
	Temp &= ~(LSYS_BIT_CKSL_PSRAM);
	Temp |= LSYS_CKSL_PSRAM(Source);
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0, Temp);
}

/**
  * @brief    Configure sdm Clock.
  * @param  Source:  This parameter can be one of the following values:
  *                            @arg CKSL_SDM_OSC131K
  *                            @arg CKSL_SDM_XTAL_LPS312P5K
  * @retval   None
  * @note    Used to switch SDM clock
  */
__weak HAL_ROM_TEXT_SECTION
void RCC_PeriphClockSource_SDM(u32 Source)
{
	u32 Temp = 0;

	Temp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0);
	Temp &= ~(LSYS_BIT_CKSL_SDM);
	Temp |= LSYS_CKSL_SDM(Source);
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0, Temp);
}


/**
  * @brief    Configure vadmem Clock.
  * @param  Source:  This parameter can be one of the following values:
  *                            @arg CKSL_VADM_HS_PLFM
  *                            @arg CKSL_VADM_VAD
  * @retval   None
  * @note    Used to switch VADM clock
  */
__weak HAL_ROM_TEXT_SECTION
void RCC_PeriphClockSource_VADMEM(u32 Source)
{
	u32 Temp = 0;

	Temp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0);
	Temp &= ~(LSYS_BIT_CKSL_VADM);
	Temp |= LSYS_CKSL_VADM(Source);
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0, Temp);
}


/**
  * @brief    Configure audiocodec Clock.
  * @param  Source:  This parameter can be one of the following values:
  *                            @arg CKSL_AC_XTAL
  *                            @arg CKSL_AC_SYSPLL
  * @retval   None
  * @note    Used to switch AC clock
  */
__weak HAL_ROM_TEXT_SECTION
void RCC_PeriphClockSource_AUDIOCODEC(u32 Source)
{
	u32 Temp = 0;

	Temp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0);
	Temp &= ~(LSYS_BIT_CKSL_AC);
	Temp |= LSYS_CKSL_AC(Source);
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0, Temp);
}


/**
  * @brief    Configure ctc Clock.
  * @param  Source:  This parameter can be one of the following values:
  *                            @arg CKSL_CTC_131K
  *                            @arg CKSL_CTC_LS_APB_CLK
  * @retval   None
  * @note    Used to switch CTC clock
  */
__weak HAL_ROM_TEXT_SECTION
void RCC_PeriphClockSource_CTC(u32 Source)
{
	u32 Temp = 0;

	Temp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0);
	Temp &= ~(LSYS_BIT_CKSL_CTC);
	Temp |= LSYS_CKSL_CTC(Source);
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0, Temp);
}



/**
  * @brief    Configure adc Clock.
  * @param  Source:  This parameter can be one of the following values:
  *                            @arg CKSL_ADC_LS_APB
  *                            @arg CKSL_ADC_OSC2M
  * @retval   None
  * @note    Used to switch ADC clock
  */
__weak HAL_ROM_TEXT_SECTION
void RCC_PeriphClockSource_ADC(u32 Source)
{
	u32 Temp = 0;

	Temp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0);
	Temp &= ~(LSYS_BIT_CKSL_ADC);
	Temp |= LSYS_CKSL_ADC(Source);
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0, Temp);
}


/**
  * @brief    Configure gpio Clock.
  * @param  Source:  This parameter can be one of the following values:
  *                            @arg CKSL_GPIO_LS_APB
  *                            @arg CKSL_GPIO_32K
  * @retval   None
  * @note    Used to switch GPIO clock
  */
__weak HAL_ROM_TEXT_SECTION
void RCC_PeriphClockSource_GPIO(u32 Source)
{
	u32 Temp = 0;

	Temp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0);
	Temp &= ~(LSYS_BIT_CKSL_GPIO);
	Temp |= LSYS_CKSL_GPIO(Source);
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0, Temp);
}


/**
  * @brief    Configure sport Clock.
  * @param  Source:  This parameter can be one of the following values:
  *                            @arg CKSL_I2S_PLL98M
  *                            @arg CKSL_I2S_PLL45M
  *                            @arg CKSL_I2S_PLL24M
  *                            @arg CKSL_I2S_XTAL40M
  * @retval   None
  * @note    Used to switch SPORT clock
  */
__weak HAL_ROM_TEXT_SECTION
void RCC_PeriphClockSource_SPORT(int Sportx, u32 Source)
{
	u32 RegValue = 0;
	u32 RegValue2 = 0;
	u32 SPORTClk = 0;

	RegValue = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0);
	RegValue2 = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKD_GRP1);
	SPORTClk = LSYS_GET_CKSL_SPORT(RegValue);

	if (Source == CKSL_I2S_XTAL40M) {
		SPORTClk &= ~(BIT(Sportx));
	} else {
		SPORTClk |= (BIT(Sportx));
		switch (Sportx) {
		case 0:
			RegValue2 &= ~(LSYS_MASK_CKSL_I2S0);
			RegValue2 |= LSYS_CKSL_I2S0(Source);
			break;
		case 1:
			RegValue2 &= ~(LSYS_MASK_CKSL_I2S1);
			RegValue2 |= LSYS_CKSL_I2S1(Source);
			break;
		case 2:
			RegValue2 &= ~(LSYS_MASK_CKSL_I2S2);
			RegValue2 |= LSYS_CKSL_I2S2(Source);
			break;
		case 3:
			RegValue2 &= ~(LSYS_MASK_CKSL_I2S3);
			RegValue2 |= LSYS_CKSL_I2S3(Source);
			break;
		default:
			break;
		}
	}

	RegValue &= ~LSYS_MASK_CKSL_SPORT;
	RegValue |= LSYS_CKSL_SPORT(SPORTClk);

	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0, RegValue);
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKD_GRP1, RegValue2);
}


/**
  * @brief    Configure 32K Clock.
  * @param  Source:  This parameter can be one of the following values:
  *                            @arg CKSL_32K_SDM32K
  *                            @arg CKSL_32K_EXTERNAL_32K
  */
__weak HAL_ROM_TEXT_SECTION
void RCC_PeriphClockSource_32K(u32 Source)
{
	u32 RegTmp = 0;

	RegTmp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_AON_CKSL);

	RegTmp &= ~(AON_MASK_CKSL_32K);
	RegTmp |= AON_CKSL_32K(Source);

	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_AON_CKSL, RegTmp);

}

/**
  * @brief    Configure LOGUART Clock.
  * @param  Source:  This parameter can be one of the following values:
  *                            @arg UARTLOG_CLK_XTAL_40M
  *                            @arg UARTLOG_CLK_OSC_LP(2M)
  *                            @arg UARTLOG_CLK_XTAL_LP(2M)
  * @retval   None
  */
__weak HAL_ROM_TEXT_SECTION
void RCC_PeriphClockSource_LOGUART(u32 Source)
{
	u32 RegValue = 0;
	u32 RegValue2 = 0;

	RegValue = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0);
	RegValue2 = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKD_GRP1);

	if (Source == UARTLOG_CLK_XTAL_40M) {
		RegValue &= ~(LSYS_BIT_CKSL_LOGUART);//40MHz
	} else {
		RegValue |= LSYS_BIT_CKSL_LOGUART;//XTAL2M or OSC2M

		if (Source == UARTLOG_CLK_XTAL_LP) {
			RegValue |= LSYS_BIT_CKSL_LOGUART2M;//XTAL2M
			RegValue2 |= (LSYS_BIT_CKD_XTAL_2M_EN);//XTAL2M DIVISION ENABLE
		} else {
			RegValue &= ~(LSYS_BIT_CKSL_LOGUART2M);//OSC2M
			RegValue2 &= ~(LSYS_BIT_CKD_XTAL_2M_EN);//XTAL2M DIVISION DISABLE
		}
	}

	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0, RegValue);
	DelayUs(3); // delay for xtal2M --> osc2M, xtal2M is needed for sync
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKD_GRP1, RegValue2);
}

/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
