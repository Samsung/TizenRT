/*
 ******************************************************************************
 *
 * Copyright(c) 2014 - 2022 Realtek Corporation. All rights reserved.
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
#ifndef MBED_EXT_POWER_MODE_API_EXT_H
#define MBED_EXT_POWER_MODE_API_EXT_H
#if (defined(CONFIG_PLATFORM_8195BHP) && (CONFIG_PLATFORM_8195BHP == 1))  ||  (defined(CONFIG_PLATFORM_8195BLP) && (CONFIG_PLATFORM_8195BLP == 1))  ||  (defined(CONFIG_PLATFORM_8710C) && (CONFIG_PLATFORM_8710C == 1))  ||  (defined(CONFIG_PLATFORM_8735B) && (CONFIG_PLATFORM_8735B == 1))
///@name AmebaPro Only
///@{
#include "device.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined (CONFIG_PLATFORM_8195B)
/** @addtogroup power mode POWER MODE
 *  @ingroup    hal
 *  @brief      power mode functions
 *  @{
 */

#if defined (CONFIG_PLATFORM_8195BLP)
//SLP
#define     SLP_STIMER    BIT0
#define     SLP_GTIMER    BIT1
#define     SLP_GPIO      BIT2
#define     SLP_PWM       BIT3
#define     SLP_UART      BIT4
#define     SLP_HSTIMER   BIT5
#define     SLP_WLAN      BIT6
#define     SLP_I2C       BIT7
#define     SLP_ADC       BIT8
#define     SLP_COMP      BIT9
#define     SLP_SGPIO     BIT10

//DSTBY
#define     DSTBY_STIMER  BIT0
#define     DSTBY_GTIMER  BIT1
#define     DSTBY_GPIO    BIT2
#define     DSTBY_PWM     BIT3
#define     DSTBY_UART    BIT4
#define     DSTBY_HSTIMER BIT5
#define     DSTBY_WLAN    BIT6
#define     DSTBY_I2C     BIT7
#define     DSTBY_ADC     BIT8
#define     DSTBY_COMP    BIT9
#define     DSTBY_SGPIO   BIT10

//DS wake event
#define DS_STIMER   BIT0
#define DS_GPIO     BIT1
#define DS_ADP      BIT2
#define DS_RTC      BIT3
#elif defined (CONFIG_PLATFORM_8195BHP)
//SLP
#define     SLP_GTIMER    BIT1
#define     SLP_GPIO      BIT2
#define     SLP_PWM       BIT3
#define     SLP_UART      BIT4
#define     SLP_MII   	  BIT5
#define     SLP_WLAN      BIT6
#define     SLP_I2C       BIT7
#define     SLP_ADC       BIT8
#define     SLP_USB       BIT9
#define     SLP_SGPIO     BIT10
#define     SLP_SDIO      BIT11

//DSTBY
#define     DSTBY_GTIMER  BIT1
#define     DSTBY_GPIO    BIT2
#endif

///@name Ameba Common
///@{

enum {
	AON_128K = 0,    // AON CLK 128kHz
	AON_100K = 1,    // AON CLK 100kHz
};

/**
 *  @brief The function for ls sleep mode.
 *
 *  @param[in]  Option, To slect AON Timer,GPIO...etc
 *                - bit[4]: the UART Wake up event.
 *                - bit[3]: the PWM Wake up event.
 *                - bit[2]: the GPIO A13 as a Wake up event.
 *                - bit[1]: the GTimer Wake up event. Only active at 128kHz
 *                - bit[0]: the AON Timer Wake up event.
 *  @param[in]  SDuration, wake up after SDuration value. Uint: us
 *  @param[in]  Clock, 1: 100kHz, 0: 128kHz.
 *  @param[in]  GpioOption, GPIOA0~A13 as a wake up trigger.
 *
 *  @returns void
 */
void SleepCG(u16 Option, u32 SDuration, u8 Clock, u8 GpioOption);

#if defined (CONFIG_PLATFORM_8195BHP)

/**
 *  @brief The function for hs power gated.
 *
 *  @param[in]  Option, To slect HS Timer and GPIO
 *                - bit[2]: the GPIO Wake up event.
 *                - bit[1]: the GTimer Wake up event.
 *  @param[in]  SDuration, wake up after SDuration value. Uint: us
 *  @param[in]  Memsel, 1: Enable memory, 0: Disable memory.
 *  @param[in]  GpioOption, Select GPIO pin as a wake up trigger.
 *
 *  @returns void
 */
void SleepPG(u16 Option, u32 SDuration, u8 Memsel, u8 GpioOption);

#endif
#if defined (CONFIG_PLATFORM_8195BLP)

/**
 *  @brief The function for ls standby mode.
 *
 *  @param[in]  Option, To slect AON Timer,GPIO...etc
 *                - bit[4]: the UART Wake up event.
 *                - bit[3]: the PWM Wake up event.
 *                - bit[2]: the GPIO Wake up event.
 *                - bit[1]: the GTimer Wake up event. Not ready
 *                - bit[0]: the AON Timer Wake up event.
 *  @param[in]  SDuration, wake up after SDuration value(Max:8300 sec). Uint: us
 *  @param[in]  Memsel, 1: Enable memory, 0: Disable memory.
 *  @param[in]  GpioOption, GPIOA0~A13 as a wake up trigger.
 *
 *  @returns void
 */
void Standby(u16 Option, u64 SDuration, u8 Memsel, u8 GpioOption);

/**
  * @brief The stubs functions table to exports POWER MODE HAL functions in ROM.
  */

//extern const hal_power_mode_func_stubs_t hal_power_mode_stubs;

/**
 *  @brief The function for ls deep sleep mode.
 *
 *  @param[in]  Option, To slect AON Timer,GPIO,ADP and RTC.
 *                - bit[3]: the ADP Wake up event.
 *                - bit[2]: the GPIO A13 Rising Wake up event.
 *                - bit[1]: the GPIO A13 Falling Wake up event.
 *                - bit[0]: the AON Timer Wake up event.
 *  @param[in]  SDuration, wake up after SDuration value(Max:8300 sec). Uint: us
 *  @param[in]  Memsel, 1: Enable memory, 0: Disable memory.
 *  @param[in]  Clock, 1: 100kHz, 0: 128kHz.
 *
 *  @returns void
 */
void DeepSleep(u8  Option, u64 SDuration, u8 Memsel, u8 Clock);

/**
 *  @brief The function for ls deep sleep mode.
 *
 *  @param[in] Option The RTC wake up event.
 *                - bit[3]: Wake up per day.
 *                - bit[2]: Wake up per hour.
 *                - bit[1]: Wake up per minute.
 *                - bit[0]: Wake up per second.
 *  @param[in]  Memsel, 1: Enable memory, 0: Disable memory.
 *
 *  @returns void
 */
void DeepSleep_RTC(u8 Option, u8 Memsel);

/**
 *  @brief   Set BOD level
 *
 *  @param   bod_lvl: BOD level, max value is 7,
 *                               min value is 0.
 *  @return  void
 *
 */
void BOD_Level_Setting(int bod_lvl);

///@}

/*\@}*/

#endif
#endif
///@}

#if defined(CONFIG_PLATFORM_8710C)

//DS wake event
#define DS_STIMER       BIT0
#define DS_GPIO         BIT1
//SLP
#define SLP_STIMER      BIT0
#define SLP_GTIMER      BIT1
#define SLP_GPIO        BIT2
#define SLP_PWM         BIT3
#define SLP_UART        BIT4
#define SLP_WLAN        BIT6
#define SLP_SDIO        BIT11

//DSTBY
#define DSTBY_STIMER    BIT0
#define DSTBY_GTIMER    BIT1
#define DSTBY_GPIO      BIT2

enum {
	AON_250K = 0,   // AON CLK 250kHz
	AON_4M = 1,     // AON CLK 4MHz
};

/**
 *  @brief The function for ls deep sleep mode.
 *
 *  @param[in]  Option, To slect AON Timer and GPIO.
 *                - bit[1]: the GPIO as a Wake up event.
 *                - bit[0]: the LP Timer Wake up event.
 *  @param[in]  SDuration, wake up after SDuration value. Uint: us
 *  @param[in]  Clock, 1: 4MHz, 0: 250kHz.
 *
 *  @returns void
 */
void DeepSleep(u8  Option, u32 SDuration, u8 Clock);

/**
 *  @brief The function for sleep mode.
 *
 *  @param[in]  Option, To slect  GTimer, GPIO and PWM...etc
 *                - bit[4]: the UART Wake up event.
 *                - bit[3]: the PWM Wake up event.
 *                - bit[2]: the GPIO Wake up event.
 *                - bit[1]: the GTimer Wake up event.
 *                - bit[0]: the LP Timer Wake up event.
 *  @param[in]  SDuration, wake up after SDuration value. Uint: us
 *  @param[in]  Clock, 1: 4MHz, 0: 250kHz.
 *  @param[in]  GpioOption, Select GPIO pin as a wake up trigger.
 *
 *  @returns void
 */
void SleepCG(u16 Option, u32 SDuration, u8 Clock, u8 GpioOption);

/**
 *  @brief The function for Standby mode.
 *
 *  @param[in]  Option, To slect GTimer, GPIO and PWM...etc
 *                - bit[4]: the UART Wake up event.
 *                - bit[3]: the PWM Wake up event.
 *                - bit[2]: the GPIO Wake up event.
 *                - bit[1]: the GTimer Wake up event.
 *                - bit[0]: the LP Timer Wake up event.
 *  @param[in]  SDuration, wake up after SDuration value. Uint: us
 *  @param[in]  Clock, 1: 4MHz, 0: 250kHz.
 *  @param[in]  GpioOption, Select GPIO pin as a wake up trigger.
 *
 *  @returns void
 */
void Standby(u16 Option, u32 SDuration, u8 Clock, u8 GpioOption);

#endif

#if defined(CONFIG_PLATFORM_8735B)
#define     SLP_AON_TIMER   BIT0
#define     SLP_AON_GPIO    BIT1
#define     SLP_RTC         BIT2
#define     SLP_COMP        BIT3
#define     SLP_PON_GPIO    BIT4
#define     SLP_PWM         BIT5
#define     SLP_UART        BIT6
#define     SLP_GTIMER      BIT7
#define     SLP_WLAN        BIT8

//DSTBY
#define     DSTBY_AON_TIMER BIT0
#define     DSTBY_AON_GPIO  BIT1
#define     DSTBY_RTC       BIT2
#define     DSTBY_COMP      BIT3
#define     DSTBY_PON_GPIO  BIT4
#define     DSTBY_PWM       BIT5
#define     DSTBY_UART      BIT6
#define     DSTBY_GTIMER    BIT7
#define     DSTBY_WLAN      BIT8

//DS wake event
#define     DS_AON_TIMER    BIT0
#define     DS_AON_GPIO     BIT1
#define     DS_RTC          BIT2
#define     DS_COMP         BIT3

enum {
	AON_100K = 0,   // AON CLK 250kHz
	AON_4M = 1,     // AON CLK 4MHz
};

/**
 *  @brief The function for sleep mode.
 *
 *  @param[in]  Option, To slect GTimer, GPIO and PWM...etc
 *                - bit[8]: the WLAN Wake up event.
 *                - bit[7]: the GTIMER0 Wake up event.
 *                - bit[6]: the UART Wake up event.
 *                - bit[5]: the PWM Wake up event.
 *                - bit[4]: the PON GPIO Wake up event.
 *                - bit[3]: the COMPWake up event.
 *                - bit[2]: the RTC Wake up event.
 *                - bit[1]: the AON GPIO Wake up event.
 *                - bit[0]: the AON Timer Wake up event.
 *  @param[in]  SDuration, wake up after SDuration value. Uint: us
 *  @param[in]  Clock, 1: 4MHz, 0: 100kHz.
 *  @param[in]  GpioOption, Select GPIO pin as a wake up trigger.
 *
 *  @returns void
 */
void SleepCG(u16 Option, u32 SDuration, u8 Clock, u8 SramOption);


/**
 *  @brief The function for Standby mode.
 *
 *  @param[in]  Option, To slect GTimer, GPIO and PWM...etc
 *                - bit[8]: the WLAN Wake up event.
 *                - bit[7]: the GTIMER0 Wake up event.
 *                - bit[6]: the UART Wake up event.
 *                - bit[5]: the PWM Wake up event.
 *                - bit[4]: the PON GPIO Wake up event.
 *                - bit[3]: the COMPWake up event.
 *                - bit[2]: the RTC Wake up event.
 *                - bit[1]: the AON GPIO Wake up event.
 *                - bit[0]: the AON Timer Wake up event.
 *  @param[in]  SDuration, wake up after SDuration value. Uint: us
 *  @param[in]  Clock, 1: 4MHz, 0: 100kHz.
 *  @param[in]  SramOption, Select SRAM1~SRAM2 state.
 *                - 0: shutdown mode.
 *                - 1: retention mode.
 *                - 2: Normal mode.
 *  @returns void
 */
void Standby(u16 Option, u32 SDuration, u8 Clock, u8 SramOption);

/**
 *  @brief The function for ls deep sleep mode.
 *
 *  @param[in]  Option, To slect AON Timer and GPIO...etc
 *                - bit[3]: the COMP as a Wake up event.
 *                - bit[2]: the RTC Wake up event.
 *                - bit[1]: the AON GPIO as a Wake up event.
 *                - bit[0]: the AON Timer Wake up event.
 *  @param[in]  SDuration, wake up after SDuration value. Uint: us
 *  @param[in]  Clock, 1: 4MHz, 0: 100kHz.
 *
 *  @returns void
 */
void DeepSleep(u8 Option, u32 SDuration, u8 Clock);

/**
 *  @brief The function for get wake up reason.
 *  @returns wake up status
 *   wake up status:
 *                - bit[12]: the AON GPIO3 Wake up status.
 *                - bit[11]: the AON GPIO2 Wake up status.
 *                - bit[10]: the AON GPIO1 Wake up status.
 *                - bit[9]: the AON GPIO0 Wake up status.
 *                - bit[8]: the RTC Wake up status.
 *                - bit[7]: the COMP Wake up status.
 *                - bit[6]: the AON TIMER Wake up status.
 *                - bit[5]: the UART Wake up status.
 *                - bit[4]: NA.
 *                - bit[3]: the WLAN Wake up status.
 *                - bit[2]: NA.
 *                - bit[1]: the PON GPIO Wake up status.
 *                - bit[0]: the GTimer0 Wake up status.
 */
u32 Get_wake_reason(void);

#endif

#endif  // end of "#if (defined(CONFIG_PLATFORM_8195BHP) && (CONFIG_PLATFORM_8195BHP == 1))  ||  (defined(CONFIG_PLATFORM_8195BLP) && (CONFIG_PLATFORM_8195BLP == 1))  ||  (defined(CONFIG_PLATFORM_8710C) && (CONFIG_PLATFORM_8710C == 1))  ||  (defined(CONFIG_PLATFORM_8735B) && (CONFIG_PLATFORM_8735B == 1))"

#ifdef __cplusplus
}
#endif

#endif
