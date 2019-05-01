/****************************************************************************
 *
 * Copyright 2019 NXP Semiconductors All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
/************************************************************************************
 * os/arch/arm/src/imxrt/imxrt_config.h
 *
 *   Copyright (C) 2018 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ************************************************************************************/

#ifndef __ARCH_ARM_SRC_IMXRT_IMXRT_CONFIG_H
#define __ARCH_ARM_SRC_IMXRT_IMXRT_CONFIG_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <tinyara/config.h>
#include <arch/board/board.h>

#include "chip.h"

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

/* Configuration *********************************************************************/

#undef HAVE_LPUART1
#undef HAVE_LPUART2
#undef HAVE_LPUART3
#undef HAVE_LPUART4
#undef HAVE_LPUART5
#undef HAVE_LPUART6
#undef HAVE_LPUART7
#undef HAVE_LPUART8

#ifdef CONFIG_IMXRT_LPUART1
#define HAVE_LPUART1 1
#endif
#ifdef CONFIG_IMXRT_LPUART2
#define HAVE_LPUART2 1
#endif
#ifdef CONFIG_IMXRT_LPUART3
#define HAVE_LPUART3 1
#endif
#ifdef CONFIG_IMXRT_LPUART4
#define HAVE_LPUART4 1
#endif
#ifdef CONFIG_IMXRT_LPUART5
#define HAVE_LPUART5 1
#endif
#ifdef CONFIG_IMXRT_LPUART6
#define HAVE_LPUART6 1
#endif
#ifdef CONFIG_IMXRT_LPUART7
#define HAVE_LPUART7 1
#endif
#ifdef CONFIG_IMXRT_LPUART8
#define HAVE_LPUART8 1
#endif

/* Check if we have a LPUART device */

#undef CONFIG_IMXRT_HAVE_LPUART
#undef HAVE_LPUART_DEVICE

#if defined(HAVE_LPUART1) || defined(HAVE_LPUART2) || defined(HAVE_LPUART3) || \
		defined(HAVE_LPUART4) || defined(HAVE_LPUART5) || defined(HAVE_LPUART6) || \
		defined(HAVE_LPUART7) || defined(HAVE_LPUART8)
#define HAVE_LPUART_DEVICE 1
#endif

/* Is there a serial console? There should be at most one defined.  It could be on
 * any LPUARTn, n=1,2,3,4,5,6,7,8
 */

#undef HAVE_LPUART_CONSOLE

#if defined(CONFIG_LPUART1_SERIAL_CONSOLE) && defined(HAVE_LPUART1)
#undef CONFIG_LPUART2_SERIAL_CONSOLE
#undef CONFIG_LPUART3_SERIAL_CONSOLE
#undef CONFIG_LPUART4_SERIAL_CONSOLE
#undef CONFIG_LPUART5_SERIAL_CONSOLE
#undef CONFIG_LPUART6_SERIAL_CONSOLE
#undef CONFIG_LPUART7_SERIAL_CONSOLE
#undef CONFIG_LPUART8_SERIAL_CONSOLE
#define HAVE_LPUART_CONSOLE 1
#elif defined(CONFIG_LPUART2_SERIAL_CONSOLE) && defined(HAVE_LPUART2)
#undef CONFIG_LPUART1_SERIAL_CONSOLE
#undef CONFIG_LPUART3_SERIAL_CONSOLE
#undef CONFIG_LPUART4_SERIAL_CONSOLE
#undef CONFIG_LPUART5_SERIAL_CONSOLE
#undef CONFIG_LPUART6_SERIAL_CONSOLE
#undef CONFIG_LPUART7_SERIAL_CONSOLE
#undef CONFIG_LPUART8_SERIAL_CONSOLE
#define HAVE_LPUART_CONSOLE 1
#elif defined(CONFIG_LPUART3_SERIAL_CONSOLE) && defined(HAVE_LPUART3)
#undef CONFIG_LPUART1_SERIAL_CONSOLE
#undef CONFIG_LPUART2_SERIAL_CONSOLE
#undef CONFIG_LPUART4_SERIAL_CONSOLE
#undef CONFIG_LPUART5_SERIAL_CONSOLE
#undef CONFIG_LPUART6_SERIAL_CONSOLE
#undef CONFIG_LPUART7_SERIAL_CONSOLE
#undef CONFIG_LPUART8_SERIAL_CONSOLE
#define HAVE_LPUART_CONSOLE 1
#elif defined(CONFIG_LPUART4_SERIAL_CONSOLE) && defined(HAVE_LPUART4)
#undef CONFIG_LPUART1_SERIAL_CONSOLE
#undef CONFIG_LPUART2_SERIAL_CONSOLE
#undef CONFIG_LPUART3_SERIAL_CONSOLE
#undef CONFIG_LPUART5_SERIAL_CONSOLE
#undef CONFIG_LPUART6_SERIAL_CONSOLE
#undef CONFIG_LPUART7_SERIAL_CONSOLE
#undef CONFIG_LPUART8_SERIAL_CONSOLE
#define HAVE_LPUART_CONSOLE 1
#elif defined(CONFIG_LPUART5_SERIAL_CONSOLE) && defined(HAVE_LPUART5)
#undef CONFIG_LPUART1_SERIAL_CONSOLE
#undef CONFIG_LPUART2_SERIAL_CONSOLE
#undef CONFIG_LPUART3_SERIAL_CONSOLE
#undef CONFIG_LPUART4_SERIAL_CONSOLE
#undef CONFIG_LPUART6_SERIAL_CONSOLE
#undef CONFIG_LPUART7_SERIAL_CONSOLE
#undef CONFIG_LPUART8_SERIAL_CONSOLE
#define HAVE_LPUART_CONSOLE 1
#elif defined(CONFIG_LPUART6_SERIAL_CONSOLE) && defined(HAVE_LPUART6)
#undef CONFIG_LPUART1_SERIAL_CONSOLE
#undef CONFIG_LPUART2_SERIAL_CONSOLE
#undef CONFIG_LPUART3_SERIAL_CONSOLE
#undef CONFIG_LPUART4_SERIAL_CONSOLE
#undef CONFIG_LPUART5_SERIAL_CONSOLE
#undef CONFIG_LPUART7_SERIAL_CONSOLE
#undef CONFIG_LPUART8_SERIAL_CONSOLE
#define HAVE_LPUART_CONSOLE 1
#elif defined(CONFIG_LPUART7_SERIAL_CONSOLE) && defined(HAVE_LPUART7)
#undef CONFIG_LPUART1_SERIAL_CONSOLE
#undef CONFIG_LPUART2_SERIAL_CONSOLE
#undef CONFIG_LPUART3_SERIAL_CONSOLE
#undef CONFIG_LPUART4_SERIAL_CONSOLE
#undef CONFIG_LPUART5_SERIAL_CONSOLE
#undef CONFIG_LPUART6_SERIAL_CONSOLE
#undef CONFIG_LPUART8_SERIAL_CONSOLE
#define HAVE_LPUART_CONSOLE 1
#elif defined(CONFIG_LPUART8_SERIAL_CONSOLE) && defined(HAVE_LPUART8)
#undef CONFIG_LPUART1_SERIAL_CONSOLE
#undef CONFIG_LPUART2_SERIAL_CONSOLE
#undef CONFIG_LPUART3_SERIAL_CONSOLE
#undef CONFIG_LPUART4_SERIAL_CONSOLE
#undef CONFIG_LPUART5_SERIAL_CONSOLE
#undef CONFIG_LPUART6_SERIAL_CONSOLE
#undef CONFIG_LPUART7_SERIAL_CONSOLE
#define HAVE_LPUART_CONSOLE 1
#else
#ifdef CONFIG_DEV_CONSOLE
#warning "No valid CONFIG_[LP]LPUART[n]_SERIAL_CONSOLE Setting"
#endif
#undef CONFIG_LPUART1_SERIAL_CONSOLE
#undef CONFIG_LPUART2_SERIAL_CONSOLE
#undef CONFIG_LPUART3_SERIAL_CONSOLE
#undef CONFIG_LPUART4_SERIAL_CONSOLE
#undef CONFIG_LPUART5_SERIAL_CONSOLE
#undef CONFIG_LPUART6_SERIAL_CONSOLE
#undef CONFIG_LPUART7_SERIAL_CONSOLE
#undef CONFIG_LPUART8_SERIAL_CONSOLE
#endif

/* Check LPUART flow control (Not yet supported) */

#undef CONFIG_LPUART1_FLOWCONTROL
#undef CONFIG_LPUART2_FLOWCONTROL
#undef CONFIG_LPUART3_FLOWCONTROL
#undef CONFIG_LPUART4_FLOWCONTROL
#undef CONFIG_LPUART5_FLOWCONTROL
#undef CONFIG_LPUART6_FLOWCONTROL
#undef CONFIG_LPUART7_FLOWCONTROL
#undef CONFIG_LPUART8_FLOWCONTROL

/* Ethernet controller configuration */

#ifndef CONFIG_IMXRT_ENET_NRXBUFFERS
#define CONFIG_IMXRT_ENET_NRXBUFFERS 6
#endif

#ifndef CONFIG_IMXRT_ENET_NTXBUFFERS
#define CONFIG_IMXRT_ENET_NTXBUFFERS 2
#endif

#ifndef CONFIG_IMXRT_ENET_NETHIFS
#define CONFIG_IMXRT_ENET_NETHIFS 1
#endif

#define IMXRT_ENET_HAS_DBSWAP 1

/* EMAC Default Interrupt Priorities */

#ifndef CONFIG_IMXRT_ENET_PRIO
#define CONFIG_IMXRT_ENET_PRIO  NVIC_SYSH_PRIORITY_DEFAULT
#endif

#ifndef CONFIG_ETH0_PHY_KSZ8081
#define CONFIG_ETH0_PHY_KSZ8081
#endif

#ifndef CONFIG_NET_ETH_PKTSIZE
#define CONFIG_NET_ETH_PKTSIZE 590
#endif

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL))
#define FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL 0
#endif

/*! @brief Construct a status code value from a group and code number. */
#define MAKE_STATUS(group, code) ((((group) * 100) + (code)))

/*! @brief Construct the version number for drivers. */
#define MAKE_VERSION(major, minor, bugfix) (((major) << 16) | ((minor) << 8) | (bugfix))
#define FSL_SAI_DRIVER_VERSION (MAKE_VERSION(2, 1, 3)) /*!< Version 2.1.3 */
/*@}*/

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#ifndef   __STATIC_FORCEINLINE                 
	#define __STATIC_FORCEINLINE                   __attribute__((always_inline)) static inline
#endif 

#define AT_NONCACHEABLE_SECTION(var) var
#define AT_NONCACHEABLE_SECTION_ALIGN(var, alignbytes) var __attribute__((aligned(alignbytes)))
#define AT_NONCACHEABLE_SECTION_INIT(var) var
#define AT_NONCACHEABLE_SECTION_ALIGN_INIT(var, alignbytes) var __attribute__((aligned(alignbytes)))

/*! @brief Generic status return codes. */
enum _generic_status {
	kStatus_Success = MAKE_STATUS(0, 0),
	kStatus_Fail = MAKE_STATUS(0, 1),
	kStatus_ReadOnly = MAKE_STATUS(0, 2),
	kStatus_OutOfRange = MAKE_STATUS(0, 3),
	kStatus_InvalidArgument = MAKE_STATUS(0, 4),
	kStatus_Timeout = MAKE_STATUS(0, 5),
	kStatus_NoTransferInProgress = MAKE_STATUS(0, 6),
};

/*! @brief Type used for all status and error return values. */
typedef int32_t status_t;

/* ###########################  Core Function Access  ########################### */
/** \ingroup  CMSIS_Core_FunctionInterface
    \defgroup CMSIS_Core_RegAccFunctions CMSIS Core Register Access Functions
  @{
 */

/**
  \brief   Get Priority Mask
  \details Returns the current state of the priority mask bit from the Priority Mask Register.
  \return               Priority Mask value
 */
__STATIC_FORCEINLINE uint32_t __get_PRIMASK(void)
{
	uint32_t result;

	__asm__ __volatile__ ("MRS %0, primask" : "=r" (result) :: "memory");
	return(result);
}

/**
  \brief   Set Priority Mask
  \details Assigns the given value to the Priority Mask Register.
  \param [in]    priMask  Priority Mask
 */
__STATIC_FORCEINLINE void __set_PRIMASK(uint32_t priMask)
{
	__asm__ __volatile__ ("MSR primask, %0" : : "r" (priMask) : "memory");
}

/**
  \brief   Enable IRQ Interrupts
  \details Enables IRQ interrupts by clearing the I-bit in the CPSR.
           Can only be executed in Privileged modes.
 */
__STATIC_FORCEINLINE void __enable_irq(void)
{
	__asm__ __volatile__ ("cpsie i" : : : "memory");
}


/**
  \brief   Disable IRQ Interrupts
  \details Disables IRQ interrupts by setting the I-bit in the CPSR.
           Can only be executed in Privileged modes.
 */
__STATIC_FORCEINLINE void __disable_irq(void)
{
	__asm__ __volatile__ ("cpsid i" : : : "memory");
}

/*!
* @brief Disable the global IRQ
*
* Disable the global interrupt and return the current primask register. User is required to provided the primask
* register for the EnableGlobalIRQ().
*
* @return Current primask value.
*/
static inline uint32_t imxrt_disableglobalirq(void)
{
	uint32_t regPrimask = __get_PRIMASK();

	__disable_irq();

	return regPrimask;
}

/*!
* @brief Enable the global IRQ
*
* Set the primask register with the provided primask value but not just enable the primask. The idea is for the
* convenience of integration of RTOS. some RTOS get its own management mechanism of primask. User is required to
* use the EnableGlobalIRQ() and DisableGlobalIRQ() in pair.
*
* @param primask value of primask register to be restored. The primask value is supposed to be provided by the
* DisableGlobalIRQ().
*/
static inline void imxrt_enableglobalirq(uint32_t primask)
{
	__set_PRIMASK(primask);
}

/************************************************************************************
 * Public Functions
 ************************************************************************************/

#endif							/* __ARCH_ARM_SRC_IMXRT_IMXRT_CONFIG_H */
