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
/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "imxrt_clock.h"
#include "imxrt_pit.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Gets the instance from the base address to be used to gate or ungate the module clock
 *
 * @param base PIT peripheral base address
 *
 * @return The PIT instance
 */
static uint32_t imxrt_pit_getinstance(PIT_Type *base);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*! @brief Pointers to PIT bases for each instance. */
static PIT_Type *const s_pitBases[] = PIT_BASE_PTRS;

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
/*! @brief Pointers to PIT clocks for each instance. */
static const clock_ip_name_t s_pitClocks[] = PIT_CLOCKS;
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

/*******************************************************************************
 * Code
 ******************************************************************************/
static uint32_t imxrt_pit_getinstance(PIT_Type *base)
{
	uint32_t instance;

	/* Find the instance index from base address mappings. */
	for (instance = 0; instance < ARRAY_SIZE(s_pitBases); instance++) {
		if (s_pitBases[instance] == base) {
			break;
		}
	}

	assert(instance < ARRAY_SIZE(s_pitBases));

	return instance;
}

/*!
 * brief Ungates the PIT clock, enables the PIT module, and configures the peripheral for basic operations.
 *
 * note This API should be called at the beginning of the application using the PIT driver.
 *
 * param base   PIT peripheral base address
 * param config Pointer to the user's PIT config structure
 */
void imxrt_pit_init(PIT_Type *base, const pit_config_t *config)
{
	assert(config);

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
	/* Ungate the PIT clock*/
	imxrt_clock_enableclock(s_pitClocks[imxrt_pit_getinstance(base)]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

#if defined(FSL_FEATURE_PIT_HAS_MDIS) && FSL_FEATURE_PIT_HAS_MDIS
	/* Enable PIT timers */
	base->MCR &= ~PIT_MCR_MDIS_MASK;
#endif

#if defined(FSL_FEATURE_PIT_TIMER_COUNT) && (FSL_FEATURE_PIT_TIMER_COUNT)
	/* Clear the timer enable bit for all channels to make sure the channel's timer is disabled. */
	for (uint8_t i = 0U; i < FSL_FEATURE_PIT_TIMER_COUNT; i++) {
		base->CHANNEL[i].TCTRL &= ~PIT_TCTRL_TEN_MASK;
	}
#endif /* FSL_FEATURE_PIT_TIMER_COUNT */

	/* Config timer operation when in debug mode */
	if (config->enableRunInDebug) {
		base->MCR &= ~PIT_MCR_FRZ_MASK;
	} else {
		base->MCR |= PIT_MCR_FRZ_MASK;
	}
}

/*!
 * brief Gates the PIT clock and disables the PIT module.
 *
 * param base PIT peripheral base address
 */
void imxrt_pit_deinit(PIT_Type *base)
{
#if defined(FSL_FEATURE_PIT_HAS_MDIS) && FSL_FEATURE_PIT_HAS_MDIS
	/* Disable PIT timers */
	base->MCR |= PIT_MCR_MDIS_MASK;
#endif

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
	/* Gate the PIT clock*/
	imxrt_clock_disableclock(s_pitClocks[imxrt_pit_getinstance(base)]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
}

#if defined(FSL_FEATURE_PIT_HAS_LIFETIME_TIMER) && FSL_FEATURE_PIT_HAS_LIFETIME_TIMER

/*!
 * brief Reads the current lifetime counter value.
 *
 * The lifetime timer is a 64-bit timer which chains timer 0 and timer 1 together.
 * Timer 0 and 1 are chained by calling the PIT_SetTimerChainMode before using this timer.
 * The period of lifetime timer is equal to the "period of timer 0 * period of timer 1".
 * For the 64-bit value, the higher 32-bit has the value of timer 1, and the lower 32-bit
 * has the value of timer 0.
 *
 * param base PIT peripheral base address
 *
 * return Current lifetime timer value
 */
uint64_t imxrt_pit_getlifetimetimercount(PIT_Type *base)
{
	uint32_t valueH = 0U;
	uint32_t valueL = 0U;

	/* LTMR64H should be read before LTMR64L */
	valueH = base->LTMR64H;
	valueL = base->LTMR64L;

	return (((uint64_t)valueH << 32U) + (uint64_t)(valueL));
}

#endif /* FSL_FEATURE_PIT_HAS_LIFETIME_TIMER */
