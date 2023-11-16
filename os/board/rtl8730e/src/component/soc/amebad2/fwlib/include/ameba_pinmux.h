/**
  ******************************************************************************
  * @file    ameba_pinmux.h
  * @author
  * @version V1.0.0
  * @date    2016-05-17
  * @brief   This file contains all the functions prototypes for the pinmux firmware
  *          library.
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef _HAL_AMEBA_PINMUX_
#define _HAL_AMEBA_PINMUX_

/** @addtogroup Ameba_Platform
  * @{
  */

/** @defgroup PIN
  * @brief PIN driver modules
  * @{
  */

/** @defgroup PINMUX
  * @brief PINMUX modules
  * @{
  */

/** @addtogroup PINMUX
  * @verbatim
  *****************************************************************************************
  * Introduction
  *****************************************************************************************
  *		-Every GPIO pin can be set to some function based on pinmux spec.
  *		-Every GPIO pin can set internal pull-up, pull-down based on pinmux spec.
  *
  *====================================================================
  * pad control Spec.
  *====================================================================
  *		-[31:18]		reserved
  *		-[17]			pull down resistor enable when system is in sleep
  *		-[16]			pull up resistor enable when system is in sleep
  *		-[13]			srew rate control
  *		-[12]			schmitt trigger enable
  *		-[11]			pad driving strength
  *		-[10]			pull resistor selection
  *		-[9]			pull down resistor enable when system is in active
  *		-[8]			pull up resistor enable when system is in active
  *		-[7:5]			reserved for function id extend
  *		-[4:0]			function id
  *
  *****************************************************************************************
  * How to use Pinmux
  *****************************************************************************************
  *		1. Set the Internal pad function type for  each pin using the follwoing function:
  *			Pinmux_Config(u8 PinName, u32 PinFunc)
  *
  *		2. Set the Internal pad pull type for each pin using the follwoing function:
  *			PAD_PullCtrl(u8 PinName, u8 PullType)
  *			PAD_SleepPullCtrl(u8 PinName, u8 PullType);
  *
  *****************************************************************************************
  * @endverbatim
  */

/* Exported constants --------------------------------------------------------*/

/** @defgroup PINMUX_Exported_Constants PINMUX Exported Constants
  * @{
  */

/** @defgroup PINMUX_Pin_Name_definitions
  * @note: Pin_Name = (((port)<<5)|(pin))
  * @{
  */
#define _PA_0		(0x00)	//0x00
#define _PA_1		(0x01)	//0x04
#define _PA_2		(0x02)	//0x08
#define _PA_3		(0x03)	//0x0C
#define _PA_4		(0x04)	//0x10
#define _PA_5		(0x05)	//0x14
#define _PA_6		(0x06)	//0x18
#define _PA_7		(0x07)	//0x1C
#define _PA_8		(0x08)	//0x20
#define _PA_9		(0x09)	//0x24
#define _PA_10		(0x0A)	//0x28
#define _PA_11		(0x0B)	//0x2C
#define _PA_12		(0x0C)	//0x30
#define _PA_13		(0x0D)	//0x34
#define _PA_14		(0x0E)	//0x38
#define _PA_15		(0x0F)	//0x3C
#define _PA_16		(0x10)	//0x40
#define _PA_17		(0x11)	//0x44
#define _PA_18		(0x12)	//0x48
#define _PA_19		(0x13)	//0x4C
#define _PA_20		(0x14)	//0x50
#define _PA_21		(0x15)	//0x54
#define _PA_22		(0x16)	//0x58
#define _PA_23		(0x17)	//0x5C
#define _PA_24		(0x18)	//0x60
#define _PA_25		(0x19)	//0x64
#define _PA_26		(0x1A)	//0x68
#define _PA_27		(0x1B)	//0x6C
#define _PA_28		(0x1C)	//0x70
#define _PA_29		(0x1D)	//0x74
#define _PA_30		(0x1E)	//0x78
#define _PA_31		(0x1F)	//0x7C

#define _PB_0		(0x20)	//0x80
#define _PB_1		(0x21)	//0x84
#define _PB_2		(0x22)	//0x88
#define _PB_3		(0x23)	//0x8C
#define _PB_4		(0x24)	//0x90
#define _PB_5		(0x25)	//0x94
#define _PB_6		(0x26)	//0x98
#define _PB_7		(0x27)	//0x9C
#define _PB_8		(0x28)	//0xA0
#define _PB_9		(0x29)	//0xA4
#define _PB_10		(0x2A)	//0xA8
#define _PB_11		(0x2B)	//0xAC
#define _PB_12		(0x2C)	//0xB0
#define _PB_13		(0x2D)	//0xB4
#define _PB_14		(0x2E)	//0xB8
#define _PB_15		(0x2F)	//0xBc
#define _PB_16		(0x30)	//0xC0
#define _PB_17		(0x31)	//0xC4
#define _PB_18		(0x32)	//0xC8
#define _PB_19		(0x33)	//0xCC
#define _PB_20		(0x34)	//0xD0
#define _PB_21		(0x35)	//0xD4
#define _PB_22		(0x36)	//0xD8
#define _PB_23		(0x37)	//0xDC
#define _PB_24		(0x38)	//0xE0
#define _PB_25		(0x39)	//0xE4
#define _PB_26		(0x3A)	//0xE8
#define _PB_27		(0x3B)	//0xEC
#define _PB_28		(0x3C)	//0xF0
#define _PB_29		(0x3D)	//0xF4
#define _PB_30		(0x3E)	//0xF8
#define _PB_31		(0x3F)	//0xFC

#define _PC_0		(0x40)	//0x100
#define _PC_1		(0x41)	//0x104
#define _PC_2		(0x42)	//0x108
#define _PC_3		(0x43)	//0x10C
#define _PC_4		(0x44)	//0x110
#define _PC_5		(0x45)	//0x114
#define _PC_6		(0x46)	//0x118
#define _PC_7		(0x47)	//0x118

#define _PNC		(0xFFFF)
/**
  * @}
  */

/** @defgroup PINMUX_Port_And_Pin_definitions
  * @{
  */
#define PORT_NUM(pin)		((pin>>5) & 0x03)
#define PIN_NUM(pin)		(pin & 0x1f)
/**
  * @}
  */

/** @defgroup GPIO_Pull_Resistor_definitions
  * @{
  */
#define GPIO_Resistor_LARGE		0x00 /*!< GPIO Resistor LARGE */
#define GPIO_Resistor_SMALL		0x01 /*!< GPIO Resistor SMALL */
/**
  * @}
  */

/** @defgroup PINMUX_Function_definitions
  * @{
  */
#define PINMUX_FUNCTION_GPIO			(0)
#define PINMUX_FUNCTION_UART			(1)
#define PINMUX_FUNCTION_UART_RTSCTS		(2)
#define PINMUX_FUNCTION_LOGUART			(2)
#define PINMUX_FUNCTION_SPI				(3)
#define PINMUX_FUNCTION_RTC				(4)
#define PINMUX_FUNCTION_IR				(5)
#define PINMUX_FUNCTION_SPIF			(6)
#define PINMUX_FUNCTION_I2C				(7)
#define PINMUX_FUNCTION_SDIOH			(8)
#define PINMUX_FUNCTION_LEDC			(9)
#define PINMUX_FUNCTION_PWM				(10)
#define PINMUX_FUNCTION_SWD				(11)
#define PINMUX_FUNCTION_AUDIO			(12)
#define PINMUX_FUNCTION_I2S0			(13)
#define PINMUX_FUNCTION_I2S1			(13)
#define PINMUX_FUNCTION_I2S2			(14)
#define PINMUX_FUNCTION_I2S3			(15)
#define PINMUX_FUNCTION_SPK				(16)
#define PINMUX_FUNCTION_AUXIN			(16)
#define PINMUX_FUNCTION_DMIC			(17)
#define PINMUX_FUNCTION_CAPTOUCH		(18)
#define PINMUX_FUNCTION_SIC				(19)
#define PINMUX_FUNCTION_MIPI			(20)
#define PINMUX_FUNCTION_USB				(21)
#define PINMUX_FUNCTION_FEM_C			(22)
#define PINMUX_FUNCTION_ANT_SEL			(22)
#define PINMUX_FUNCTION_EXT_ZIGBEE		(23)
#define PINMUX_FUNCTION_BT_UART			(24)
#define PINMUX_FUNCTION_BT_GPIO			(25)
#define PINMUX_FUNCTION_BT_RF			(26)
#define PINMUX_FUNCTION_DBG_BTCOEX_GNT	(27)
#define PINMUX_FUNCTION_TIMINPUT_HS		(28)
#define PINMUX_FUNCTION_DBGPORT			(29)
#define PINMUX_FUNCTION_WAKEUP			(30)

/**
  * @}
  */

/** @defgroup PINMUX_Peripheral_Location_definitions
  * @note just used by function PINMUX_Ctrl
  * @{
  */
#define PINMUX_S0		(0)
#define PINMUX_S1		(1)
#define PINMUX_S2		(2)
#define PINMUX_S3		(3)
#define PINMUX_S4		(4)
#define PINMUX_S5		(5)
/**
  * @}
  */

/** @defgroup PINMUX_PAD_DrvStrength_definitions
  * @{
  */
#define PAD_DRV_ABILITITY_LOW			(0)
#define PAD_DRV_ABILITITY_HIGH			(1)
/**
  * @}
  */

/**
  * @}
  */

/** @defgroup PINMUX_Exported_Functions PINMUX Exported Functions
  * @{
  */
_LONG_CALL_ void PAD_CMD(u8 PinName, u8 NewStatus);
_LONG_CALL_ void PAD_DrvStrength(u8 PinName, u32 DrvStrength);
_LONG_CALL_ void PAD_PullCtrl(u8 PinName, u8 PullType);
_LONG_CALL_ void PAD_SleepPullCtrl(u8 PinName, u8 PullType);
_LONG_CALL_ void _Pinmux_Config(u8 PinName, u32 PinFunc);
_LONG_CALL_ u32 Pinmux_ConfigGet(u8 PinName);
_LONG_CALL_ void Pinmux_UartLogCtrl(u32  PinLocation, BOOL   Operation);
_LONG_CALL_ void Pinmux_SpicCtrl(u32  PinLocation, BOOL Operation);
_LONG_CALL_ void PAD_SpicCtrl(u32  PinLocation);
_LONG_CALL_ void PAD_ResistorCtrl(u8 PinName, u8 RType);
_LONG_CALL_ void Pinmux_Swdoff(void);

/**
  *  @brief Control digital path input for ADC and CTC pad.
  *  @param PinName: Pin for ADC module or CTC module.
  *  @param NewState: Digital path input status.
  *    @arg ENABLE: Enable digital path input.
  *    @arg DISABLE: Disable digital path input.
  *  @retval None
  *  @note only valid for ADC and CTC pads
  */
__STATIC_INLINE
void PAD_InputCtrl(u8 PinName, u32 NewState)
{
	u32 RTemp, PadShift = 0;

	if (PinName > _PA_8) {
		return;
	}

	RTemp = HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_DUMMY_094);

	PadShift = PinName - _PA_0;

	if (NewState != DISABLE) {
		RTemp |= BIT(PadShift);
	} else {
		RTemp &= (~ BIT(PadShift));
	}

	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_DUMMY_094, RTemp);
}

/**
  *  @brief Control digital path input for Audio share PAD.
  *  @param PinName: Pin of Audio share PAD or 0xFF.
  *  @param NewState: Digital path input status.
  *    @arg ENABLE: Enable digital path input.
  *    @arg DISABLE: Disable digital path input.
  *  @retval None
  *  @note Only valid for Audio share PADs.
  *  @note If PinName is 0xFF, all the pins of Audio share PAD will be controlled.
  */
__STATIC_INLINE
void APAD_InputCtrl(u8 PinName, u32 NewState)
{
	u32 RTemp, PadShift = 0;

	if (PinName == 0xFF) {
		if (NewState != DISABLE) {
			RTemp = 0x1FFFFF;
		} else {
			RTemp = 0x0;
		}
	} else {
		if (PinName < _PA_18 || PinName > _PB_6) {
			return;
		}

		RTemp = HAL_READ32(PINMUX_REG_BASE, REG_PAD_AUD_PAD_CTRL);
		PadShift = PinName - _PA_18;
		if (NewState != DISABLE) {
			RTemp |= BIT(PadShift);
		} else {
			RTemp &= ~BIT(PadShift);
		}
	}

	HAL_WRITE32(PINMUX_REG_BASE, REG_PAD_AUD_PAD_CTRL, RTemp);
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

#endif   //_HAL_AMEBA_PINMUX_
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/

