/** mbed Microcontroller Library
  ******************************************************************************
  * @file    sys_api.h
  * @author
  * @version V1.0.0
  * @brief   This file provides following mbed system API:
  *				-JTAG OFF
  *				-System Reset
  *				-LOGUART ON/OFF
  *				-OTA image switch
  ******************************************************************************
  * @attention
  *
  * Copyright(c) 2006 - 2022 Realtek Corporation. All rights reserved.
  *
  * SPDX-License-Identifier: Apache-2.0
  *
  * Licensed under the Apache License, Version 2.0 (the License); you may
  * not use this file except in compliance with the License.
  * You may obtain a copy of the License at
  *
  * http://www.apache.org/licenses/LICENSE-2.0
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an AS IS BASIS, WITHOUT
  * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
#ifndef MBED_SYS_API_H
#define MBED_SYS_API_H

#include "device.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup sys SYSTEM
 *  @ingroup    hal
 *  @brief      system functions
 *  @{
 */

///@name Ameba Common
///@{

/**
  * @brief  Turn off the JTAG function
  * @retval none
  */
void sys_jtag_off(void);

/**
  * @brief  system software reset
  * @retval none
  */
void sys_reset(void);

/**
  * @brief  Get currently selected boot device.
  * @retval boot device
  * @note
  *  BootFromNORFlash            = 0,
  *  BootFromNANDFlash           = 1,
  *  BootFromUART                = 2
  */
uint8_t sys_get_boot_sel(void);

///@}

#if ((defined(CONFIG_PLATFORM_AMEBAD2) && (CONFIG_PLATFORM_AMEBAD2 == 1)) || \
	(defined(CONFIG_PLATFORM_AMEBALITE) && (CONFIG_PLATFORM_AMEBALITE == 1)) || \
	(defined(CONFIG_PLATFORM_AMEBADPLUS) && (CONFIG_PLATFORM_AMEBADPLUS == 1)))
/**
  * @brief  store or load adc calibration parameter
  * @param  write:  this parameter can be one of the following values:
  *		@arg 0: load adc calibration parameter a & b & c from flash system data region
  *		@arg 1: store adc calibration parameter a & b & c to flash system data region
  * @param  a: pointer to adc parameter a
  * @param  b: pointer to adc parameter b
  * @param  c: pointer to adc parameter c
  * @retval none
  */
void sys_adc_calibration(u8 write, u16 *a, u16 *b, u16 *c);
#else
/**
  * @brief  store or load adc calibration parameter
  * @param  write:  this parameter can be one of the following values:
  *		@arg 0: load adc calibration parameter offset & gain from flash system data region
  *		@arg 1: store adc calibration parameter offset & gain to flash system data region
  * @param  offset: pointer to adc parameter offset
  * @param  gain: pointer to adc parameter gain
  * @retval none
  */
void sys_adc_calibration(u8 write, u16 *offset, u16 *gain);
#endif

#if defined(CONFIG_PLATFORM_8195A) && (CONFIG_PLATFORM_8195A == 1)
///@name Ameba1 Only
///@{
/**
  * @brief check whether is sdram power on
  * @retval 1: power on
  *         0: power off
  */
u8   sys_is_sdram_power_on(void);

/**
  * @brief sdram power off
  * @retval none
  */
void sys_sdram_off(void);
///@}
#endif //CONFIG_PLATFORM_8195A

#if (defined(CONFIG_PLATFORM_8711B) && (CONFIG_PLATFORM_8711B == 1)) || defined(CONFIG_PLATFORM_8710C) && (CONFIG_PLATFORM_8710C == 1)
///@name AmebaZ/AmebaZ2
///@{
/**
  * @brief vector reset
  * @retval none
  */
void sys_cpu_reset(void);
///@}
#endif //CONFIG_PLATFORM_8711B | CONFIG_PLATFORM_8710C

/**
  * @brief  open log uart
  * @retval none
  */
void sys_log_uart_on(void);

/**
  * @brief  close log uart
  * @retval none
  */
void sys_log_uart_off(void);

/**
  * @brief  switch OTA image if the othe OTA image is valid
  * @retval none
  * @note for AmebaZ, sys_clear_ota_signature is the same with sys_recover_ota_signature
  */
void sys_clear_ota_signature(void);

/**
  * @brief  switch OTA image if the othe OTA image is valid
  * @retval none
  * @note for AmebaZ, sys_clear_ota_signature is the same with sys_recover_ota_signature
  */
void sys_recover_ota_signature(void);

#if (defined(CONFIG_PLATFORM_8735B) && (CONFIG_PLATFORM_8735B == 1))
///@name AmebaPro2
///@{
/**
  * @brief vector reset
  * @retval none
  */
void sys_backtrace_enable(void);
///@}
#endif //CONFIG_PLATFORM_8735B

/*\@}*/

#ifdef __cplusplus
}
#endif

#endif
