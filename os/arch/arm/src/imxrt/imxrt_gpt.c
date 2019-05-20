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

#include <stdint.h>
#include <string.h>
#include <debug.h>

#include <arch/irq.h>
#include <tinyara/clock.h>

#include "imxrt_clock.h"
#include "imxrt_gpt.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*! @brief Pointers to GPT bases for each instance. */
static GPT_Type *const s_gptBases[] = GPT_BASE_PTRS;

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
/*! @brief Pointers to GPT clocks for each instance. */
static const clock_ip_name_t s_gptClocks[] = GPT_CLOCKS;
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

static struct imxrt_gpt_chipinfo_s imxrt_gpt0_chipinfo = {
	.base   = GPT1,
	.irq_id = IMXRT_IRQ_GPT1,
};

static struct imxrt_gpt_chipinfo_s imxrt_gpt1_chipinfo = {
	.base   = GPT2,
	.irq_id = IMXRT_IRQ_GPT2,
};

/*******************************************************************************
 * Code
 ******************************************************************************/
static uint32_t imxrt_gpt_getinstance(GPT_Type *base)
{
	uint32_t instance;

	/* Find the instance index from base address mappings. */
	for (instance = 0U; instance < ARRAY_SIZE(s_gptBases); instance++) {
		if (s_gptBases[instance] == base) {
			break;
		}
	}

	assert(instance < ARRAY_SIZE(s_gptBases));

	return instance;
}

/*!
 * brief Initialize GPT to reset state and initialize running mode.
 *
 * param base GPT peripheral base address.
 * param initConfig GPT mode setting configuration.
 */
void imxrt_gpt_init(GPT_Type *base, const gpt_config_t *initConfig)
{
	assert(initConfig);

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
	/* Ungate the GPT clock*/
	imxrt_clock_enableclock(s_gptClocks[imxrt_gpt_getinstance(base)]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
	base->CR = 0U;

	imxrt_gpt_softwarereset(base);

	base->CR =
		(initConfig->enableFreeRun ? GPT_CR_FRR_MASK : 0U) | (initConfig->enableRunInWait ? GPT_CR_WAITEN_MASK : 0U) |
		(initConfig->enableRunInStop ? GPT_CR_STOPEN_MASK : 0U) |
		(initConfig->enableRunInDoze ? GPT_CR_DOZEEN_MASK : 0U) |
		(initConfig->enableRunInDbg ? GPT_CR_DBGEN_MASK : 0U) | (initConfig->enableMode ? GPT_CR_ENMOD_MASK : 0U);

	imxrt_gpt_setclocksource(base, initConfig->clockSource);
	if (initConfig->clockSource == kGPT_ClockSource_Osc) {
		imxrt_gpt_setoscclockdivider(base, initConfig->divider);
	} else {
		imxrt_gpt_setclockdivider(base, initConfig->divider);
	}
}

/*!
 * brief Disables the module and gates the GPT clock.
 *
 * param base GPT peripheral base address.
 */
void imxrt_gpt_deinit(GPT_Type *base)
{
	/* Disable GPT timers */
	base->CR = 0U;

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
	/* Gate the GPT clock*/
	imxrt_clock_disableclock(s_gptClocks[imxrt_gpt_getinstance(base)]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
}

/*!
 * brief Fills in the GPT configuration structure with default settings.
 *
 * The default values are:
 * code
 *    config->clockSource = kGPT_ClockSource_Periph;
 *    config->divider = 1U;
 *    config->enableRunInStop = true;
 *    config->enableRunInWait = true;
 *    config->enableRunInDoze = false;
 *    config->enableRunInDbg = false;
 *    config->enableFreeRun = true;
 *    config->enableMode  = true;
 * endcode
 * param config Pointer to the user configuration structure.
 */
void imxrt_gpt_getdefaultconfig(gpt_config_t *config)
{
	assert(config);

	/* Initializes the configure structure to zero. */
	memset(config, 0, sizeof(*config));

	config->clockSource = kGPT_ClockSource_Periph;
	config->divider = 1U;
	config->enableRunInStop = true;
	config->enableRunInWait = true;
	config->enableRunInDoze = false;
	config->enableRunInDbg = false;
	config->enableFreeRun = false;
	config->enableMode = true;
}

void imxrt_gpt_setisr(struct imxrt_gpt_chipinfo_s *gpt, xcpt_t handler, void *arg)
{
	irq_attach(gpt->irq_id, handler, arg);

	up_enable_irq(gpt->irq_id);
}

static uint32_t imxrt_gpt_get_clockfreq(gpt_clock_source_t clock)
{
	uint32_t freq;

	switch (clock) {
	case kGPT_ClockSource_Periph:
		freq = imxrt_clock_getperclkfreq();
		break;
	case kGPT_ClockSource_HighFreq:
		freq = imxrt_clock_getipgfreq();
		break;
	case kGPT_ClockSource_LowFreq:
		freq = imxrt_clock_getrtcfreq();
		break;
	case kGPT_ClockSource_Osc:
		freq = imxrt_clock_getoscfreq();
		break;
	case kGPT_ClockSource_Ext:
	default:
		freq = 0;
		tmrdbg("Invalid Timer clock to get freq\n");
		break;
	}

	return freq;
}

uint32_t imxrt_gpt_convert_time_tick(imxrt_timer_convert_dir_t dir, gpt_clock_source_t clock_source, uint32_t value)
{
	uint64_t converted;
	uint32_t freq;

	freq = imxrt_gpt_get_clockfreq(clock_source);

	switch (dir) {
	case TIME2TICK:
		converted = ((uint64_t)freq * (uint64_t)value) / USEC_PER_SEC;
		break;
	case TICK2TIME:
		converted = ((uint64_t)USEC_PER_SEC * (uint64_t)value) / freq;
		break;
	default:
		tmrdbg("Invalid converting type (%d)\n", dir);
		return 0;
	}

	if (converted > UINT32_MAX) {
		tmrdbg("%lld converted to UINT32_MAX\n", converted);
		converted = UINT32_MAX;
	}

	return (uint32_t)converted;
}


struct imxrt_gpt_chipinfo_s *imxrt_gpt_configure(int timer, gpt_config_t *config)
{
	struct imxrt_gpt_chipinfo_s *gpt = NULL;

	if (timer >= IMXRT_GPT_CH_MAX) {
		return gpt;
	}

	switch (timer) {
	case IMXRT_GPT_CH0:
		gpt = &imxrt_gpt0_chipinfo;
		break;

	case IMXRT_GPT_CH1:
		gpt = &imxrt_gpt1_chipinfo;
		break;
	}

	imxrt_gpt_init(gpt->base, config);

	return gpt;
}
