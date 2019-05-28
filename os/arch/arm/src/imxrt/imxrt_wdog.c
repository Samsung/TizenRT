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
/****************************************************************************
 * os/arch/arm/src/imxrt/imxrt_wdog.c
 *
 *   Copyright (C) 2018 Gregory Nutt. All rights reserved.
 *   Author:  Janne Rosberg <janne@offcode.fi>
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
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdint.h>
#include <fixedmath.h>
#include <assert.h>

#include "up_arch.h"

#include "chip/imxrt_wdog.h"
#include "imxrt_config.h"
#include "imxrt_clock.h"
#include "imxrt_wdog.h"
#include <arch/board/board.h>	/* Include last:  has dependencies */

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Configuration ************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/
/*******************************************************************************
 * Variables
 ******************************************************************************/
static WDOG_Type *const s_wdogBases[] = WDOG_BASE_PTRS;
#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
/* Array of WDOG clock name. */
static const clock_ip_name_t s_wdogClock[] = WDOG_CLOCKS;
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

static const IRQn_Type s_wdogIRQ[] = WDOG_IRQS;

/*******************************************************************************
 * Code
 ******************************************************************************/
static uint32_t imxrt_wdog_getinstance(WDOG_Type *base)
{
	uint32_t instance;

	/* Find the instance index from base address mappings. */
	for (instance = 0; instance < ARRAY_SIZE(s_wdogBases); instance++) {
		if (s_wdogBases[instance] == base) {
			break;
		}
	}

	assert(instance < ARRAY_SIZE(s_wdogBases));

	return instance;
}

/*!
 * brief Initializes the WDOG configuration sturcture.
 *
 * This function initializes the WDOG configuration structure to default values. The default
 * values are as follows.
 * code
 *   wdogConfig->enableWdog = true;
 *   wdogConfig->workMode.enableWait = true;
 *   wdogConfig->workMode.enableStop = false;
 *   wdogConfig->workMode.enableDebug = false;
 *   wdogConfig->enableInterrupt = false;
 *   wdogConfig->enablePowerdown = false;
 *   wdogConfig->resetExtension = flase;
 *   wdogConfig->timeoutValue = 0xFFU;
 *   wdogConfig->interruptTimeValue = 0x04u;
 * endcode
 *
 * param config Pointer to the WDOG configuration structure.
 * see wdog_config_t
 */
void imxrt_wdog_getdefaultconfig(wdog_config_t *config)
{
	assert(config);

	/* Initializes the configure structure to zero. */
	memset(config, 0, sizeof(*config));

	config->enableWdog = true;
	config->workMode.enableWait = false;
	config->workMode.enableStop = false;
	config->workMode.enableDebug = false;
	config->enableInterrupt = false;
	config->softwareResetExtension = false;
	config->enablePowerDown = false;
	config->timeoutValue = 0xffu;
	config->interruptTimeValue = 0x04u;
	config->enableTimeOutAssert = false;
}

/*!
 * brief Initializes the WDOG.
 *
 * This function initializes the WDOG. When called, the WDOG runs according to the configuration.
 *
 * This is an example.
 * code
 *   wdog_config_t config;
 *   WDOG_GetDefaultConfig(&config);
 *   config.timeoutValue = 0xffU;
 *   config->interruptTimeValue = 0x04u;
 *   WDOG_Init(wdog_base,&config);
 * endcode
 *
 * param base   WDOG peripheral base address
 * param config The configuration of WDOG
 */
void imxrt_wdog_init(WDOG_Type *base, const wdog_config_t *config)
{
	assert(config);

	uint16_t value = 0u;

	value = WDOG_WCR_WDE(config->enableWdog) | WDOG_WCR_WDW(config->workMode.enableWait) |
			WDOG_WCR_WDZST(config->workMode.enableStop) | WDOG_WCR_WDBG(config->workMode.enableDebug) |
			WDOG_WCR_SRE(config->softwareResetExtension) | WDOG_WCR_WT(config->timeoutValue) |
			WDOG_WCR_WDT(config->enableTimeOutAssert) | WDOG_WCR_SRS_MASK | WDOG_WCR_WDA_MASK;

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
	/* Set configruation */
	imxrt_clock_enableclock(s_wdogClock[imxrt_wdog_getinstance(base)]);
#endif

	base->WICR = WDOG_WICR_WICT(config->interruptTimeValue) | WDOG_WICR_WIE(config->enableInterrupt);
	base->WMCR = WDOG_WMCR_PDE(config->enablePowerDown);
	base->WCR = value;
	if (config->enableInterrupt) {
		up_enable_irq(s_wdogIRQ[imxrt_wdog_getinstance(base)]);
	}
}

/*!
 * brief Shuts down the WDOG.
 *
 * This function shuts down the WDOG.
 * Watchdog Enable bit is a write one once only bit. It is not
 * possible to clear this bit by a software write, once the bit is set.
 * This bit(WDE) can be set/reset only in debug mode(exception).
 */
void imxrt_wdog_deinit(WDOG_Type *base)
{
	if (base->WCR & WDOG_WCR_WDBG_MASK) {
		imxrt_wdog_disable(base);
	}
}

/*!
 * brief Gets the WDOG all reset status flags.
 *
 * This function gets all reset status flags.
 *
 * code
 * uint16_t status;
 * status = WDOG_GetStatusFlags (wdog_base);
 * endcode
 * param base        WDOG peripheral base address
 * return            State of the status flag: asserted (true) or not-asserted (false).see _wdog_status_flags
 *                    - true: a related status flag has been set.
 *                    - false: a related status flag is not set.
 */
uint16_t imxrt_wdog_getstatusflags(WDOG_Type *base)
{
	uint16_t status_flag = 0U;

	status_flag |= (base->WCR & WDOG_WCR_WDE_MASK);
	status_flag |= (base->WRSR & WDOG_WRSR_POR_MASK);
	status_flag |= (base->WRSR & WDOG_WRSR_TOUT_MASK);
	status_flag |= (base->WRSR & WDOG_WRSR_SFTW_MASK);
	status_flag |= (base->WICR & WDOG_WICR_WTIS_MASK);

	return status_flag;
}

/*!
 * brief Clears the WDOG flag.
 *
 * This function clears the WDOG status flag.
 *
 * This is an example for clearing the interrupt flag.
 * code
 *   WDOG_ClearStatusFlags(wdog_base,KWDOG_InterruptFlag);
 * endcode
 * param base        WDOG peripheral base address
 * param mask        The status flags to clear.
 *                    The parameter could be any combination of the following values.
 *                    kWDOG_TimeoutFlag
 */
void imxrt_wdog_clearinterruptstatus(WDOG_Type *base, uint16_t mask)
{
	if (mask & kWDOG_InterruptFlag) {
		base->WICR |= WDOG_WICR_WTIS_MASK;
	}
}

/*!
 * brief Refreshes the WDOG timer.
 *
 * This function feeds the WDOG.
 * This function should be called before the WDOG timer is in timeout. Otherwise, a reset is asserted.
 *
 * param base WDOG peripheral base address
 */
void imxrt_wdog_refresh(WDOG_Type *base)
{
	base->WSR = WDOG_REFRESH_KEY & 0xFFFFU;
	base->WSR = (WDOG_REFRESH_KEY >> 16U) & 0xFFFFU;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: imxrt_wdog_disable
 *
 * Description:
 *   Disables all watchdogs
 *
 ****************************************************************************/

void imxrt_wdog_disable_all(void)
{
	uint32_t reg;
	irqstate_t flags;

	reg = getreg16(IMXRT_WDOG1_WCR);
	if (reg & WDOG_WCR_WDT_MASK) {
		reg &= ~WDOG_WCR_WDT_MASK;
		putreg16(reg, IMXRT_WDOG1_WCR);
	}

	reg = getreg16(IMXRT_WDOG2_WCR);
	if (reg & WDOG_WCR_WDE_MASK) {
		reg &= ~WDOG_WCR_WDE_MASK;
		putreg16(reg, IMXRT_WDOG2_WCR);
	}

	flags = irqsave();
	putreg32(RTWDOG_UPDATE_KEY, IMXRT_RTWDOG_CNT);
	putreg32(0xFFFF, IMXRT_RTWDOG_TOVAL);
	modifyreg32(IMXRT_RTWDOG_CS, RTWDOG_CS_EN_MASK, RTWDOG_CS_UPDATE_MASK);
	irqrestore(flags);
}

/****************************************************************************
 * Name: imxrt_wdog_hwreset
 *
 * Description:
 *   Called to do hw reset in source code
 *
 ****************************************************************************/
void imxrt_wdog_hwreset(void)
{
	uint32_t reg;
	irqstate_t flags;

	reg = getreg16(IMXRT_WDOG1_WCR);
	if (reg & ~WDOG_WCR_WDT_MASK) {
		reg &= WDOG_WCR_WDT_MASK;
		putreg16(reg, IMXRT_WDOG1_WCR);
	}

	reg = getreg16(IMXRT_WDOG1_WCR);
	if (reg & WDOG_WCR_WDA_MASK) {
		reg &= ~WDOG_WCR_WDA_MASK;
		putreg16(reg, IMXRT_WDOG1_WCR);
	}
}

/****************************************************************************
 * Name: board_reset
 *
 * Description:
 *   Reset board.  This function may or may not be supported by a
 *   particular board architecture.
 *
 * Input Parameters:
 *   status - Status information provided with the reset event.  This
 *     meaning of this status information is board-specific.  If not used by
 *     a board, the value zero may be provided in calls to board_reset.
 *
 * Returned Value:
 *   If this function returns, then it was not possible to power-off the
 *   board due to some constraints.  The return value int this case is a
 *   board-specific reason for the failure to shutdown.
 *
 ****************************************************************************/
#ifdef CONFIG_BOARDCTL_RESET
int board_reset(int status)
{
	imxrt_wdog_hwreset();

	return 0;
}
#endif
