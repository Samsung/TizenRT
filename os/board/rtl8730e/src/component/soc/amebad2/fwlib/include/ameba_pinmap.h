/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _AMEBA_PINMAP_H_
#define _AMEBA_PINMAP_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @verbatim
  *****************************************************************************************
  * PINMAP
  *****************************************************************************************
  *   you can use pinmap to config pinmux instead of Pinmux_Config function
  *   you can use pinmap to config GPIO pull high/low status for power save when enter power save mode
  *****************************************************************************************
  * How To Use
  *****************************************************************************************
  *	-1) ENABLE MACRO: CONFIG_PINMAP_ENABLE
  *	-2) Set all pins function in pmap_func based on your board
  *	-3) Bootloader will call pinmap_init, then all pinmux will be set based on pmap_func
  *	-4) pinmap_sleep will be called when enter sleep mode, all GPIO will pull high or low based on pmap_func
  *	-5) pinmap_wake will be called when wake from sleep mode, all GPIO will back to active mode
  *****************************************************************************************
  * @endverbatim
  */


/** @defgroup PINMAP
  * @brief PINMAP driver modules
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** @defgroup PINMAP_Exported_Types PINMAP Exported Types
  * @{
  */

/**
  * @brief  PINMAP Init structure definition
  */
typedef struct {
	u16 PinName; /*!< Specifies the pin name, This parameter can be a value of @ref PINMUX_Pin_Name_definitions */
	u8 FuncPuPd;/*!< Specifies the pin function PU/PD, This parameter can be a value of @ref GPIO_Pull_parameter_definitions */
	u8 SleepPuPd;/*!< Specifies the pin sleep PU/PD, This parameter can be a value of @ref GPIO_Pull_parameter_definitions */
} PMAP_TypeDef;
/** @} */


/* Exported functions --------------------------------------------------------*/
/** @defgroup PINMAP_Exported_Functions PINMAP Exported Functions
  * @{
  */
void pinmap_init(void);
/** @} */

/** @} */

/** @} */

/* Other definations --------------------------------------------------------*/
#define GPIO_PuPd_KEEP	0xFF /* keep pupd unchanged */

#ifdef __cplusplus
}
#endif

#endif //_AMEBA_PINMAP_H_
