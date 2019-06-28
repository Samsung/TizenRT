/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
 * os/arch/arm/src/imxrt/imxrt_pwm.c
 *
 *   Copyright (C) 2011-2012, 2014 Gregory Nutt. All rights reserved.
 *   Authors: Gregory Nutt <gnutt@nuttx.org>
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
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>

#include <sys/types.h>
#include <assert.h>
#include <debug.h>
#include <errno.h>
#include <string.h>

#include <tinyara/pwm.h>

#include "up_arch.h"

#if defined(CONFIG_ARCH_CHIP_FAMILY_IMXRT102x)
#include "chip/imxrt102x_config.h"
#include "chip/imxrt102x_features.h"
#elif defined(CONFIG_ARCH_CHIP_FAMILY_IMXRT105x)
#include "chip/imxrt105x_config.h"
#include "chip/imxrt105x_features.h"
#endif

#include "imxrt_clock.h"
#include "imxrt_iomuxc.h"
#include "chip/imxrt_memorymap.h"
#include "imxrt_pwm.h"

/****************************************************************************
 * Private Types
 ****************************************************************************/
#ifndef CONFIG_PWM1_SUB1_CLOCK
#define CONFIG_PWM1_SUB1_CLOCK	0
#endif
#ifndef CONFIG_PWM1_SUB2_CLOCK
#define CONFIG_PWM1_SUB2_CLOCK	0
#endif
#ifndef CONFIG_PWM1_SUB3_CLOCK
#define CONFIG_PWM1_SUB3_CLOCK	0
#endif
#ifndef CONFIG_PWM1_SUB4_CLOCK
#define CONFIG_PWM1_SUB4_CLOCK	0
#endif
#ifndef CONFIG_PWM2_SUB1_CLOCK
#define CONFIG_PWM2_SUB1_CLOCK	0
#endif
#ifndef CONFIG_PWM2_SUB2_CLOCK
#define CONFIG_PWM2_SUB2_CLOCK	0
#endif
#ifndef CONFIG_PWM2_SUB3_CLOCK
#define CONFIG_PWM2_SUB3_CLOCK	0
#endif
#ifndef CONFIG_PWM2_SUB4_CLOCK
#define CONFIG_PWM2_SUB4_CLOCK	0
#endif

#if defined(CONFIG_ARCH_CHIP_FAMILY_IMXRT105x)
#ifndef CONFIG_PWM3_SUB1_CLOCK
#define CONFIG_PWM3_SUB1_CLOCK	0
#endif
#ifndef CONFIG_PWM3_SUB2_CLOCK
#define CONFIG_PWM3_SUB2_CLOCK	0
#endif
#ifndef CONFIG_PWM3_SUB3_CLOCK
#define CONFIG_PWM3_SUB3_CLOCK	0
#endif
#ifndef CONFIG_PWM3_SUB4_CLOCK
#define CONFIG_PWM3_SUB4_CLOCK	0
#endif
#ifndef CONFIG_PWM4_SUB1_CLOCK
#define CONFIG_PWM4_SUB1_CLOCK	0
#endif
#ifndef CONFIG_PWM4_SUB2_CLOCK
#define CONFIG_PWM4_SUB2_CLOCK	0
#endif
#ifndef CONFIG_PWM4_SUB3_CLOCK
#define CONFIG_PWM4_SUB3_CLOCK	0
#endif
#ifndef CONFIG_PWM4_SUB4_CLOCK
#define CONFIG_PWM4_SUB4_CLOCK	0
#endif
#endif

/* channel structure */
struct imxrt_pwmch_s {
	FAR const struct pwm_ops_s *ops; /* PWM operations */
	uint8_t ch_id;					 /* Channel ID {1,...,32} */
	uint8_t controller_id;			 /* Which controller the channel is in */
	uint8_t submodule_id;			 /* Which submodule the channel is in */
	uint8_t ch_name;				 /* See the pwm_channels_t definitions */
	PWM_Type *base;					 /* The controller register base */
	uint8_t submodule_mode;			 /* See the pwm_chnl_pair_operation_t definitions */
	uint8_t ch_mode;				 /* See the pwm_mode_t definitions */
	uint32_t pincfg;				 /* Output pin configuration */
	uint32_t freq;					 /* Current output frequency */
	uint32_t duty;					 /* Current output duty */
};
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Get the instance from the base address
 *
 * @param base PWM peripheral base address
 *
 * @return The PWM module instance
 */
static uint32_t imxrt_pwm_getinstance(PWM_Type *base);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*! @brief Pointers to PWM bases for each instance. */
static PWM_Type *const s_pwmBases[] = PWM_BASE_PTRS;

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
/*! @brief Pointers to PWM clocks for each PWM submodule. */
static const clock_ip_name_t s_pwmClocks[][FSL_FEATURE_PWM_SUBMODULE_COUNT] = PWM_CLOCKS;
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

/****************************************************************************
 * Private Functions
 ****************************************************************************/
/*******************************************************************************
 * Code
 ******************************************************************************/
static uint32_t imxrt_pwm_getinstance(PWM_Type *base)
{
	uint32_t instance;

	/* Find the instance index from base address mappings. */
	for (instance = 0; instance < ARRAY_SIZE(s_pwmBases); instance++) {
		if (s_pwmBases[instance] == base) {
			break;
		}
	}

	assert(instance < ARRAY_SIZE(s_pwmBases));

	return instance;
}

/*!
 * brief Ungates the PWM submodule clock and configures the peripheral for basic operation.
 *
 * note This API should be called at the beginning of the application using the PWM driver.
 *
 * param base      PWM peripheral base address
 * param subModule PWM submodule to configure
 * param config    Pointer to user's PWM config structure.
 *
 * return kStatus_Success means success; else failed.
 */
status_t imxrt_pwm_init(PWM_Type *base, pwm_submodule_t subModule, const pwm_config_t *config)
{
	assert(config);

	uint16_t reg;

	/* Source clock for submodule 0 cannot be itself */
	if ((config->clockSource == kPWM_Submodule0Clock) && (subModule == kPWM_Module_0)) {
		return kStatus_Fail;
	}

	/* Reload source select clock for submodule 0 cannot be master reload */
	if ((config->reloadSelect == kPWM_MasterReload) && (subModule == kPWM_Module_0)) {
		return kStatus_Fail;
	}

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
	/* Ungate the PWM submodule clock*/
	imxrt_clock_enableclock(s_pwmClocks[imxrt_pwm_getinstance(base)][subModule]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

	/* Clear the fault status flags */
	base->FSTS |= PWM_FSTS_FFLAG_MASK;

	reg = base->SM[subModule].CTRL2;

	/* Setup the submodule clock-source, control source of the INIT signal,
		* source of the force output signal, operation in debug & wait modes and reload source select
		*/
	reg &= ~(PWM_CTRL2_CLK_SEL_MASK | PWM_CTRL2_FORCE_SEL_MASK | PWM_CTRL2_INIT_SEL_MASK | PWM_CTRL2_INDEP_MASK |
			 PWM_CTRL2_WAITEN_MASK | PWM_CTRL2_DBGEN_MASK | PWM_CTRL2_RELOAD_SEL_MASK);
	reg |= (PWM_CTRL2_CLK_SEL(config->clockSource) | PWM_CTRL2_FORCE_SEL(config->forceTrigger) |
			PWM_CTRL2_INIT_SEL(config->initializationControl) | PWM_CTRL2_DBGEN(config->enableDebugMode) |
			PWM_CTRL2_WAITEN(config->enableWait) | PWM_CTRL2_RELOAD_SEL(config->reloadSelect));

	/* Setup PWM A & B to be independent or a complementary-pair */
	switch (config->pairOperation) {
	case kPWM_Independent:
		reg |= PWM_CTRL2_INDEP_MASK;
		break;
	case kPWM_ComplementaryPwmA:
		base->MCTRL &= ~(1U << (PWM_MCTRL_IPOL_SHIFT + subModule));
		break;
	case kPWM_ComplementaryPwmB:
		base->MCTRL |= (1U << (PWM_MCTRL_IPOL_SHIFT + subModule));
		break;
	default:
		break;
	}
	base->SM[subModule].CTRL2 = reg;

	reg = base->SM[subModule].CTRL;

	/* Setup the clock prescale, load mode and frequency */
	reg &= ~(PWM_CTRL_PRSC_MASK | PWM_CTRL_LDFQ_MASK | PWM_CTRL_LDMOD_MASK);
	reg |= (PWM_CTRL_PRSC(config->prescale) | PWM_CTRL_LDFQ(config->reloadFrequency));

	/* Setup register reload logic */
	switch (config->reloadLogic) {
	case kPWM_ReloadImmediate:
		reg |= PWM_CTRL_LDMOD_MASK;
		break;
	case kPWM_ReloadPwmHalfCycle:
		reg |= PWM_CTRL_HALF_MASK;
		reg &= ~PWM_CTRL_FULL_MASK;
		break;
	case kPWM_ReloadPwmFullCycle:
		reg &= ~PWM_CTRL_HALF_MASK;
		reg |= PWM_CTRL_FULL_MASK;
		break;
	case kPWM_ReloadPwmHalfAndFullCycle:
		reg |= PWM_CTRL_HALF_MASK;
		reg |= PWM_CTRL_FULL_MASK;
		break;
	default:
		break;
	}
	base->SM[subModule].CTRL = reg;

	/* Setup the fault filter */
	if (base->FFILT & PWM_FFILT_FILT_PER_MASK) {
		/* When changing values for fault period from a non-zero value, first write a value of 0
			* to clear the filter
			*/
		base->FFILT &= ~(PWM_FFILT_FILT_PER_MASK);
	}

	base->FFILT = (PWM_FFILT_FILT_CNT(config->faultFilterCount) | PWM_FFILT_FILT_PER(config->faultFilterPeriod));

	/* Issue a Force trigger event when configured to trigger locally */
	if (config->forceTrigger == kPWM_Force_Local) {
		base->SM[subModule].CTRL2 |= PWM_CTRL2_FORCE(1U);
	}

	return kStatus_Success;
}

/*!
 * brief Gate the PWM submodule clock
 *
 * param base      PWM peripheral base address
 * param subModule PWM submodule to deinitialize
 */
void imxrt_pwm_deinit(PWM_Type *base, pwm_submodule_t subModule)
{
	/* Stop the submodule */
	base->MCTRL &= ~(1U << (PWM_MCTRL_RUN_SHIFT + subModule));

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
	/* Gate the PWM submodule clock*/
	imxrt_clock_disableclock(s_pwmClocks[imxrt_pwm_getinstance(base)][subModule]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
}

/*!
 * brief  Fill in the PWM config struct with the default settings
 *
 * The default values are:
 * code
 *   config->enableDebugMode = false;
 *   config->enableWait = false;
 *   config->reloadSelect = kPWM_LocalReload;
 *   config->faultFilterCount = 0;
 *   config->faultFilterPeriod = 0;
 *   config->clockSource = kPWM_BusClock;
 *   config->prescale = kPWM_Prescale_Divide_1;
 *   config->initializationControl = kPWM_Initialize_LocalSync;
 *   config->forceTrigger = kPWM_Force_Local;
 *   config->reloadFrequency = kPWM_LoadEveryOportunity;
 *   config->reloadLogic = kPWM_ReloadImmediate;
 *   config->pairOperation = kPWM_Independent;
 * endcode
 * param config Pointer to user's PWM config structure.
 */
void imxrt_pwm_getdefaultconfig(pwm_config_t *config)
{
	assert(config);

	/* Initializes the configure structure to zero. */
	memset(config, 0, sizeof(*config));

	/* PWM is paused in debug mode */
	config->enableDebugMode = false;
	/* PWM is paused in wait mode */
	config->enableWait = false;
	/* PWM module uses the local reload signal to reload registers */
	config->reloadSelect = kPWM_LocalReload;
	/* Fault filter count is set to 0 */
	config->faultFilterCount = 0;
	/* Fault filter period is set to 0 which disables the fault filter */
	config->faultFilterPeriod = 0;
	/* Use the IP Bus clock as source clock for the PWM submodule */
	config->clockSource = kPWM_BusClock;
	/* Clock source prescale is set to divide by 1*/
	config->prescale = kPWM_Prescale_Divide_1;
	/* Local sync causes initialization */
	config->initializationControl = kPWM_Initialize_LocalSync;
	/* The local force signal, CTRL2[FORCE], from the submodule is used to force updates */
	config->forceTrigger = kPWM_Force_Local;
	/* PWM reload frequency, reload opportunity is PWM half cycle or full cycle.
		* This field is not used in Immediate reload mode
		*/
	config->reloadFrequency = kPWM_LoadEveryOportunity;
	/* Buffered-registers get loaded with new values as soon as LDOK bit is set */
	config->reloadLogic = kPWM_ReloadImmediate;
	/* PWM A & PWM B operate as 2 independent channels */
	config->pairOperation = kPWM_Independent;
}

/*!
 * brief Sets up the PWM signals for a PWM submodule.
 *
 * The function initializes the submodule according to the parameters passed in by the user. The function
 * also sets up the value compare registers to match the PWM signal requirements.
 * If the dead time insertion logic is enabled, the pulse period is reduced by the
 * dead time period specified by the user.
 *
 * param base        PWM peripheral base address
 * param subModule   PWM submodule to configure
 * param chnlParams  Array of PWM channel parameters to configure the channel(s)
 * param numOfChnls  Number of channels to configure, this should be the size of the array passed in.
 *                    Array size should not be more than 2 as each submodule has 2 pins to output PWM
 * param mode        PWM operation mode, options available in enumeration ::pwm_mode_t
 * param pwmFreq_Hz  PWM signal frequency in Hz
 * param srcClock_Hz PWM main counter clock in Hz.
 *
 * return Returns kStatusFail if there was error setting up the signal; kStatusSuccess otherwise
 */
status_t imxrt_pwm_setuppwm(PWM_Type *base,
							pwm_submodule_t subModule,
							const pwm_signal_param_t *chnlParams,
							uint8_t numOfChnls,
							pwm_mode_t mode,
							uint32_t pwmFreq_Hz,
							uint32_t srcClock_Hz)
{
	assert(chnlParams);
	assert(pwmFreq_Hz);
	assert(numOfChnls);
	assert(srcClock_Hz);

	uint32_t pwmClock;
	uint16_t pulseCnt = 0, pwmHighPulse = 0;
	int16_t modulo = 0;
	uint8_t i, polarityShift = 0, outputEnableShift = 0;

	if (numOfChnls > 2) {
		/* Each submodule has 2 signals; PWM A & PWM B */
		return kStatus_Fail;
	}

	/* Divide the clock by the prescale value */
	pwmClock = (srcClock_Hz / (1U << ((base->SM[subModule].CTRL & PWM_CTRL_PRSC_MASK) >> PWM_CTRL_PRSC_SHIFT)));
	pulseCnt = (pwmClock / pwmFreq_Hz);

	/* Setup each PWM channel */
	for (i = 0; i < numOfChnls; i++) {
		/* Calculate pulse width */
		pwmHighPulse = (pulseCnt * chnlParams->dutyCyclePercent) / 100;

		/* Setup the different match registers to generate the PWM signal */
		switch (mode) {
		case kPWM_SignedCenterAligned:
			/* Setup the PWM period for a signed center aligned signal */
			modulo = pulseCnt >> 1;
			/* Indicates the start of the PWM period */
			base->SM[subModule].INIT = (-modulo);
			/* Indicates the center value */
			base->SM[subModule].VAL0 = 0;
			/* Indicates the end of the PWM period */
			base->SM[subModule].VAL1 = modulo;

			/* Setup the PWM dutycycle */
			if (chnlParams->pwmChannel == kPWM_PwmA) {
				base->SM[subModule].VAL2 = (-(pwmHighPulse / 2));
				base->SM[subModule].VAL3 = (pwmHighPulse / 2);
			} else {
				base->SM[subModule].VAL4 = (-(pwmHighPulse / 2));
				base->SM[subModule].VAL5 = (pwmHighPulse / 2);
			}
			break;
		case kPWM_CenterAligned:
			/* Setup the PWM period for an unsigned center aligned signal */
			/* Indicates the start of the PWM period */
			base->SM[subModule].INIT = 0;
			/* Indicates the center value */
			base->SM[subModule].VAL0 = (pulseCnt / 2);
			/* Indicates the end of the PWM period */
			base->SM[subModule].VAL1 = pulseCnt;

			/* Setup the PWM dutycycle */
			if (chnlParams->pwmChannel == kPWM_PwmA) {
				base->SM[subModule].VAL2 = ((pulseCnt - pwmHighPulse) / 2);
				base->SM[subModule].VAL3 = ((pulseCnt + pwmHighPulse) / 2);
			} else {
				base->SM[subModule].VAL4 = ((pulseCnt - pwmHighPulse) / 2);
				base->SM[subModule].VAL5 = ((pulseCnt + pwmHighPulse) / 2);
			}
			break;
		case kPWM_SignedEdgeAligned:
			/* Setup the PWM period for a signed edge aligned signal */
			modulo = pulseCnt >> 1;
			/* Indicates the start of the PWM period */
			base->SM[subModule].INIT = (-modulo);
			/* Indicates the center value */
			base->SM[subModule].VAL0 = 0;
			/* Indicates the end of the PWM period */
			base->SM[subModule].VAL1 = modulo;

			/* Setup the PWM dutycycle */
			if (chnlParams->pwmChannel == kPWM_PwmA) {
				base->SM[subModule].VAL2 = (-modulo);
				base->SM[subModule].VAL3 = (-modulo + pwmHighPulse);
			} else {
				base->SM[subModule].VAL4 = (-modulo);
				base->SM[subModule].VAL5 = (-modulo + pwmHighPulse);
			}
			break;
		case kPWM_EdgeAligned:
			/* Setup the PWM period for a unsigned edge aligned signal */
			/* Indicates the start of the PWM period */
			base->SM[subModule].INIT = 0;
			/* Indicates the center value */
			base->SM[subModule].VAL0 = (pulseCnt / 2);
			/* Indicates the end of the PWM period */
			base->SM[subModule].VAL1 = pulseCnt;

			/* Setup the PWM dutycycle */
			if (chnlParams->pwmChannel == kPWM_PwmA) {
				base->SM[subModule].VAL2 = 0;
				base->SM[subModule].VAL3 = pwmHighPulse;
			} else {
				base->SM[subModule].VAL4 = 0;
				base->SM[subModule].VAL5 = pwmHighPulse;
			}
			break;
		default:
			break;
		}
		/* Setup register shift values based on the channel being configured.
			* Also setup the deadtime value
			*/
		if (chnlParams->pwmChannel == kPWM_PwmA) {
			polarityShift = PWM_OCTRL_POLA_SHIFT;
			outputEnableShift = PWM_OUTEN_PWMA_EN_SHIFT;
			base->SM[subModule].DTCNT0 = PWM_DTCNT0_DTCNT0(chnlParams->deadtimeValue);
		} else {
			polarityShift = PWM_OCTRL_POLB_SHIFT;
			outputEnableShift = PWM_OUTEN_PWMB_EN_SHIFT;
			base->SM[subModule].DTCNT1 = PWM_DTCNT1_DTCNT1(chnlParams->deadtimeValue);
		}

		/* Setup signal active level */
		if (chnlParams->level == kPWM_HighTrue) {
			base->SM[subModule].OCTRL &= ~(1U << polarityShift);
		} else {
			base->SM[subModule].OCTRL |= (1U << polarityShift);
		}
		/* Enable PWM output */
		base->OUTEN |= (1U << (outputEnableShift + subModule));

		/* Get the next channel parameters */
		chnlParams++;
	}

	return kStatus_Success;
}

/*!
 * brief Updates the PWM signal's dutycycle.
 *
 * The function updates the PWM dutycyle to the new value that is passed in.
 * If the dead time insertion logic is enabled then the pulse period is reduced by the
 * dead time period specified by the user.
 *
 * param base              PWM peripheral base address
 * param subModule         PWM submodule to configure
 * param pwmSignal         Signal (PWM A or PWM B) to update
 * param currPwmMode       The current PWM mode set during PWM setup
 * param dutyCyclePercent  New PWM pulse width, value should be between 0 to 100
 *                          0=inactive signal(0% duty cycle)...
 *                          100=active signal (100% duty cycle)
 */
void imxrt_pwm_updatepwmdutycycle(PWM_Type *base,
								  pwm_submodule_t subModule,
								  pwm_channels_t pwmSignal,
								  pwm_mode_t currPwmMode,
								  uint8_t dutyCyclePercent)
{
	assert(dutyCyclePercent <= 100);
	assert(pwmSignal < 2);
	uint16_t pulseCnt = 0, pwmHighPulse = 0;
	int16_t modulo = 0;

	switch (currPwmMode) {
	case kPWM_SignedCenterAligned:
		modulo = base->SM[subModule].VAL1;
		pulseCnt = modulo * 2;
		/* Calculate pulse width */
		pwmHighPulse = (pulseCnt * dutyCyclePercent) / 100;

		/* Setup the PWM dutycycle */
		if (pwmSignal == kPWM_PwmA) {
			base->SM[subModule].VAL2 = (-(pwmHighPulse / 2));
			base->SM[subModule].VAL3 = (pwmHighPulse / 2);
		} else {
			base->SM[subModule].VAL4 = (-(pwmHighPulse / 2));
			base->SM[subModule].VAL5 = (pwmHighPulse / 2);
		}
		break;
	case kPWM_CenterAligned:
		pulseCnt = base->SM[subModule].VAL1;
		/* Calculate pulse width */
		pwmHighPulse = (pulseCnt * dutyCyclePercent) / 100;

		/* Setup the PWM dutycycle */
		if (pwmSignal == kPWM_PwmA) {
			base->SM[subModule].VAL2 = ((pulseCnt - pwmHighPulse) / 2);
			base->SM[subModule].VAL3 = ((pulseCnt + pwmHighPulse) / 2);
		} else {
			base->SM[subModule].VAL4 = ((pulseCnt - pwmHighPulse) / 2);
			base->SM[subModule].VAL5 = ((pulseCnt + pwmHighPulse) / 2);
		}
		break;
	case kPWM_SignedEdgeAligned:
		modulo = base->SM[subModule].VAL1;
		pulseCnt = modulo * 2;
		/* Calculate pulse width */
		pwmHighPulse = (pulseCnt * dutyCyclePercent) / 100;

		/* Setup the PWM dutycycle */
		if (pwmSignal == kPWM_PwmA) {
			base->SM[subModule].VAL2 = (-modulo);
			base->SM[subModule].VAL3 = (-modulo + pwmHighPulse);
		} else {
			base->SM[subModule].VAL4 = (-modulo);
			base->SM[subModule].VAL5 = (-modulo + pwmHighPulse);
		}
		break;
	case kPWM_EdgeAligned:
		pulseCnt = base->SM[subModule].VAL1;
		/* Calculate pulse width */
		pwmHighPulse = (pulseCnt * dutyCyclePercent) / 100;

		/* Setup the PWM dutycycle */
		if (pwmSignal == kPWM_PwmA) {
			base->SM[subModule].VAL2 = 0;
			base->SM[subModule].VAL3 = pwmHighPulse;
		} else {
			base->SM[subModule].VAL4 = 0;
			base->SM[subModule].VAL5 = pwmHighPulse;
		}
		break;
	default:
		break;
	}
}

/*!
 * brief Sets up the PWM input capture
 *
 * Each PWM submodule has 3 pins that can be configured for use as input capture pins. This function
 * sets up the capture parameters for each pin and enables the pin for input capture operation.
 *
 * param base               PWM peripheral base address
 * param subModule          PWM submodule to configure
 * param pwmChannel         Channel in the submodule to setup
 * param inputCaptureParams Parameters passed in to set up the input pin
 */
void imxrt_pwm_setupinputcapture(PWM_Type *base,
								 pwm_submodule_t subModule,
								 pwm_channels_t pwmChannel,
								 const pwm_input_capture_param_t *inputCaptureParams)
{
	uint32_t reg = 0;
	switch (pwmChannel) {
	case kPWM_PwmA:
		/* Setup the capture paramters for PWM A pin */
		reg = (PWM_CAPTCTRLA_INP_SELA(inputCaptureParams->captureInputSel) |
			   PWM_CAPTCTRLA_EDGA0(inputCaptureParams->edge0) | PWM_CAPTCTRLA_EDGA1(inputCaptureParams->edge1) |
			   PWM_CAPTCTRLA_ONESHOTA(inputCaptureParams->enableOneShotCapture) |
			   PWM_CAPTCTRLA_CFAWM(inputCaptureParams->fifoWatermark));
		/* Enable the edge counter if using the output edge counter */
		if (inputCaptureParams->captureInputSel) {
			reg |= PWM_CAPTCTRLA_EDGCNTA_EN_MASK;
		}
		/* Enable input capture operation */
		reg |= PWM_CAPTCTRLA_ARMA_MASK;

		base->SM[subModule].CAPTCTRLA = reg;

		/* Setup the compare value when using the edge counter as source */
		base->SM[subModule].CAPTCOMPA = PWM_CAPTCOMPA_EDGCMPA(inputCaptureParams->edgeCompareValue);
		/* Setup PWM A pin for input capture */
		base->OUTEN &= ~(1U << (PWM_OUTEN_PWMA_EN_SHIFT + subModule));

		break;
	case kPWM_PwmB:
		/* Setup the capture paramters for PWM B pin */
		reg = (PWM_CAPTCTRLB_INP_SELB(inputCaptureParams->captureInputSel) |
			   PWM_CAPTCTRLB_EDGB0(inputCaptureParams->edge0) | PWM_CAPTCTRLB_EDGB1(inputCaptureParams->edge1) |
			   PWM_CAPTCTRLB_ONESHOTB(inputCaptureParams->enableOneShotCapture) |
			   PWM_CAPTCTRLB_CFBWM(inputCaptureParams->fifoWatermark));
		/* Enable the edge counter if using the output edge counter */
		if (inputCaptureParams->captureInputSel) {
			reg |= PWM_CAPTCTRLB_EDGCNTB_EN_MASK;
		}
		/* Enable input capture operation */
		reg |= PWM_CAPTCTRLB_ARMB_MASK;

		base->SM[subModule].CAPTCTRLB = reg;

		/* Setup the compare value when using the edge counter as source */
		base->SM[subModule].CAPTCOMPB = PWM_CAPTCOMPB_EDGCMPB(inputCaptureParams->edgeCompareValue);
		/* Setup PWM B pin for input capture */
		base->OUTEN &= ~(1U << (PWM_OUTEN_PWMB_EN_SHIFT + subModule));
		break;
	case kPWM_PwmX:
		reg = (PWM_CAPTCTRLX_INP_SELX(inputCaptureParams->captureInputSel) |
			   PWM_CAPTCTRLX_EDGX0(inputCaptureParams->edge0) | PWM_CAPTCTRLX_EDGX1(inputCaptureParams->edge1) |
			   PWM_CAPTCTRLX_ONESHOTX(inputCaptureParams->enableOneShotCapture) |
			   PWM_CAPTCTRLX_CFXWM(inputCaptureParams->fifoWatermark));
		/* Enable the edge counter if using the output edge counter */
		if (inputCaptureParams->captureInputSel) {
			reg |= PWM_CAPTCTRLX_EDGCNTX_EN_MASK;
		}
		/* Enable input capture operation */
		reg |= PWM_CAPTCTRLX_ARMX_MASK;

		base->SM[subModule].CAPTCTRLX = reg;

		/* Setup the compare value when using the edge counter as source */
		base->SM[subModule].CAPTCOMPX = PWM_CAPTCOMPX_EDGCMPX(inputCaptureParams->edgeCompareValue);
		/* Setup PWM X pin for input capture */
		base->OUTEN &= ~(1U << (PWM_OUTEN_PWMX_EN_SHIFT + subModule));
		break;
	default:
		break;
	}
}

/*!
 * brief Sets up the PWM fault protection.
 *
 * PWM has 4 fault inputs.
 *
 * param base        PWM peripheral base address
 * param faultNum    PWM fault to configure.
 * param faultParams Pointer to the PWM fault config structure
 */
void imxrt_pwm_setupfaults(PWM_Type *base, pwm_fault_input_t faultNum, const pwm_fault_param_t *faultParams)
{
	assert(faultParams);
	uint16_t reg;

	reg = base->FCTRL;
	/* Set the faults level-settting */
	if (faultParams->faultLevel) {
		reg |= (1U << (PWM_FCTRL_FLVL_SHIFT + faultNum));
	} else {
		reg &= ~(1U << (PWM_FCTRL_FLVL_SHIFT + faultNum));
	}
	/* Set the fault clearing mode */
	if (faultParams->faultClearingMode) {
		/* Use manual fault clearing */
		reg &= ~(1U << (PWM_FCTRL_FAUTO_SHIFT + faultNum));
		if (faultParams->faultClearingMode == kPWM_ManualSafety) {
			/* Use manual fault clearing with safety mode enabled */
			reg |= (1U << (PWM_FCTRL_FSAFE_SHIFT + faultNum));
		} else {
			/* Use manual fault clearing with safety mode disabled */
			reg &= ~(1U << (PWM_FCTRL_FSAFE_SHIFT + faultNum));
		}
	} else {
		/* Use automatic fault clearing */
		reg |= (1U << (PWM_FCTRL_FAUTO_SHIFT + faultNum));
	}
	base->FCTRL = reg;

	/* Set the combinational path option */
	if (faultParams->enableCombinationalPath) {
		/* Combinational path from the fault input to the PWM output is available */
		base->FCTRL2 &= ~(1U << faultNum);
	} else {
		/* No combinational path available, only fault filter & latch signal can disable PWM output */
		base->FCTRL2 |= (1U << faultNum);
	}

	/* Initially clear both recovery modes */
	reg = base->FSTS;
	reg &= ~((1U << (PWM_FSTS_FFULL_SHIFT + faultNum)) | (1U << (PWM_FSTS_FHALF_SHIFT + faultNum)));
	/* Setup fault recovery */
	switch (faultParams->recoverMode) {
	case kPWM_NoRecovery:
		break;
	case kPWM_RecoverHalfCycle:
		reg |= (1U << (PWM_FSTS_FHALF_SHIFT + faultNum));
		break;
	case kPWM_RecoverFullCycle:
		reg |= (1U << (PWM_FSTS_FFULL_SHIFT + faultNum));
		break;
	case kPWM_RecoverHalfAndFullCycle:
		reg |= (1U << (PWM_FSTS_FHALF_SHIFT + faultNum));
		reg |= (1U << (PWM_FSTS_FFULL_SHIFT + faultNum));
		break;
	default:
		break;
	}
	base->FSTS = reg;
}

/*!
 * brief Selects the signal to output on a PWM pin when a FORCE_OUT signal is asserted.
 *
 * The user specifies which channel to configure by supplying the submodule number and whether
 * to modify PWM A or PWM B within that submodule.
 *
 * param base       PWM peripheral base address
 * param subModule  PWM submodule to configure
 * param pwmChannel Channel to configure
 * param mode       Signal to output when a FORCE_OUT is triggered
 */
void imxrt_pwm_setupforcesignal(PWM_Type *base, pwm_submodule_t subModule, pwm_channels_t pwmChannel, pwm_force_signal_t mode)

{
	uint16_t shift;
	uint16_t reg;

	/* DTSRCSEL register has 4 bits per submodule; 2 bits for PWM A and 2 bits for PWM B */
	shift = subModule * 4 + pwmChannel * 2;

	/* Setup the signal to be passed upon occurrence of a FORCE_OUT signal */
	reg = base->DTSRCSEL;
	reg &= ~(0x3U << shift);
	reg |= (uint16_t)((uint16_t)mode << shift);
	base->DTSRCSEL = reg;
}

/*!
 * brief Enables the selected PWM interrupts
 *
 * param base      PWM peripheral base address
 * param subModule PWM submodule to configure
 * param mask      The interrupts to enable. This is a logical OR of members of the
 *                  enumeration ::pwm_interrupt_enable_t
 */
void imxrt_pwm_enableinterrupts(PWM_Type *base, pwm_submodule_t subModule, uint32_t mask)
{
	/* Upper 16 bits are for related to the submodule */
	base->SM[subModule].INTEN |= (mask & 0xFFFFU);
	/* Fault related interrupts */
	base->FCTRL |= ((mask >> 16U) & PWM_FCTRL_FIE_MASK);
}

/*!
 * brief Disables the selected PWM interrupts
 *
 * param base      PWM peripheral base address
 * param subModule PWM submodule to configure
 * param mask      The interrupts to enable. This is a logical OR of members of the
 *                  enumeration ::pwm_interrupt_enable_t
 */
void imxrt_pwm_disableinterrupts(PWM_Type *base, pwm_submodule_t subModule, uint32_t mask)
{
	base->SM[subModule].INTEN &= ~(mask & 0xFFFF);
	base->FCTRL &= ~((mask >> 16U) & PWM_FCTRL_FIE_MASK);
}

/*!
 * brief Gets the enabled PWM interrupts
 *
 * param base      PWM peripheral base address
 * param subModule PWM submodule to configure
 *
 * return The enabled interrupts. This is the logical OR of members of the
 *         enumeration ::pwm_interrupt_enable_t
 */
uint32_t imxrt_pwm_getenabledinterrupts(PWM_Type *base, pwm_submodule_t subModule)
{
	uint32_t enabledInterrupts;

	enabledInterrupts = base->SM[subModule].INTEN;
	enabledInterrupts |= ((uint32_t)(base->FCTRL & PWM_FCTRL_FIE_MASK) << 16U);
	return enabledInterrupts;
}

/*!
 * brief Gets the PWM status flags
 *
 * param base      PWM peripheral base address
 * param subModule PWM submodule to configure
 *
 * return The status flags. This is the logical OR of members of the
 *         enumeration ::pwm_status_flags_t
 */
uint32_t imxrt_pwm_getstatusflags(PWM_Type *base, pwm_submodule_t subModule)
{
	uint32_t statusFlags;

	statusFlags = base->SM[subModule].STS;
	statusFlags |= ((uint32_t)(base->FSTS & PWM_FSTS_FFLAG_MASK) << 16U);

	return statusFlags;
}

/*!
 * brief Clears the PWM status flags
 *
 * param base      PWM peripheral base address
 * param subModule PWM submodule to configure
 * param mask      The status flags to clear. This is a logical OR of members of the
 *                  enumeration ::pwm_status_flags_t
 */
void imxrt_pwm_clearstatusflags(PWM_Type *base, pwm_submodule_t subModule, uint32_t mask)
{
	uint16_t reg;

	base->SM[subModule].STS = (mask & 0xFFFFU);
	reg = base->FSTS;
	/* Clear the fault flags and set only the ones we wish to clear as the fault flags are cleared
		* by writing a login one
		*/
	reg &= ~(PWM_FSTS_FFLAG_MASK);
	reg |= ((mask >> 16U) & PWM_FSTS_FFLAG_MASK);
	base->FSTS = reg;
}

/****************************************************************************
 * Name: imxrt_pwm_setup
 *
 * Description:
 *   This method is called when the driver is opened.  The lower half driver
 *   should configure and initialize the device so that it is ready for use.
 *   It should not, however, output pulses until the start method is called.
 *
 * Input parameters:
 *   dev - A reference to the lower half PWM driver state structure
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure
 *
 * Assumptions:
 *   APB1 or 2 clocking for the GPIOs has already been configured by the RCC
 *   logic at power up.
 *
 ****************************************************************************/
static int imxrt_pwm_setup(FAR struct pwm_lowerhalf_s *dev)
{
	FAR struct imxrt_pwmch_s *priv = (FAR struct imxrt_pwmch_s *)dev;
	PWM_Type *base;
	pwm_submodule_t submodule;

	base = priv->base;
	submodule = priv->submodule_id;

	base->SM[submodule].DISMAP[0] = 0;
	base->SM[submodule].DISMAP[1] = 0;

	return OK;
}

/****************************************************************************
 * Name: imxrt_pwm_start
 *
 * Description:
 *   (Re-)initialize the timer resources and start the pulsed output
 *
 * Input parameters:
 *   dev - A reference to the lower half PWM driver state structure
 *   info - A reference to the characteristics of the pulsed output
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure
 *
 ****************************************************************************/
static int imxrt_pwm_start(FAR struct pwm_lowerhalf_s *dev,
						   FAR const struct pwm_info_s *info)
{
	FAR struct imxrt_pwmch_s *priv = (FAR struct imxrt_pwmch_s *)dev;
	PWM_Type *base;
	pwm_submodule_t submodule;
	pwm_mode_t mode;
	uint8_t pwm_ch;
	uint32_t pwmClock;
	uint32_t srcClock_Hz;
	uint16_t pulseCnt = 0;
	uint16_t pwmHighPulse = 0;
	uint16_t dutycycle = 32768;
	int16_t modulo = 0;
	uint8_t polarityShift = 0;
	uint8_t outputEnableShift = 0;
	pwm_level_select_t level = kPWM_HighTrue;
	uint16_t deadtimeValue = 0;
	uint32_t pwmFreq_Hz = 10000;

	srcClock_Hz = imxrt_clock_getfreq(kCLOCK_IpgClk);

	base = priv->base;
	submodule = priv->submodule_id;
	pwm_ch = priv->ch_id;
	mode = priv->ch_mode;

	dutycycle = info->duty;
	if (!info->frequency)
		return -1;
	pwmFreq_Hz = info->frequency;

	/* Divide the clock by the prescale value */
	pwmClock = (srcClock_Hz / (1U << ((base->SM[submodule].CTRL & PWM_CTRL_PRSC_MASK) >> PWM_CTRL_PRSC_SHIFT)));
	pulseCnt = (pwmClock / pwmFreq_Hz);
	/* Setup each PWM channel */
	/* Calculate pulse width */
	pwmHighPulse = ((uint32_t)pulseCnt * (uint32_t)dutycycle) / 65535;
	/* Setup the different match registers to generate the PWM signal */
	switch (mode) {
	case kPWM_SignedCenterAligned:
		/* Setup the PWM period for a signed center aligned signal */
		modulo = pulseCnt >> 1;
		/* Indicates the start of the PWM period */
		base->SM[submodule].INIT = (-modulo);
		/* Indicates the center value */
		base->SM[submodule].VAL0 = 0;
		/* Indicates the end of the PWM period */
		base->SM[submodule].VAL1 = modulo;

		/* Setup the PWM dutycycle */
		if (pwm_ch % 2) {
			base->SM[submodule].VAL2 = (-(pwmHighPulse / 2));
			base->SM[submodule].VAL3 = (pwmHighPulse / 2);
		} else {
			base->SM[submodule].VAL4 = (-(pwmHighPulse / 2));
			base->SM[submodule].VAL5 = (pwmHighPulse / 2);
		}
		break;
	case kPWM_CenterAligned:
		/* Setup the PWM period for an unsigned center aligned signal */
		/* Indicates the start of the PWM period */
		base->SM[submodule].INIT = 0;
		/* Indicates the center value */
		base->SM[submodule].VAL0 = (pulseCnt / 2);
		/* Indicates the end of the PWM period */
		base->SM[submodule].VAL1 = pulseCnt;

		/* Setup the PWM dutycycle */
		if (pwm_ch % 2) {
			base->SM[submodule].VAL2 = ((pulseCnt - pwmHighPulse) / 2);
			base->SM[submodule].VAL3 = ((pulseCnt + pwmHighPulse) / 2);
		} else {
			base->SM[submodule].VAL4 = ((pulseCnt - pwmHighPulse) / 2);
			base->SM[submodule].VAL5 = ((pulseCnt + pwmHighPulse) / 2);
		}
		break;
	case kPWM_SignedEdgeAligned:
		/* Setup the PWM period for a signed edge aligned signal */
		modulo = pulseCnt >> 1;
		/* Indicates the start of the PWM period */
		base->SM[submodule].INIT = (-modulo);
		/* Indicates the center value */
		base->SM[submodule].VAL0 = 0;
		/* Indicates the end of the PWM period */
		base->SM[submodule].VAL1 = modulo;

		/* Setup the PWM dutycycle */
		if (pwm_ch % 2) {
			base->SM[submodule].VAL2 = (-modulo);
			base->SM[submodule].VAL3 = (-modulo + pwmHighPulse);
		} else {
			base->SM[submodule].VAL4 = (-modulo);
			base->SM[submodule].VAL5 = (-modulo + pwmHighPulse);
		}
		break;
	case kPWM_EdgeAligned:
		/* Setup the PWM period for a unsigned edge aligned signal */
		/* Indicates the start of the PWM period */
		base->SM[submodule].INIT = 0;
		/* Indicates the center value */
		base->SM[submodule].VAL0 = (pulseCnt / 2);
		/* Indicates the end of the PWM period */
		base->SM[submodule].VAL1 = pulseCnt;

		/* Setup the PWM dutycycle */
		if (pwm_ch % 2) {
			base->SM[submodule].VAL2 = 0;
			base->SM[submodule].VAL3 = pwmHighPulse;
		} else {
			base->SM[submodule].VAL4 = 0;
			base->SM[submodule].VAL5 = pwmHighPulse;
		}
		break;
	default:
		break;
	}
	/* Setup register shift values based on the channel being configured.
	* Also setup the deadtime value
	*/
	if (pwm_ch % 2) {
		polarityShift = PWM_OCTRL_POLA_SHIFT;
		outputEnableShift = PWM_OUTEN_PWMA_EN_SHIFT;
		base->SM[submodule].DTCNT0 = PWM_DTCNT0_DTCNT0(deadtimeValue);
	} else {
		polarityShift = PWM_OCTRL_POLB_SHIFT;
		outputEnableShift = PWM_OUTEN_PWMB_EN_SHIFT;
		base->SM[submodule].DTCNT1 = PWM_DTCNT1_DTCNT1(deadtimeValue);
	}

	/* Setup signal active level */
	if (level == kPWM_HighTrue) {
		base->SM[submodule].OCTRL &= ~(1U << polarityShift);
	} else {
		base->SM[submodule].OCTRL |= (1U << polarityShift);
	}
	/* Enable PWM output */
	base->OUTEN |= (1U << (outputEnableShift + submodule));

	/* Set the load okay bit for all submodules to load registers from their buffer */
	imxrt_pwm_setpwmldok(base, 1U << submodule, true);

	/* Start the PWM generation from Submodules  */
	imxrt_pwm_starttimer(base, 1U << submodule);

	return OK;
}

/****************************************************************************
 * Name: imxrt_pwm_stop
 *
 * Description:
 *   Stop the pulsed output and reset the timer resources
 *
 * Input parameters:
 *   dev - A reference to the lower half PWM driver state structure
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure
 *
 * Assumptions:
 *   This function is called to stop the pulsed output at anytime.  This
 *   method is also called from the timer interrupt handler when a repetition
 *   count expires... automatically stopping the timer.
 *
 ****************************************************************************/
static int imxrt_pwm_stop(FAR struct pwm_lowerhalf_s *dev)
{
	FAR struct imxrt_pwmch_s *priv = (FAR struct imxrt_pwmch_s *)dev;

	PWM_Type *base;
	pwm_submodule_t submodule;
	uint8_t pwm_ch;

	base = priv->base;
	submodule = priv->submodule_id;
	pwm_ch = priv->ch_id;

	if (pwm_ch % 2) {
		base->OUTEN &= ~(1U << (PWM_OUTEN_PWMA_EN_SHIFT + submodule));
	} else {
		base->OUTEN &= ~(1U << (PWM_OUTEN_PWMB_EN_SHIFT + submodule));
	}

	return OK;
}
/****************************************************************************
 * Name: imxrt_pwm_shutdown
 *
 * Description:
 *   This method is called when the driver is closed.  The lower half driver
 *   stop pulsed output, free any resources, disable the timer hardware, and
 *   put the system into the lowest possible power usage state
 *
 * Input parameters:
 *   dev - A reference to the lower half PWM driver state structure
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure
 *
 ****************************************************************************/
static int imxrt_pwm_shutdown(FAR struct pwm_lowerhalf_s *dev)
{
	FAR struct imxrt_pwmch_s *priv = (FAR struct imxrt_pwmch_s *)dev;

	PWM_Type *base;
	pwm_submodule_t submodule;
	uint8_t pwm_ch;

	base = priv->base;
	submodule = priv->submodule_id;
	pwm_ch = priv->ch_id;

	/* Stop the submodule */
	base->MCTRL &= ~(1U << (PWM_MCTRL_RUN_SHIFT + submodule));
	return OK;
}

/****************************************************************************
 * Name: imxrt_pwm_ioctl
 *
 * Description:
 *   Lower-half logic may support platform-specific ioctl commands
 *
 * Input parameters:
 *   dev - A reference to the lower half PWM driver state structure
 *   cmd - The ioctl command
 *   arg - The argument accompanying the ioctl command
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure
 *
 ****************************************************************************/
static int imxrt_pwm_ioctl(FAR struct pwm_lowerhalf_s *dev, int cmd,
						   unsigned long arg)
{
	return OK;
}

/****************************************************************************
 * Name: imxrt_pwm_reset
 *
 * Description:
 *   Reset pwm register value
 *
 * Input parameters:
 *   dev - A reference to the lower half PWM driver state structure
 *   timer - A number identifying the timer use. The number of valid timer
 *     IDs varies but is somewhere in the range of
 *     {0,...,5}.
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/
static void imxrt_pwm_reset(FAR struct pwm_lowerhalf_s *dev, int timer)
{
	imxrt_pwm_stop(dev);
}

/****************************************************************************
 * Private Data
 ****************************************************************************/
static const struct pwm_ops_s g_pwm_ops = {
	.setup = imxrt_pwm_setup,
	.shutdown = imxrt_pwm_shutdown,
	.start = imxrt_pwm_start,
	.stop = imxrt_pwm_stop,
	.ioctl = imxrt_pwm_ioctl,
};

#ifdef CONFIG_IMXRT_PWM1
static struct imxrt_pwmch_s g_ch1dev = {
	.ops = &g_pwm_ops,
	.ch_id = 1,
	.controller_id = 0,
	.submodule_id = kPWM_Module_0,
	.ch_name = kPWM_PwmA,
	.base = (PWM_Type *)PWM1_BASE,
	.submodule_mode = kPWM_Independent,
	.ch_mode = kPWM_SignedCenterAligned,
	.pincfg = 0,
	.freq = 0,
	.duty = 0,
};
static struct imxrt_pwmch_s g_ch2dev = {
	.ops = &g_pwm_ops,
	.ch_id = 2,
	.controller_id = 0,
	.submodule_id = kPWM_Module_0,
	.ch_name = kPWM_PwmB,
	.base = (PWM_Type *)PWM1_BASE,
	.submodule_mode = kPWM_Independent,
	.ch_mode = kPWM_SignedCenterAligned,
	.pincfg = 0,
	.freq = 0,
	.duty = 0,
};
static struct imxrt_pwmch_s g_ch3dev = {
	.ops = &g_pwm_ops,
	.ch_id = 3,
	.controller_id = 0,
	.submodule_id = kPWM_Module_1,
	.ch_name = kPWM_PwmA,
	.base = (PWM_Type *)PWM1_BASE,
	.submodule_mode = kPWM_Independent,
	.ch_mode = kPWM_SignedCenterAligned,
	.pincfg = 0,
	.freq = 0,
	.duty = 0,
};
static struct imxrt_pwmch_s g_ch4dev = {
	.ops = &g_pwm_ops,
	.ch_id = 4,
	.controller_id = 0,
	.submodule_id = kPWM_Module_1,
	.ch_name = kPWM_PwmB,
	.base = (PWM_Type *)PWM1_BASE,
	.submodule_mode = kPWM_Independent,
	.ch_mode = kPWM_SignedCenterAligned,
	.pincfg = 0,
	.freq = 0,
	.duty = 0,
};
static struct imxrt_pwmch_s g_ch5dev = {
	.ops = &g_pwm_ops,
	.ch_id = 5,
	.controller_id = 0,
	.submodule_id = kPWM_Module_2,
	.ch_name = kPWM_PwmA,
	.base = (PWM_Type *)PWM1_BASE,
	.submodule_mode = kPWM_Independent,
	.ch_mode = kPWM_SignedCenterAligned,
	.pincfg = 0,
	.freq = 0,
	.duty = 0,
};
static struct imxrt_pwmch_s g_ch6dev = {
	.ops = &g_pwm_ops,
	.ch_id = 6,
	.controller_id = 0,
	.submodule_id = kPWM_Module_2,
	.ch_name = kPWM_PwmB,
	.base = (PWM_Type *)PWM1_BASE,
	.submodule_mode = kPWM_Independent,
	.ch_mode = kPWM_SignedCenterAligned,
	.pincfg = 0,
	.freq = 0,
	.duty = 0,
};
static struct imxrt_pwmch_s g_ch7dev = {
	.ops = &g_pwm_ops,
	.ch_id = 7,
	.controller_id = 0,
	.submodule_id = kPWM_Module_3,
	.ch_name = kPWM_PwmA,
	.base = (PWM_Type *)PWM1_BASE,
	.submodule_mode = kPWM_Independent,
	.ch_mode = kPWM_SignedCenterAligned,
	.pincfg = 0,
	.freq = 0,
	.duty = 0,
};
static struct imxrt_pwmch_s g_ch8dev = {
	.ops = &g_pwm_ops,
	.ch_id = 8,
	.controller_id = 0,
	.submodule_id = kPWM_Module_3,
	.ch_name = kPWM_PwmB,
	.base = (PWM_Type *)PWM1_BASE,
	.submodule_mode = kPWM_Independent,
	.ch_mode = kPWM_SignedCenterAligned,
	.pincfg = 0,
	.freq = 0,
	.duty = 0,
};
#endif

#ifdef CONFIG_IMXRT_PWM2
static struct imxrt_pwmch_s g_ch9dev = {
	.ops = &g_pwm_ops,
	.ch_id = 9,
	.controller_id = 1,
	.submodule_id = kPWM_Module_0,
	.ch_name = kPWM_PwmA,
	.base = (PWM_Type *)PWM2_BASE,
	.submodule_mode = kPWM_Independent,
	.ch_mode = kPWM_SignedCenterAligned,
	.pincfg = 0,
	.freq = 0,
	.duty = 0,
};
static struct imxrt_pwmch_s g_ch10dev = {
	.ops = &g_pwm_ops,
	.ch_id = 10,
	.controller_id = 1,
	.submodule_id = kPWM_Module_0,
	.ch_name = kPWM_PwmB,
	.base = (PWM_Type *)PWM2_BASE,
	.submodule_mode = kPWM_Independent,
	.ch_mode = kPWM_SignedCenterAligned,
	.pincfg = 0,
	.freq = 0,
	.duty = 0,
};
static struct imxrt_pwmch_s g_ch11dev = {
	.ops = &g_pwm_ops,
	.ch_id = 11,
	.controller_id = 1,
	.submodule_id = kPWM_Module_1,
	.ch_name = kPWM_PwmA,
	.base = (PWM_Type *)PWM2_BASE,
	.submodule_mode = kPWM_Independent,
	.ch_mode = kPWM_SignedCenterAligned,
	.pincfg = 0,
	.freq = 0,
	.duty = 0,
};
static struct imxrt_pwmch_s g_ch12dev = {
	.ops = &g_pwm_ops,
	.ch_id = 12,
	.controller_id = 1,
	.submodule_id = kPWM_Module_1,
	.ch_name = kPWM_PwmB,
	.base = (PWM_Type *)PWM2_BASE,
	.submodule_mode = kPWM_Independent,
	.ch_mode = kPWM_SignedCenterAligned,
	.pincfg = 0,
	.freq = 0,
	.duty = 0,
};
static struct imxrt_pwmch_s g_ch13dev = {
	.ops = &g_pwm_ops,
	.ch_id = 13,
	.controller_id = 1,
	.submodule_id = kPWM_Module_2,
	.ch_name = kPWM_PwmA,
	.base = (PWM_Type *)PWM2_BASE,
	.submodule_mode = kPWM_Independent,
	.ch_mode = kPWM_SignedCenterAligned,
	.pincfg = 0,
	.freq = 0,
	.duty = 0,
};
static struct imxrt_pwmch_s g_ch14dev = {
	.ops = &g_pwm_ops,
	.ch_id = 14,
	.controller_id = 1,
	.submodule_id = kPWM_Module_2,
	.ch_name = kPWM_PwmB,
	.base = (PWM_Type *)PWM2_BASE,
	.submodule_mode = kPWM_Independent,
	.ch_mode = kPWM_SignedCenterAligned,
	.pincfg = 0,
	.freq = 0,
	.duty = 0,
};
static struct imxrt_pwmch_s g_ch15dev = {
	.ops = &g_pwm_ops,
	.ch_id = 15,
	.controller_id = 1,
	.submodule_id = kPWM_Module_3,
	.ch_name = kPWM_PwmA,
	.base = (PWM_Type *)PWM2_BASE,
	.submodule_mode = kPWM_Independent,
	.ch_mode = kPWM_SignedCenterAligned,
	.pincfg = 0,
	.freq = 0,
	.duty = 0,
};
static struct imxrt_pwmch_s g_ch16dev = {
	.ops = &g_pwm_ops,
	.ch_id = 16,
	.controller_id = 1,
	.submodule_id = kPWM_Module_3,
	.ch_name = kPWM_PwmB,
	.base = (PWM_Type *)PWM2_BASE,
	.submodule_mode = kPWM_Independent,
	.ch_mode = kPWM_SignedCenterAligned,
	.pincfg = 0,
	.freq = 0,
	.duty = 0,
};
#endif

#if defined(CONFIG_ARCH_CHIP_FAMILY_IMXRT105x)
#ifdef CONFIG_IMXRT_PWM3
static struct imxrt_pwmch_s g_ch17dev = {
	.ops = &g_pwm_ops,
	.ch_id = 17,
	.controller_id = 2,
	.submodule_id = kPWM_Module_0,
	.ch_name = kPWM_PwmA,
	.base = (PWM_Type *)PWM3_BASE,
	.submodule_mode = kPWM_Independent,
	.ch_mode = kPWM_SignedCenterAligned,
	.pincfg = 0,
	.freq = 0,
	.duty = 0,
};
static struct imxrt_pwmch_s g_ch18dev = {
	.ops = &g_pwm_ops,
	.ch_id = 18,
	.controller_id = 2,
	.submodule_id = kPWM_Module_0,
	.ch_name = kPWM_PwmB,
	.base = (PWM_Type *)PWM3_BASE,
	.submodule_mode = kPWM_Independent,
	.ch_mode = kPWM_SignedCenterAligned,
	.pincfg = 0,
	.freq = 0,
	.duty = 0,
};
static struct imxrt_pwmch_s g_ch19dev = {
	.ops = &g_pwm_ops,
	.ch_id = 19,
	.controller_id = 2,
	.submodule_id = kPWM_Module_1,
	.ch_name = kPWM_PwmA,
	.base = (PWM_Type *)PWM3_BASE,
	.submodule_mode = kPWM_Independent,
	.ch_mode = kPWM_SignedCenterAligned,
	.pincfg = 0,
	.freq = 0,
	.duty = 0,
};
static struct imxrt_pwmch_s g_ch20dev = {
	.ops = &g_pwm_ops,
	.ch_id = 20,
	.controller_id = 2,
	.submodule_id = kPWM_Module_1,
	.ch_name = kPWM_PwmB,
	.base = (PWM_Type *)PWM3_BASE,
	.submodule_mode = kPWM_Independent,
	.ch_mode = kPWM_SignedCenterAligned,
	.pincfg = 0,
	.freq = 0,
	.duty = 0,
};
static struct imxrt_pwmch_s g_ch21dev = {
	.ops = &g_pwm_ops,
	.ch_id = 21,
	.controller_id = 2,
	.submodule_id = kPWM_Module_2,
	.ch_name = kPWM_PwmA,
	.base = (PWM_Type *)PWM3_BASE,
	.submodule_mode = kPWM_Independent,
	.ch_mode = kPWM_SignedCenterAligned,
	.pincfg = 0,
	.freq = 0,
	.duty = 0,
};
static struct imxrt_pwmch_s g_ch22dev = {
	.ops = &g_pwm_ops,
	.ch_id = 22,
	.controller_id = 2,
	.submodule_id = kPWM_Module_2,
	.ch_name = kPWM_PwmB,
	.base = (PWM_Type *)PWM3_BASE,
	.submodule_mode = kPWM_Independent,
	.ch_mode = kPWM_SignedCenterAligned,
	.pincfg = 0,
	.freq = 0,
	.duty = 0,
};
static struct imxrt_pwmch_s g_ch23dev = {
	.ops = &g_pwm_ops,
	.ch_id = 23,
	.controller_id = 2,
	.submodule_id = kPWM_Module_3,
	.ch_name = kPWM_PwmA,
	.base = (PWM_Type *)PWM3_BASE,
	.submodule_mode = kPWM_Independent,
	.ch_mode = kPWM_SignedCenterAligned,
	.pincfg = 0,
	.freq = 0,
	.duty = 0,
};
static struct imxrt_pwmch_s g_ch24dev = {
	.ops = &g_pwm_ops,
	.ch_id = 24,
	.controller_id = 2,
	.submodule_id = kPWM_Module_3,
	.ch_name = kPWM_PwmB,
	.base = (PWM_Type *)PWM3_BASE,
	.submodule_mode = kPWM_Independent,
	.ch_mode = kPWM_SignedCenterAligned,
	.pincfg = 0,
	.freq = 0,
	.duty = 0,
};
#endif

#ifdef CONFIG_IMXRT_PWM4
static struct imxrt_pwmch_s g_ch25dev = {
	.ops = &g_pwm_ops,
	.ch_id = 25,
	.controller_id = 3,
	.submodule_id = kPWM_Module_0,
	.ch_name = kPWM_PwmA,
	.base = (PWM_Type *)PWM4_BASE,
	.submodule_mode = kPWM_Independent,
	.ch_mode = kPWM_SignedCenterAligned,
	.pincfg = 0,
	.freq = 0,
	.duty = 0,
};
static struct imxrt_pwmch_s g_ch26dev = {
	.ops = &g_pwm_ops,
	.ch_id = 26,
	.controller_id = 3,
	.submodule_id = kPWM_Module_0,
	.ch_name = kPWM_PwmB,
	.base = (PWM_Type *)PWM4_BASE,
	.submodule_mode = kPWM_Independent,
	.ch_mode = kPWM_SignedCenterAligned,
	.pincfg = 0,
	.freq = 0,
	.duty = 0,
};
static struct imxrt_pwmch_s g_ch27dev = {
	.ops = &g_pwm_ops,
	.ch_id = 27,
	.controller_id = 3,
	.submodule_id = kPWM_Module_1,
	.ch_name = kPWM_PwmA,
	.base = (PWM_Type *)PWM4_BASE,
	.submodule_mode = kPWM_Independent,
	.ch_mode = kPWM_SignedCenterAligned,
	.pincfg = 0,
	.freq = 0,
	.duty = 0,
};
static struct imxrt_pwmch_s g_ch28dev = {
	.ops = &g_pwm_ops,
	.ch_id = 28,
	.controller_id = 3,
	.submodule_id = kPWM_Module_1,
	.ch_name = kPWM_PwmB,
	.base = (PWM_Type *)PWM4_BASE,
	.submodule_mode = kPWM_Independent,
	.ch_mode = kPWM_SignedCenterAligned,
	.pincfg = 0,
	.freq = 0,
	.duty = 0,
};
static struct imxrt_pwmch_s g_ch29dev = {
	.ops = &g_pwm_ops,
	.ch_id = 29,
	.controller_id = 3,
	.submodule_id = kPWM_Module_2,
	.ch_name = kPWM_PwmA,
	.base = (PWM_Type *)PWM4_BASE,
	.submodule_mode = kPWM_Independent,
	.ch_mode = kPWM_SignedCenterAligned,
	.pincfg = 0,
	.freq = 0,
	.duty = 0,
};
static struct imxrt_pwmch_s g_ch30dev = {
	.ops = &g_pwm_ops,
	.ch_id = 30,
	.controller_id = 3,
	.submodule_id = kPWM_Module_2,
	.ch_name = kPWM_PwmB,
	.base = (PWM_Type *)PWM4_BASE,
	.submodule_mode = kPWM_Independent,
	.ch_mode = kPWM_SignedCenterAligned,
	.pincfg = 0,
	.freq = 0,
	.duty = 0,
};
static struct imxrt_pwmch_s g_ch31dev = {
	.ops = &g_pwm_ops,
	.ch_id = 31,
	.controller_id = 3,
	.submodule_id = kPWM_Module_3,
	.ch_name = kPWM_PwmA,
	.base = (PWM_Type *)PWM4_BASE,
	.submodule_mode = kPWM_Independent,
	.ch_mode = kPWM_SignedCenterAligned,
	.pincfg = 0,
	.freq = 0,
	.duty = 0,
};
static struct imxrt_pwmch_s g_ch32dev = {
	.ops = &g_pwm_ops,
	.ch_id = 32,
	.controller_id = 3,
	.submodule_id = kPWM_Module_3,
	.ch_name = kPWM_PwmB,
	.base = (PWM_Type *)PWM4_BASE,
	.submodule_mode = kPWM_Independent,
	.ch_mode = kPWM_SignedCenterAligned,
	.pincfg = 0,
	.freq = 0,
	.duty = 0,
};
#endif
#endif

/************************************************************************************
 * Name: imxrt_pwm_pins_init
 *
 * Description:
 *   Initialize pin setting in pwm
 *
 ************************************************************************************/
static inline void imxrt_pwm_pins_init(uint8_t channel)
{
	uint8_t pwm_ch = channel;

	switch (pwm_ch) {
#if defined(CONFIG_ARCH_CHIP_FAMILY_IMXRT102x)
	case 1:
		imxrt_iomuxc_setpinmux(IOMUXC_GPIO_AD_B1_06_FLEXPWM1_PWMA00, 0U);
		imxrt_iomuxc_setpinconfig(IOMUXC_GPIO_AD_B1_06_FLEXPWM1_PWMA00, 0x10B0u);
		break;
	case 2:
		imxrt_iomuxc_setpinmux(IOMUXC_GPIO_AD_B1_07_FLEXPWM1_PWMB00, 0U);
		imxrt_iomuxc_setpinconfig(IOMUXC_GPIO_AD_B1_07_FLEXPWM1_PWMB00, 0x10B0u);
		break;
	case 3:
		imxrt_iomuxc_setpinmux(IOMUXC_GPIO_AD_B1_08_FLEXPWM1_PWMA01, 0U);
		imxrt_iomuxc_setpinconfig(IOMUXC_GPIO_AD_B1_08_FLEXPWM1_PWMA01, 0x10B0u);
		break;
	case 4:
		imxrt_iomuxc_setpinmux(IOMUXC_GPIO_AD_B1_09_FLEXPWM1_PWMB01, 0U);
		imxrt_iomuxc_setpinconfig(IOMUXC_GPIO_AD_B1_09_FLEXPWM1_PWMB01, 0x10B0u);
		break;
	case 5:
		imxrt_iomuxc_setpinmux(IOMUXC_GPIO_AD_B1_10_FLEXPWM1_PWMA02, 0U);
		imxrt_iomuxc_setpinconfig(IOMUXC_GPIO_AD_B1_10_FLEXPWM1_PWMA02, 0x10B0u);
		break;
	case 6:
		imxrt_iomuxc_setpinmux(IOMUXC_GPIO_AD_B1_11_FLEXPWM1_PWMB02, 0U);
		imxrt_iomuxc_setpinconfig(IOMUXC_GPIO_AD_B1_11_FLEXPWM1_PWMB02, 0x10B0u);
		break;
	case 7:
		imxrt_iomuxc_setpinmux(IOMUXC_GPIO_AD_B1_12_FLEXPWM1_PWMA03, 0U);
		imxrt_iomuxc_setpinconfig(IOMUXC_GPIO_AD_B1_12_FLEXPWM1_PWMA03, 0x10B0u);
		break;
	case 8:
		imxrt_iomuxc_setpinmux(IOMUXC_GPIO_AD_B1_13_FLEXPWM1_PWMB03, 0U);
		imxrt_iomuxc_setpinconfig(IOMUXC_GPIO_AD_B1_13_FLEXPWM1_PWMB03, 0x10B0u);
		break;
	case 9:
		imxrt_iomuxc_setpinmux(IOMUXC_GPIO_AD_B0_14_FLEXPWM2_PWMA00, 0U);
		imxrt_iomuxc_setpinconfig(IOMUXC_GPIO_AD_B0_14_FLEXPWM2_PWMA00, 0x10B0u);
		break;
	case 10:
		imxrt_iomuxc_setpinmux(IOMUXC_GPIO_AD_B0_15_FLEXPWM2_PWMB00, 0U);
		imxrt_iomuxc_setpinconfig(IOMUXC_GPIO_AD_B0_15_FLEXPWM2_PWMB00, 0x10B0u);
		break;
	case 11:
		imxrt_iomuxc_setpinmux(IOMUXC_GPIO_AD_B0_12_FLEXPWM2_PWMA01, 0U);
		imxrt_iomuxc_setpinconfig(IOMUXC_GPIO_AD_B0_12_FLEXPWM2_PWMA01, 0x10B0u);
		break;
	case 12:
		imxrt_iomuxc_setpinmux(IOMUXC_GPIO_AD_B0_13_FLEXPWM2_PWMB01, 0U);
		imxrt_iomuxc_setpinconfig(IOMUXC_GPIO_AD_B0_13_FLEXPWM2_PWMB01, 0x10B0u);
		break;
	case 13:
		imxrt_iomuxc_setpinmux(IOMUXC_GPIO_AD_B0_10_FLEXPWM2_PWMA02, 0U);
		imxrt_iomuxc_setpinconfig(IOMUXC_GPIO_AD_B0_10_FLEXPWM2_PWMA02, 0x10B0u);
		break;
	case 14:
		imxrt_iomuxc_setpinmux(IOMUXC_GPIO_AD_B0_11_FLEXPWM2_PWMB02, 0U);
		imxrt_iomuxc_setpinconfig(IOMUXC_GPIO_AD_B0_11_FLEXPWM2_PWMB02, 0x10B0u);
		break;
	case 15:
		imxrt_iomuxc_setpinmux(IOMUXC_GPIO_AD_B0_06_FLEXPWM2_PWMA03, 0U);
		imxrt_iomuxc_setpinconfig(IOMUXC_GPIO_AD_B0_06_FLEXPWM2_PWMA03, 0x10B0u);
		break;
	case 16:
		imxrt_iomuxc_setpinmux(IOMUXC_GPIO_AD_B0_07_FLEXPWM2_PWMB03, 0U);
		imxrt_iomuxc_setpinconfig(IOMUXC_GPIO_AD_B0_07_FLEXPWM2_PWMB03, 0x10B0u);
		break;
	default:
		break;
#elif defined(CONFIG_ARCH_CHIP_FAMILY_IMXRT105x)
	case 1:
		imxrt_iomuxc_setpinmux(IOMUXC_GPIO_SD_B0_00_FLEXPWM1_PWMA00, 0U);
		imxrt_iomuxc_setpinconfig(IOMUXC_GPIO_SD_B0_00_FLEXPWM1_PWMA00, 0x10B0u);
		break;
	case 2:
		imxrt_iomuxc_setpinmux(IOMUXC_GPIO_SD_B0_01_FLEXPWM1_PWMB00, 0U);
		imxrt_iomuxc_setpinconfig(IOMUXC_GPIO_SD_B0_01_FLEXPWM1_PWMB00, 0x10B0u);
		break;
	case 3:
		imxrt_iomuxc_setpinmux(IOMUXC_GPIO_SD_B0_02_FLEXPWM1_PWMA01, 0U);
		imxrt_iomuxc_setpinconfig(IOMUXC_GPIO_SD_B0_02_FLEXPWM1_PWMA01, 0x10B0u);
		break;
	case 4:
		imxrt_iomuxc_setpinmux(IOMUXC_GPIO_SD_B0_03_FLEXPWM1_PWMB01, 0U);
		imxrt_iomuxc_setpinconfig(IOMUXC_GPIO_SD_B0_03_FLEXPWM1_PWMB01, 0x10B0u);
		break;
	case 5:
		imxrt_iomuxc_setpinmux(IOMUXC_GPIO_SD_B0_04_FLEXPWM1_PWMA02, 0U);
		imxrt_iomuxc_setpinconfig(IOMUXC_GPIO_SD_B0_04_FLEXPWM1_PWMA02, 0x10B0u);
		break;
	case 6:
		imxrt_iomuxc_setpinmux(IOMUXC_GPIO_SD_B0_05_FLEXPWM1_PWMB02, 0U);
		imxrt_iomuxc_setpinconfig(IOMUXC_GPIO_SD_B0_05_FLEXPWM1_PWMB02, 0x10B0u);
		break;
	case 7:
		imxrt_iomuxc_setpinmux(IOMUXC_GPIO_SD_B1_00_FLEXPWM1_PWMA03, 0U);
		imxrt_iomuxc_setpinconfig(IOMUXC_GPIO_SD_B1_00_FLEXPWM1_PWMA03, 0x10B0u);
		break;
	case 8:
		imxrt_iomuxc_setpinmux(IOMUXC_GPIO_SD_B1_01_FLEXPWM1_PWMB03, 0U);
		imxrt_iomuxc_setpinconfig(IOMUXC_GPIO_SD_B1_01_FLEXPWM1_PWMB03, 0x10B0u);
		break;
	case 9:
		imxrt_iomuxc_setpinmux(IOMUXC_GPIO_B0_06_FLEXPWM2_PWMA00, 0U);
		imxrt_iomuxc_setpinconfig(IOMUXC_GPIO_B0_06_FLEXPWM2_PWMA00, 0x10B0u);
		break;
	case 10:
		imxrt_iomuxc_setpinmux(IOMUXC_GPIO_B0_07_FLEXPWM2_PWMB00, 0U);
		imxrt_iomuxc_setpinconfig(IOMUXC_GPIO_B0_07_FLEXPWM2_PWMB00, 0x10B0u);
		break;
	case 11:
		imxrt_iomuxc_setpinmux(IOMUXC_GPIO_B0_08_FLEXPWM2_PWMA01, 0U);
		imxrt_iomuxc_setpinconfig(IOMUXC_GPIO_B0_08_FLEXPWM2_PWMA01, 0x10B0u);
		break;
	case 12:
		imxrt_iomuxc_setpinmux(IOMUXC_GPIO_B0_09_FLEXPWM2_PWMB01, 0U);
		imxrt_iomuxc_setpinconfig(IOMUXC_GPIO_B0_09_FLEXPWM2_PWMB01, 0x10B0u);
		break;
	case 13:
		imxrt_iomuxc_setpinmux(IOMUXC_GPIO_B0_10_FLEXPWM2_PWMA02, 0U);
		imxrt_iomuxc_setpinconfig(IOMUXC_GPIO_B0_10_FLEXPWM2_PWMA02, 0x10B0u);
		break;
	case 14:
		imxrt_iomuxc_setpinmux(IOMUXC_GPIO_B0_11_FLEXPWM2_PWMB02, 0U);
		imxrt_iomuxc_setpinconfig(IOMUXC_GPIO_B0_11_FLEXPWM2_PWMB02, 0x10B0u);
		break;
	case 15:
		imxrt_iomuxc_setpinmux(IOMUXC_GPIO_B1_02_FLEXPWM2_PWMA03, 0U);
		imxrt_iomuxc_setpinconfig(IOMUXC_GPIO_B1_02_FLEXPWM2_PWMA03, 0x10B0u);
		break;
	case 16:
		imxrt_iomuxc_setpinmux(IOMUXC_GPIO_SD_B1_03_FLEXPWM2_PWMB03, 0U);
		imxrt_iomuxc_setpinconfig(IOMUXC_GPIO_SD_B1_03_FLEXPWM2_PWMB03, 0x10B0u);
		break;
	case 17:
		imxrt_iomuxc_setpinmux(IOMUXC_GPIO_EMC_29_FLEXPWM3_PWMA00, 0U);
		imxrt_iomuxc_setpinconfig(IOMUXC_GPIO_EMC_29_FLEXPWM3_PWMA00, 0x10B0u);
		break;
	case 18:
		imxrt_iomuxc_setpinmux(IOMUXC_GPIO_EMC_30_FLEXPWM3_PWMB00, 0U);
		imxrt_iomuxc_setpinconfig(IOMUXC_GPIO_EMC_30_FLEXPWM3_PWMB00, 0x10B0u);
		break;
	case 19:
		imxrt_iomuxc_setpinmux(IOMUXC_GPIO_EMC_31_FLEXPWM3_PWMA01, 0U);
		imxrt_iomuxc_setpinconfig(IOMUXC_GPIO_EMC_31_FLEXPWM3_PWMA01, 0x10B0u);
		break;
	case 20:
		imxrt_iomuxc_setpinmux(IOMUXC_GPIO_EMC_32_FLEXPWM3_PWMB01, 0U);
		imxrt_iomuxc_setpinconfig(IOMUXC_GPIO_EMC_32_FLEXPWM3_PWMB01, 0x10B0u);
		break;
	case 21:
		imxrt_iomuxc_setpinmux(IOMUXC_GPIO_EMC_33_FLEXPWM3_PWMA02, 0U);
		imxrt_iomuxc_setpinconfig(IOMUXC_GPIO_EMC_33_FLEXPWM3_PWMA02, 0x10B0u);
		break;
	case 22:
		imxrt_iomuxc_setpinmux(IOMUXC_GPIO_EMC_34_FLEXPWM3_PWMB02, 0U);
		imxrt_iomuxc_setpinconfig(IOMUXC_GPIO_EMC_34_FLEXPWM3_PWMB02, 0x10B0u);
		break;
	case 23:
		imxrt_iomuxc_setpinmux(IOMUXC_GPIO_EMC_21_FLEXPWM3_PWMA03, 0U);
		imxrt_iomuxc_setpinconfig(IOMUXC_GPIO_EMC_21_FLEXPWM3_PWMA03, 0x10B0u);
		break;
	case 24:
		imxrt_iomuxc_setpinmux(IOMUXC_GPIO_EMC_22_FLEXPWM3_PWMB03, 0U);
		imxrt_iomuxc_setpinconfig(IOMUXC_GPIO_EMC_22_FLEXPWM3_PWMB03, 0x10B0u);
		break;
	case 25:
		imxrt_iomuxc_setpinmux(IOMUXC_GPIO_AD_B1_08_FLEXPWM4_PWMA00, 0U);
		imxrt_iomuxc_setpinconfig(IOMUXC_GPIO_AD_B1_08_FLEXPWM4_PWMA00, 0x10B0u);
		break;
	case 26:
		imxrt_iomuxc_setpinmux(IOMUXC_GPIO_EMC_01_FLEXPWM4_PWMB00, 0U);
		imxrt_iomuxc_setpinconfig(IOMUXC_GPIO_EMC_01_FLEXPWM4_PWMB00, 0x10B0u);
		break;
	case 27:
		imxrt_iomuxc_setpinmux(IOMUXC_GPIO_AD_B1_09_FLEXPWM4_PWMA01, 0U);
		imxrt_iomuxc_setpinconfig(IOMUXC_GPIO_AD_B1_09_FLEXPWM4_PWMA01, 0x10B0u);
		break;
	case 28:
		imxrt_iomuxc_setpinmux(IOMUXC_GPIO_EMC_03_FLEXPWM4_PWMB01, 0U);
		imxrt_iomuxc_setpinconfig(IOMUXC_GPIO_EMC_03_FLEXPWM4_PWMB01, 0x10B0u);
		break;
	case 29:
		imxrt_iomuxc_setpinmux(IOMUXC_GPIO_B1_14_FLEXPWM4_PWMA02, 0U);
		imxrt_iomuxc_setpinconfig(IOMUXC_GPIO_B1_14_FLEXPWM4_PWMA02, 0x10B0u);
		break;
	case 30:
		imxrt_iomuxc_setpinmux(IOMUXC_GPIO_EMC_05_FLEXPWM4_PWMB02, 0U);
		imxrt_iomuxc_setpinconfig(IOMUXC_GPIO_EMC_05_FLEXPWM4_PWMB02, 0x10B0u);
		break;
	case 31:
		imxrt_iomuxc_setpinmux(IOMUXC_GPIO_B1_15_FLEXPWM4_PWMA03, 0U);
		imxrt_iomuxc_setpinconfig(IOMUXC_GPIO_B1_15_FLEXPWM4_PWMA03, 0x10B0u);
		break;
	case 32:
		imxrt_iomuxc_setpinmux(IOMUXC_GPIO_EMC_18_FLEXPWM4_PWMB03, 0U);
		imxrt_iomuxc_setpinconfig(IOMUXC_GPIO_EMC_18_FLEXPWM4_PWMB03, 0x10B0u);
		break;
	default:
		break;
#endif
	}
}

/************************************************************************************
 * Name: imxrt_pwm_clock_init
 *
 * Description:
 *   Initialize clock setting in flexspi
 *
 ************************************************************************************/
static inline void imxrt_pwm_clock_init(void)
{
	imxrt_clock_setdiv(kCLOCK_AhbDiv, 0x2); /* Set AHB PODF to 2, divide by 3 */
	imxrt_clock_setdiv(kCLOCK_IpgDiv, 0x3); /* Set IPG PODF to 3, divede by 4 */
}

static pwm_clock_prescale_t imxrt_set_sub_freq(FAR struct imxrt_pwmch_s *lower)
{
	uint8_t controller = lower->controller_id;
	uint8_t submodule = lower->submodule_id;

	pwm_clock_prescale_t ret = kPWM_Prescale_Divide_1;

	if (controller == 0) {
		switch (submodule) {
		case kPWM_Module_0:
			ret = CONFIG_PWM1_SUB1_CLOCK;
			break;
		case kPWM_Module_1:
			ret = CONFIG_PWM1_SUB2_CLOCK;
			break;
		case kPWM_Module_2:
			ret = CONFIG_PWM1_SUB3_CLOCK;
			break;
		case kPWM_Module_3:
			ret = CONFIG_PWM1_SUB4_CLOCK;
			break;
		default:
			break;
		}
	} else if (controller == 1) {
		switch (submodule) {
		case kPWM_Module_0:
			ret = CONFIG_PWM2_SUB1_CLOCK;
			break;
		case kPWM_Module_1:
			ret = CONFIG_PWM2_SUB2_CLOCK;
			break;
		case kPWM_Module_2:
			ret = CONFIG_PWM2_SUB3_CLOCK;
			break;
		case kPWM_Module_3:
			ret = CONFIG_PWM2_SUB4_CLOCK;
			break;
		default:
			break;
		}
#if defined(CONFIG_ARCH_CHIP_FAMILY_IMXRT105x)
	} else if (controller == 2) {
		switch (submodule) {
		case kPWM_Module_0:
			ret = CONFIG_PWM3_SUB1_CLOCK;
			break;
		case kPWM_Module_1:
			ret = CONFIG_PWM3_SUB2_CLOCK;
			break;
		case kPWM_Module_2:
			ret = CONFIG_PWM3_SUB3_CLOCK;
			break;
		case kPWM_Module_3:
			ret = CONFIG_PWM3_SUB4_CLOCK;
			break;
		default:
			break;
		}
	} else if (controller == 3) {
		switch (submodule) {
		case kPWM_Module_0:
			ret = CONFIG_PWM3_SUB1_CLOCK;
			break;
		case kPWM_Module_1:
			ret = CONFIG_PWM3_SUB2_CLOCK;
			break;
		case kPWM_Module_2:
			ret = CONFIG_PWM3_SUB3_CLOCK;
			break;
		case kPWM_Module_3:
			ret = CONFIG_PWM3_SUB4_CLOCK;
			break;
		default:
			break;
		}
#endif
	}

	return ret;
}
/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: imxrt_pwminitialize
 *
 * Description:
 *   Initialize one channel for use with the upper_level PWM driver.
 *
 * Input Parameters:
 *   channel - A number identifying the channel use. The number of valid channel
 *   IDs varies with the IMXRT MCU and MCU family but is somewhere in the range of
 *   {1,..,16}. They are organized as 8 pair channels: [1,2],[3,4],[5,6],[7,8],
 *   [9,10],[11,12],[13,14],[15,16], and must work with same frequency for two channels
 *   in one pair. Each pair can work on complementary mode for motor control. However
 *   the two channels in one pair can also work on individual mode, the duty for each
 *   channel can be set individually. (with the same frequency)
 *
 * Returned Value:
 *   On success, a pointer to the IMXRT lower half PWM driver is returned.
 *   NULL is returned on any failure.
 *
 ****************************************************************************/

FAR struct pwm_lowerhalf_s *imxrt_pwminitialize(int channel)
{
	FAR struct imxrt_pwmch_s *lower;
	uint8_t pwm_ch = (uint8_t)channel;
	pwm_config_t pwmconfig;
	PWM_Type *base;
	pwm_submodule_t submodule;

	vdbg("pwm channel %d init\n", channel);
	imxrt_pwm_pins_init(channel);
	imxrt_pwm_getdefaultconfig(&pwmconfig);

	switch (channel) {
#ifdef CONFIG_IMXRT_PWM1
	case 1:
		lower = &g_ch1dev;
		break;
	case 2:
		lower = &g_ch2dev;
		break;
	case 3:
		lower = &g_ch3dev;
		break;
	case 4:
		lower = &g_ch4dev;
		break;
	case 5:
		lower = &g_ch5dev;
		break;
	case 6:
		lower = &g_ch6dev;
		break;
	case 7:
		lower = &g_ch7dev;
		break;
	case 8:
		lower = &g_ch8dev;
		break;
#endif
#ifdef CONFIG_IMXRT_PWM2
	case 9:
		lower = &g_ch9dev;
		break;
	case 10:
		lower = &g_ch10dev;
		break;
	case 11:
		lower = &g_ch11dev;
		break;
	case 12:
		lower = &g_ch12dev;
		break;
	case 13:
		lower = &g_ch13dev;
		break;
	case 14:
		lower = &g_ch14dev;
		break;
	case 15:
		lower = &g_ch15dev;
		break;
	case 16:
		lower = &g_ch16dev;
		break;
#endif
#if defined(CONFIG_ARCH_CHIP_FAMILY_IMXRT105x)
#ifdef CONFIG_IMXRT_PWM3
	case 17:
		lower = &g_ch17dev;
		break;
	case 18:
		lower = &g_ch18dev;
		break;
	case 19:
		lower = &g_ch19dev;
		break;
	case 20:
		lower = &g_ch20dev;
		break;
	case 21:
		lower = &g_ch21dev;
		break;
	case 22:
		lower = &g_ch22dev;
		break;
	case 23:
		lower = &g_ch23dev;
		break;
	case 24:
		lower = &g_ch24dev;
		break;
#endif
#ifdef CONFIG_IMXRT_PWM4
	case 25:
		lower = &g_ch25dev;
		break;
	case 26:
		lower = &g_ch26dev;
		break;
	case 27:
		lower = &g_ch27dev;
		break;
	case 28:
		lower = &g_ch28dev;
		break;
	case 29:
		lower = &g_ch29dev;
		break;
	case 30:
		lower = &g_ch30dev;
		break;
	case 31:
		lower = &g_ch31dev;
		break;
	case 32:
		lower = &g_ch32dev;
		break;
#endif
#endif

	default:
		return NULL;
	}

	base = lower->base;
	submodule = lower->submodule_id;

	pwmconfig.prescale = imxrt_set_sub_freq(lower);

	/* Use full cycle reload */
	pwmconfig.reloadLogic = kPWM_ReloadPwmFullCycle;
	pwmconfig.pairOperation = lower->submodule_mode;
	pwmconfig.enableDebugMode = true;

	if (imxrt_pwm_init(base, submodule, &pwmconfig) == kStatus_Fail) {
		return NULL;
	}
	if (pwm_ch % 2) {
		base->OUTEN &= ~(1U << (PWM_OUTEN_PWMA_EN_SHIFT + submodule));
	} else {
		base->OUTEN &= ~(1U << (PWM_OUTEN_PWMB_EN_SHIFT + submodule));
	}
	imxrt_pwm_setup((FAR struct pwm_lowerhalf_s *)lower);

	return (FAR struct pwm_lowerhalf_s *)lower;
}
