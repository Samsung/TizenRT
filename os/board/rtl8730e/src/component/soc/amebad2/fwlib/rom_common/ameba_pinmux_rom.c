/**
  ******************************************************************************
  * @file    rtl8721d_pinmux_rom.c
  * @author
  * @version V1.0.0
  * @date    2016-05-17
  * @brief   This file provides firmware functions to manage the following
  *          functionalities of pinmux:
  *           - pad pull control
  *           - pinmux
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#include "ameba_soc.h"

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @defgroup PAD
  * @brief  PAD driver modules
  * @{
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup PAD_Exported_Functions PAD Exported Functions
  * @{
  */
/**
  *  @brief Set the pad Driving Strength.
  *  @param PinName : value of @ref PINMUX_Pin_Name_definitions.
  *  @param DrvStrength : PAD Driving Strength.This parameter can be one of the following values:
  *		 @arg PAD_DRV_ABILITITY_LOW
  *		 @arg PAD_DRV_ABILITITY_HIGH
  *  @retval None
  *  @note invalid for some pads
  *        SDIO pad driving strength is controlled by SDIO_PAD_CTRL
  */
__weak HAL_ROM_TEXT_SECTION
void PAD_DrvStrength(u8 PinName, u32 DrvStrength)
{
	u32 Temp = 0;

	/* get PADCTR */
	Temp = PINMUX->PADCTR[PinName];

	if (DrvStrength == PAD_DRV_ABILITITY_LOW) {
		/* clear Pin_Num Pull contrl */
		Temp &= ~PAD_BIT_GPIOx_E2;
	} else {
		Temp |= PAD_BIT_GPIOx_E2;
	}

	/* set PADCTR register */
	PINMUX->PADCTR[PinName] = Temp;
}

/**
  *  @brief Set the Internal pad pull type.
  *  @param PinName : value of @ref PINMUX_Pin_Name_definitions.
  *  @param PullType : the pull type for the pin.This parameter can be one of the following values:
  *		 @arg GPIO_PuPd_NOPULL
  *		 @arg GPIO_PuPd_DOWN
  *		 @arg GPIO_PuPd_UP
  *  @retval None
  */
__weak HAL_ROM_TEXT_SECTION
void PAD_PullCtrl(u8 PinName, u8 PullType)
{
	u32 Temp = 0;

	/* get PADCTR */
	Temp = PINMUX->PADCTR[PinName];

	/* clear Pin_Num Pull contrl */
	Temp &= ~(PAD_BIT_GPIOx_PU | PAD_BIT_GPIOx_PD);

	/* set needs Pull contrl */
	if (PullType == GPIO_PuPd_DOWN) {
		Temp |= PAD_BIT_GPIOx_PD;
	} else if (PullType == GPIO_PuPd_UP) {
		Temp |= PAD_BIT_GPIOx_PU;
	}

	/* set PADCTR register */
	PINMUX->PADCTR[PinName] = Temp;
}

/**
  *  @brief Set the Internal pad pull type in sleep and deepsleep mode.
  *    HW will switch to the sleep pull ctrl automatically when enter sleep or deepsleep mode.
  *    HW will switch to function pull ctrl automatically when wakes up from sleep mode
  *  @param PinName : value of @ref PINMUX_Pin_Name_definitions.
  *  @param PullType : the pull type for the pin.This parameter can be one of the following values:
  *		 @arg GPIO_PuPd_NOPULL
  *		 @arg GPIO_PuPd_DOWN
  *		 @arg GPIO_PuPd_UP
  *  @retval None
  */
__weak HAL_ROM_TEXT_SECTION
void PAD_SleepPullCtrl(u8 PinName, u8 PullType)
{
	u32 Temp = 0;

	/* get PADCTR */
	Temp = PINMUX->PADCTR[PinName];

	/* clear Pin_Num Pull contrl */
	Temp &= ~(PAD_BIT_GPIOx_PU_SLP | PAD_BIT_GPIOx_PD_SLP);

	/* set needs Pull contrl */
	if (PullType == GPIO_PuPd_DOWN) {
		Temp |= PAD_BIT_GPIOx_PD_SLP;
	} else if (PullType == GPIO_PuPd_UP) {
		Temp |= PAD_BIT_GPIOx_PU_SLP;
	}

	/* set PADCTR register */
	PINMUX->PADCTR[PinName] = Temp;
}

/**
  *  @brief Set the Internal pad Resistor type.
  *  @param PinName : value of @ref PINMUX_Pin_Name_definitions.
  *  @param PullType : the pull type for the pin.This parameter can be one of the following values:
  *		 @arg GPIO_Resistor_LARGE
  *		 @arg GPIO_Resistor_SMALL
  *  @retval None
  *  @note Only some pads support to change pull resistor type
  *  @note for some pads, PU resistor and PD resitor may be configured to respective values
  *  @note for other other pads, PU/PD resistor only support fixed value
  */
HAL_ROM_TEXT_SECTION
void PAD_ResistorCtrl(u8 PinName, u8 RType)
{
	u32 Temp = 0;

	/* get PADCTR */
	Temp = PINMUX->PADCTR[PinName];

	/* set resistor small */
	Temp |= PAD_BIT_GPIOx_PUPDC; /* by default is small */

	/* set large if needed */
	if (RType == GPIO_Resistor_LARGE) {
		Temp &= ~PAD_BIT_GPIOx_PUPDC;
	}

	/* set PADCTR register */
	PINMUX->PADCTR[PinName] = Temp;
}


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
  *  @brief Set the Internal pad shutdown or enable mode.
  *  @param PinName : value of @ref PINMUX_Pin_Name_definitions.
  *  @param NewStatus: enable or disable (shutdown)
  *  @retval None
  *  @note PAD_BIT_GPIOx_DIS 0: enable pad; 1: shutdown pad
  *  @note only valid for digital function, invalid for audio analog pads
  */
HAL_ROM_TEXT_SECTION
void PAD_CMD(u8 PinName, u8 NewStatus)
{
	u32 Temp = 0;

	/* get PADCTR */
	Temp = PINMUX->PADCTR[PinName];

	if (NewStatus == ENABLE) {
		Temp &= ~PAD_BIT_GPIOx_DIS;
	} else {
		Temp |= PAD_BIT_GPIOx_DIS;
	}

	/* set PADCTR register */
	PINMUX->PADCTR[PinName] = Temp;
}

/**
  *  @brief Set the function type of one pin.
  *  @param PinName : value of @ref PINMUX_Pin_Name_definitions.
  *  @param PinFunc : the function type for the pin, value of @ref PINMUX_Function_definitions.
  *  @retval None
  */
HAL_ROM_TEXT_SECTION
void _Pinmux_Config(u8 PinName, u32 PinFunc)
{
	u32 Temp = 0;

	/* get PADCTR */
	Temp = PINMUX->PADCTR[PinName];

	/* set needs function */
	Temp &= ~PAD_MASK_GPIOx_SEL;
	Temp |= PAD_GPIOx_SEL(PinFunc);

	/* set PADCTR register */
	PINMUX->PADCTR[PinName] = Temp;
}

/**
  *  @brief Get the function type of one pin.
  *  @param PinName : value of @ref PINMUX_Pin_Name_definitions.
  *  @retval : the function type for the pin, value of @ref PINMUX_Function_definitions.
  */
HAL_ROM_TEXT_SECTION
u32 Pinmux_ConfigGet(u8 PinName)
{
	u32 Temp = 0;

	/* get PADCTR */
	Temp = PINMUX->PADCTR[PinName];

	/* get function */
	Temp = PAD_GET_GPIOx_SEL(Temp);

	return Temp;
}

/**
  *  @brief Set UARTLOG pinmux.
  *  @param PinLocation: This parameter can be one of the following values:
  *		 @arg PINMUX_S0
  *  @param Operation: This parameter can be one of the following values:
  *		 @arg ON
  *		 @arg OFF
  *  @retval None
  */
HAL_ROM_TEXT_SECTION
void Pinmux_UartLogCtrl(u32  PinLocation, BOOL   Operation)
{
	u16 Pin_Func = PINMUX_FUNCTION_GPIO;

	assert_param(PinLocation == PINMUX_S0);

	if (Operation) {
		Pin_Func = PINMUX_FUNCTION_LOGUART;
	}

	PAD_PullCtrl(_PB_23, GPIO_PuPd_UP);
	PAD_PullCtrl(_PB_24, GPIO_PuPd_UP);

	Pinmux_Config(_PB_23, Pin_Func);
	Pinmux_Config(_PB_24, Pin_Func);
}

/**
  *  @brief Set SPIFLASH pad pull up.
  *  @param PinLocation: This parameter can be one of the following values:
  *		 @arg PINMUX_S0/PINMUX_S1
  *  @retval None
  */
HAL_ROM_TEXT_SECTION
void PAD_SpicCtrl(u32  PinLocation)
{
	if (PinLocation == PINMUX_S1) { //normal
		PAD_PullCtrl(_PC_6, GPIO_PuPd_UP);
		PAD_PullCtrl(_PC_1, GPIO_PuPd_UP);
		PAD_PullCtrl(_PC_2, GPIO_PuPd_UP);
		PAD_PullCtrl(_PC_3, GPIO_PuPd_UP);
		PAD_PullCtrl(_PC_4, GPIO_PuPd_UP);
		PAD_PullCtrl(_PC_5, GPIO_PuPd_UP);
	} else { //FT
		PAD_PullCtrl(_PC_0, GPIO_PuPd_UP);
		PAD_PullCtrl(_PB_27, GPIO_PuPd_UP);
		PAD_PullCtrl(_PB_28, GPIO_PuPd_UP);
		PAD_PullCtrl(_PB_29, GPIO_PuPd_UP);
		PAD_PullCtrl(_PB_30, GPIO_PuPd_UP);
		PAD_PullCtrl(_PB_31, GPIO_PuPd_UP);
	}
}

/**
  *  @brief Set SPIFLASH pinmux.
  *  @param PinLocation: This parameter can be one of the following values:
  *		 @arg PINMUX_S0/PINMUX_S1
  *  @param Operation: This parameter can be one of the following values:
  *		 @arg ON
  *		 @arg OFF
  *  @retval None
  */
HAL_ROM_TEXT_SECTION
void Pinmux_SpicCtrl(u32  PinLocation, BOOL Operation)
{
	u16 Pin_Func = PINMUX_FUNCTION_GPIO;

	if (Operation) {
		Pin_Func = PINMUX_FUNCTION_SPIF;
	}

	if (PinLocation == PINMUX_S1) { //normal
		PAD_PullCtrl(_PC_6, GPIO_PuPd_UP);
		PAD_PullCtrl(_PC_1, GPIO_PuPd_UP);
		PAD_PullCtrl(_PC_2, GPIO_PuPd_UP);
		PAD_PullCtrl(_PC_3, GPIO_PuPd_UP);
		PAD_PullCtrl(_PC_4, GPIO_PuPd_UP);
		PAD_PullCtrl(_PC_5, GPIO_PuPd_UP);

		Pinmux_Config(_PC_1, Pin_Func);//SPI_DATA3
		Pinmux_Config(_PC_2, Pin_Func);//SPI_CLK
		Pinmux_Config(_PC_3, Pin_Func);//SPI_DATA0
		Pinmux_Config(_PC_4, Pin_Func);//SPI_DATA2
		Pinmux_Config(_PC_5, Pin_Func);//SPI_DATA1
		Pinmux_Config(_PC_6, Pin_Func);//SPI_CS
	} else { //FT
		PAD_PullCtrl(_PC_0, GPIO_PuPd_UP);
		PAD_PullCtrl(_PB_27, GPIO_PuPd_UP);
		PAD_PullCtrl(_PB_28, GPIO_PuPd_UP);
		PAD_PullCtrl(_PB_29, GPIO_PuPd_UP);
		PAD_PullCtrl(_PB_30, GPIO_PuPd_UP);
		PAD_PullCtrl(_PB_31, GPIO_PuPd_UP);

		Pinmux_Config(_PB_27, Pin_Func);//SPI_DATA3
		Pinmux_Config(_PB_28, Pin_Func);//SPI_CLK
		Pinmux_Config(_PB_29, Pin_Func);//SPI_DATA0
		Pinmux_Config(_PB_30, Pin_Func);//SPI_DATA2
		Pinmux_Config(_PB_31, Pin_Func);//SPI_DATA1
		Pinmux_Config(_PC_0, Pin_Func);//SPI_CS
	}
}

/**
  *  @brief Turn off pinmux SWD function.
  *  @retval None
  */
HAL_ROM_TEXT_SECTION
void Pinmux_Swdoff(void)
{
	u32 Temp = 0;
	Temp = HAL_READ32(PINMUX_REG_BASE, REG_SWD_SDD_CTRL);
	Temp &= (~PAD_BIT_SWD_PMUX_EN);
	HAL_WRITE32(PINMUX_REG_BASE, REG_SWD_SDD_CTRL, Temp);
}

/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
