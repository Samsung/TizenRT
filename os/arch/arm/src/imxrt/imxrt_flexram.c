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
 * Copyright 2017 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <tinyara/irq.h>
#include <tinyara/arch.h>
#include "cache.h"
#include "imxrt_clock.h"
#include "imxrt_flexram.h"
#include "imxrt_log.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#if defined(CONFIG_ARCH_CHIP_FAMILY_IMXRT102x)
#define IMXRT_FLEXRAM_OCRAM_START_ADDR 0x20200000
#define IMXRT_FLEXRAM_OCRAM_MAGIC_ADDR 0x202000A0

#define IMXRT_FLEXRAM_DTCM_START_ADDR 0x20000000
#define IMXRT_FLEXRAM_DTCM_MAGIC_ADDR 0x200000A0

#define IMXRT_FLEXRAM_ITCM_START_ADDR 0x0
#define IMXRT_FLEXRAM_ITCM_MAGIC_ADDR 0xA0

/* OCRAM relocate definition */
//#define IMXRT_OCRAM_SIZE (256 * 1024U)
#define IMXRT_OCRAM_ALLOCATE_BANK_NUM 2
#define IMXRT_ITCM_ALLOCATE_BANK_NUM  4
#define IMXRT_DTCM_ALLOCATE_BANK_NUM  2
#elif defined(CONFIG_ARCH_CHIP_FAMILY_IMXRT105x)
#define IMXRT_FLEXRAM_OCRAM_START_ADDR 0x20200000
#define IMXRT_FLEXRAM_OCRAM_MAGIC_ADDR 0x202000A0

#define IMXRT_FLEXRAM_DTCM_START_ADDR 0x20000000
#define IMXRT_FLEXRAM_DTCM_MAGIC_ADDR 0x200000A0

#define IMXRT_FLEXRAM_ITCM_START_ADDR 0x0
#define IMXRT_FLEXRAM_ITCM_MAGIC_ADDR 0xA0

/* OCRAM relocate definition */
//#define IMXRT_OCRAM_SIZE (512 * 1024U)
#define IMXRT_OCRAM_ALLOCATE_BANK_NUM 16
#define IMXRT_ITCM_ALLOCATE_BANK_NUM  0
#define IMXRT_DTCM_ALLOCATE_BANK_NUM  0
#else
#error Unrecognized i.MX RT architecture
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Gets the instance from the base address to be used to gate or ungate the module clock
 *
 * @param base FLEXRAM base address
 *
 * @return The FLEXRAM instance
 */
static uint32_t imxrt_flexram_getinstance(FLEXRAM_Type *base);

/*!
 * @brief FLEXRAM map TCM size to register value
 *
 * @param tcmBankNum tcm banknumber
 * @retval register value correspond to the tcm size
  */
static uint8_t imxrt_flexram_maptcmsizetoregister(uint8_t tcmBankNum);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*! @brief Pointers to FLEXRAM bases for each instance. */
static FLEXRAM_Type *const s_flexramBases[] = FLEXRAM_BASE_PTRS;

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
/*! @brief Pointers to FLEXRAM clocks for each instance. */
static const clock_ip_name_t s_flexramClocks[] = FLEXRAM_CLOCKS;
#endif	/* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

/*******************************************************************************
 * Code
 ******************************************************************************/
/****************************************************************************
 * Function: imxrt_flexram_getinstance
 *
 * Description:
 *   brief get instance for flexram
 *
 * Input Parameters:
 *   base FLEXRAM base address.
 *
 * Returned Value:
 *   Returns the intance
 *
 ****************************************************************************/
static uint32_t imxrt_flexram_getinstance(FLEXRAM_Type *base)
{
	uint32_t instance;

	/* Find the instance index from base address mappings. */
	for (instance = 0; instance < ARRAY_SIZE(s_flexramBases); instance++) {
		if (s_flexramBases[instance] == base) {
			break;
		}
	}

	assert(instance < ARRAY_SIZE(s_flexramBases));

	return instance;
}

/****************************************************************************
 * Function: imxrt_flexram_init
 *
 * Description:
 *   brief FLEXRAM module initialization function.
 *
 * Input Parameters:
 *   base FLEXRAM base address.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_flexram_init(FLEXRAM_Type *base)
{
#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
	/* Ungate ENET clock. */
	imxrt_clock_enableclock(s_flexramClocks[imxrt_flexram_getinstance(base)]);
#endif	/* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

	/* enable all the interrupt status */
	base->INT_STAT_EN |= kFLEXRAM_InterruptStatusAll;

	/* clear all the interrupt status */
	base->INT_STATUS |= kFLEXRAM_InterruptStatusAll;

	/* disable all the interrpt */
	base->INT_SIG_EN = 0U;
}

/****************************************************************************
 * Function: imxrt_flexram_deinit
 *
 * Description:
 *   brief Deinitializes the FLEXRAM.
 *
 * Input Parameters:
 *   base FLEXRAM base address.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_flexram_deinit(FLEXRAM_Type *base)
{
#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
	/* Ungate ENET clock. */
	imxrt_clock_disableclock(s_flexramClocks[imxrt_flexram_getinstance(base)]);
#endif	/* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
}

/****************************************************************************
 * Function: imxrt_flexram_maptcmsizetoregister
 *
 * Description:
 *   brief map tcm size to register
 *
 * Input Parameters:
 *   tcmBankNum tcm Bank Number.
 *
 * Returned Value:
 *   Return tcmConfig
 *
 ****************************************************************************/
static uint8_t imxrt_flexram_maptcmsizetoregister(uint8_t tcmBankNum)
{
	uint8_t tcmSizeConfig = 0U;
	uint32_t totalTcmSize = 0U;

	/* if bank number is a odd value, use a new bank number which bigger than target */
	do {
		if ((tcmBankNum & (tcmBankNum - 1U)) == 0U) {
			break;
		}
	} while (++tcmBankNum < FSL_FEATURE_FLEXRAM_INTERNAL_RAM_TOTAL_BANK_NUMBERS);

	totalTcmSize = tcmBankNum * (FSL_FEATURE_FLEXRAM_INTERNAL_RAM_BANK_SIZE >> 10U);

	/* get bit '1' position */
	while (totalTcmSize) {
		if ((totalTcmSize & 1U) == 0U) {
			tcmSizeConfig++;
		} else {
			break;
		}
		totalTcmSize >>= 1U;
	}

	return tcmSizeConfig + 1U;
}

/****************************************************************************
 * Function: imxrt_flexram_settcmsize
 *
 * Description:
 *   brief map tcm size to register
 *
 * Input Parameters:
 *   itcmBankNum itcm Bank Number.
 *   dtcmBankNum dtcm Bank Number
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_flexram_settcmsize(uint8_t itcmBankNum, uint8_t dtcmBankNum)
{
	char tempLog[128];
	uint8_t size = 0;

	assert(itcmBankNum <= FSL_FEATURE_FLEXRAM_INTERNAL_RAM_TOTAL_BANK_NUMBERS);
	assert(dtcmBankNum <= FSL_FEATURE_FLEXRAM_INTERNAL_RAM_TOTAL_BANK_NUMBERS);

	IMXLOG("imxrt_flexram_settcmsize:");
	snprintf(tempLog, sizeof(tempLog), "itcmBankNum %d", itcmBankNum);
	IMXLOG(tempLog);
	snprintf(tempLog, sizeof(tempLog), "dtcmBankNum %d", dtcmBankNum);
	IMXLOG(tempLog);

	size = imxrt_flexram_maptcmsizetoregister(dtcmBankNum);
	snprintf(tempLog, sizeof(tempLog), "dtcm size %d", size);
	IMXLOG(tempLog);

	/* dtcm configuration */
	if (dtcmBankNum != 0U) {
		IOMUXC_GPR->GPR14 &= ~IOMUXC_GPR_GPR14_CM7_CFGDTCMSZ_MASK;
		IOMUXC_GPR->GPR14 |= IOMUXC_GPR_GPR14_CM7_CFGDTCMSZ(imxrt_flexram_maptcmsizetoregister(dtcmBankNum));
		IOMUXC_GPR->GPR16 |= IOMUXC_GPR_GPR16_INIT_DTCM_EN_MASK;
	} else {
		IOMUXC_GPR->GPR16 &= ~IOMUXC_GPR_GPR16_INIT_DTCM_EN_MASK;
	}

	size = imxrt_flexram_maptcmsizetoregister(itcmBankNum);
	snprintf(tempLog, sizeof(tempLog), "itcm size %d", size);
	IMXLOG(tempLog);

	/* itcm configuration */
	if (itcmBankNum != 0U) {
		IOMUXC_GPR->GPR14 &= ~IOMUXC_GPR_GPR14_CM7_CFGITCMSZ_MASK;
		IOMUXC_GPR->GPR14 |= IOMUXC_GPR_GPR14_CM7_CFGITCMSZ(imxrt_flexram_maptcmsizetoregister(itcmBankNum));
		IOMUXC_GPR->GPR16 |= IOMUXC_GPR_GPR16_INIT_ITCM_EN_MASK;
	} else {
		//IOMUXC_GPR->GPR16 &= ~IOMUXC_GPR_GPR16_INIT_ITCM_EN_MASK;
	}
}

/****************************************************************************
 * Function: imxrt_flexram_allocateram
 *
 * Description:
 *   LEXRAM allocate on-chip ram for OCRAM,ITCM,DTCM
 *   This function is independent of FLEXRAM_Init, it can be called directly if ram re-allocate
 *   is needed.
 *
 * Input Parameters:
 *   config allocate configuration.
 *
 * Returned Value:
 *   Returns kStatus_InvalidArgument the argument is invalid
 *   Returns kStatus_Success allocate success
 *
 ****************************************************************************/
status_t imxrt_flexram_allocateram(flexram_allocate_ram_t *config)
{
	assert(config != NULL);

	uint8_t dtcmBankNum = config->dtcmBankNum;
	uint8_t itcmBankNum = config->itcmBankNum;
	uint8_t ocramBankNum = config->ocramBankNum;
	uint32_t bankCfg = 0U, i = 0U;

	/* check the arguments */
	if (FSL_FEATURE_FLEXRAM_INTERNAL_RAM_TOTAL_BANK_NUMBERS < (dtcmBankNum + itcmBankNum + ocramBankNum)) {
		return kStatus_InvalidArgument;
	}

	/* flexram bank config value */
	for (i = 0U; i < FSL_FEATURE_FLEXRAM_INTERNAL_RAM_TOTAL_BANK_NUMBERS; i++) {
		if (i < ocramBankNum) {
			bankCfg |= ((uint32_t)kFLEXRAM_BankOCRAM) << (i * 2);
			continue;
		}

		if (i < (dtcmBankNum + ocramBankNum)) {
			bankCfg |= ((uint32_t)kFLEXRAM_BankDTCM) << (i * 2);
			continue;
		}

		if (i < (dtcmBankNum + ocramBankNum + itcmBankNum)) {
			bankCfg |= ((uint32_t)kFLEXRAM_BankITCM) << (i * 2);
			continue;
		}
	}

	IOMUXC_GPR->GPR17 = bankCfg;

	/* set TCM size */
	imxrt_flexram_settcmsize(itcmBankNum, dtcmBankNum);

	/* select ram allocate source from FLEXRAM_BANK_CFG */
	imxrt_flexram_setallocateramsrc(kFLEXRAM_BankAllocateThroughBankCfg);

	return kStatus_Success;
}

/****************************************************************************
 * Function: imxrt_ocram_reallocate
 *
 * Description:
 *   LEXRAM allocate on-chip ram for OCRAM,ITCM,DTCM
 *   This function is independent of FLEXRAM_Init, it can be called directly if ram re-allocate
 *   is needed.
 *
 * Input Parameters:
 *   config allocate configuration.
 *
 * Returned Value:
 *   Returns kStatus_InvalidArgument the argument is invalid
 *   Returns kStatus_Success allocate success
 *
 ****************************************************************************/
static status_t imxrt_ocram_reallocate(void)
{
	char tempLog[128];

	flexram_allocate_ram_t ramAllocate = {
		.ocramBankNum = IMXRT_OCRAM_ALLOCATE_BANK_NUM,
		.dtcmBankNum = IMXRT_ITCM_ALLOCATE_BANK_NUM,
		.itcmBankNum = IMXRT_DTCM_ALLOCATE_BANK_NUM,
	};

	IMXLOG("Allocate on-chip ram:");
	snprintf(tempLog, sizeof(tempLog), "OCRAM bank numbers %d", ramAllocate.ocramBankNum);
	IMXLOG(tempLog);
	snprintf(tempLog, sizeof(tempLog), "DTCM  bank numbers %d", ramAllocate.dtcmBankNum);
	IMXLOG(tempLog);
	snprintf(tempLog, sizeof(tempLog), "ITCM  bank numbers %d", ramAllocate.itcmBankNum);
	IMXLOG(tempLog);

	if (imxrt_flexram_allocateram(&ramAllocate) != kStatus_Success) {
		IMXLOG("Allocate on-chip ram fail");
		return kStatus_Fail;
	} else {
		IMXLOG("Allocate on-chip ram success");
	}

	return kStatus_Success;
}

/****************************************************************************
 * Function: imxrt_flexram_initialize
 *
 * Description:
 *   FlexRam Initialize
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_flexram_initialize(void)
{
	arch_disable_dcache();

	up_enable_irq(IMXRT_IRQ_CM7FR);

	imxrt_ocram_reallocate();

	imxrt_flexram_init(FLEXRAM);
}
