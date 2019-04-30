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

#ifndef _IMXRT_FLEXRAM_H_
#define _IMXRT_FLEXRAM_H_

#include <stdint.h>
#include <tinyara/config.h>

#if defined(CONFIG_ARCH_CHIP_FAMILY_IMXRT102x)
#include <chip/imxrt102x_features.h>
#include "chip/imxrt102x_config.h"
#elif defined(CONFIG_ARCH_CHIP_FAMILY_IMXRT105x)
#include <chip/imxrt105x_features.h>
#include "chip/imxrt105x_config.h"
#else
#error Unrecognized i.MX RT architecture
#endif

#include "imxrt_config.h"

/*!
 * @addtogroup flexram
 * @{
 */

/******************************************************************************
 * Definitions.
 *****************************************************************************/
/*! @brief flexram write read sel */
enum _flexram_wr_rd_sel {
	kFLEXRAM_Read  = 0U, /*!< read */
	kFLEXRAM_Write = 1U, /*!< write */
};

/*! @brief Interrupt status flag mask */
enum _flexram_interrupt_status {
	kFLEXRAM_OCRAMAccessError = FLEXRAM_INT_STATUS_OCRAM_ERR_STATUS_MASK, /*!< ocram access unallocated address */
	kFLEXRAM_DTCMAccessError  = FLEXRAM_INT_STATUS_DTCM_ERR_STATUS_MASK,  /*!< dtcm access unallocated address */
	kFLEXRAM_ITCMAccessError  = FLEXRAM_INT_STATUS_ITCM_ERR_STATUS_MASK,  /*!< itcm access unallocated address */

	kFLEXRAM_InterruptStatusAll = FLEXRAM_INT_STATUS_OCRAM_ERR_STATUS_MASK | FLEXRAM_INT_STATUS_DTCM_ERR_STATUS_MASK |
								  FLEXRAM_INT_STATUS_ITCM_ERR_STATUS_MASK, /*!< all the interrupt status mask */
};

/*! @brief FLEXRAM TCM access mode
* Fast access mode expected to be finished in 1-cycle
* Wait access mode expected to be finished in 2-cycle
* Wait access mode is a feature of the flexram and it should be used when
* the cpu clock too fast to finish tcm access in 1-cycle.
* Normally, fast mode is the default mode, the efficiency of the tcm access will better.
*/
typedef enum _flexram_tcm_access_mode {
	kFLEXRAM_TCMAccessFastMode = 0U, /*!< fast access mode */
	kFLEXRAM_TCMAccessWaitMode = 1U, /*!< wait access mode */
} flexram_tcm_access_mode_t;

/*! @brief FLEXRAM bank type */
enum _flexram_bank_type {
	kFLEXRAM_BankNotUsed = 0U, /*!< bank is not used */
	kFLEXRAM_BankOCRAM   = 1U, /*!< bank is OCRAM */
	kFLEXRAM_BankDTCM    = 2U, /*!< bank is DTCM */
	kFLEXRAM_BankITCM    = 3U, /*!< bank is ITCM */
};

/*! @brief FLEXRAM tcm support size */
enum _flexram_tcm_size {
	kFLEXRAM_TCMSize32KB  =  32 * 1024U, /*!< TCM total size 32KB */
	kFLEXRAM_TCMSize64KB  =  64 * 1024U, /*!< TCM total size 64KB */
	kFLEXRAM_TCMSize128KB = 128 * 1024U, /*!< TCM total size 128KB */
	kFLEXRAM_TCMSize256KB = 256 * 1024U, /*!< TCM total size 256KB */
	kFLEXRAM_TCMSize512KB = 512 * 1024U, /*!< TCM total size 512KB */
};

/*! @brief FLEXRAM bank allocate source */
typedef enum _flexram_bank_allocate_src {
	kFLEXRAM_BankAllocateThroughHardwareFuse = 0U, /*!< allocate ram through hardware fuse value */
	kFLEXRAM_BankAllocateThroughBankCfg      = 1U, /*!< allocate ram through FLEXRAM_BANK_CFG */
} flexram_bank_allocate_src_t;

/*! @brief FLEXRAM allocate ocram, itcm, dtcm size */
typedef struct _flexram_allocate_ram {
	const uint8_t ocramBankNum;	/*!< ocram banknumber which the SOC support */
	const uint8_t dtcmBankNum;	/*!< dtcm bank number to allocate, the number should be power of 2 */
	const uint8_t itcmBankNum;	/*!< itcm bank number to allocate, the number should be power of 2 */
} flexram_allocate_ram_t;

/*******************************************************************************
 * APIs
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name Initialization and deinitialization
 * @{
 */

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
void imxrt_flexram_init(FLEXRAM_Type *base);

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
void imxrt_flexram_deinit(FLEXRAM_Type *base);

/* @} */

/*!
 * @name Status
 * @{
 */
/****************************************************************************
 * Function: imxrt_flexram_getinterruptstatus
 *
 * Description:
 *   brief FLEXRAM module get interrupt status.
 *
 * Input Parameters:
 *   base FLEXRAM base address.
 *
 * Returned Value:
 *   Return the interrupt status
 *
 ****************************************************************************/
static inline uint32_t imxrt_flexram_getinterruptstatus(FLEXRAM_Type *base)
{
	return base->INT_STATUS & kFLEXRAM_InterruptStatusAll;
}

/****************************************************************************
 * Function: imxrt_flexram_clearinterruptstatus
 *
 * Description:
 *   brief FLEXRAM module clear interrupt status.
 *
 * Input Parameters:
 *   base FLEXRAM base address.
 *   status status to clear.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static inline void imxrt_flexram_clearinterruptstatus(FLEXRAM_Type *base, uint32_t status)
{
	base->INT_STATUS |= status;
}

/****************************************************************************
 * Function: imxrt_flexram_enableinterruptstatus
 *
 * Description:
 *   brief FLEXRAM module enable interrupt status.
 *
 * Input Parameters:
 *   base FLEXRAM base address.
 *   status status to enable.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static inline void imxrt_flexram_enableinterruptstatus(FLEXRAM_Type *base, uint32_t status)
{
	base->INT_STAT_EN |= status;
}

/****************************************************************************
 * Function: imxrt_flexram_disableinterruptstatus
 *
 * Description:
 *   FLEXRAM module disable interrupt status.
 *
 * Input Parameters:
 *   base FLEXRAM base address.
 *   status status to disable.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static inline void imxrt_flexram_disableinterruptstatus(FLEXRAM_Type *base, uint32_t status)
{
	base->INT_STAT_EN &= ~status;
}

/* @} */

/*!
 * @name Interrupts
 * @{
 */

/****************************************************************************
 * Function: imxrt_flexram_enableinterruptsignal
 *
 * Description:
 *   FLEXRAM module enable interrupt.
 *
 * Input Parameters:
 *   base FLEXRAM base address.
 *   status status interrupt to enable.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static inline void imxrt_flexram_enableinterruptsignal(FLEXRAM_Type *base, uint32_t status)
{
	base->INT_SIG_EN |= status;
}

/****************************************************************************
 * Function: imxrt_flexram_disableinterruptsignal
 *
 * Description:
 *   FLEXRAM module disable interrupt.
 *
 * Input Parameters:
 *   base FLEXRAM base address.
 *   status status interrupt to disable.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static inline void imxrt_flexram_disableinterruptsignal(FLEXRAM_Type *base, uint32_t status)
{
	base->INT_SIG_EN &= ~status;
}
/* @} */

/*!
 * @name functional
 * @{
 */

/****************************************************************************
 * Function: imxrt_flexram_settcmreadaccessmode
 *
 * Description:
 *   FLEXRAM module set TCM read access mode
 *
 * Input Parameters:
 *   base FLEXRAM base address.
 *   mode  access mode.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static inline void imxrt_flexram_settcmreadaccessmode(FLEXRAM_Type *base, flexram_tcm_access_mode_t mode)
{
	base->TCM_CTRL &= ~FLEXRAM_TCM_CTRL_TCM_RWAIT_EN_MASK;
	base->TCM_CTRL |= mode;
}

/****************************************************************************
 * Function: imxrt_flexram_settcmwriteaccessmode
 *
 * Description:
 *   FLEXRAM module set TCM write access mode
 *
 * Input Parameters:
 *   base FLEXRAM base address.
 *   mode  access mode.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static inline void imxrt_flexram_settcmwriteaccessmode(FLEXRAM_Type *base, flexram_tcm_access_mode_t mode)
{
	base->TCM_CTRL &= ~FLEXRAM_TCM_CTRL_TCM_WWAIT_EN_MASK;
	base->TCM_CTRL |= mode;
}

/****************************************************************************
 * Function: imxrt_flexram_enableforceramclockon
 *
 * Description:
 *   FLEXRAM module force ram clock on
 *
 * Input Parameters:
 *   base FLEXRAM base address.
 *   enable enable or disable clock force on.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static inline void imxrt_flexram_enableforceramclockon(FLEXRAM_Type *base, bool enable)
{
	if (enable) {
		base->TCM_CTRL |= FLEXRAM_TCM_CTRL_FORCE_CLK_ON_MASK;
	} else {
		base->TCM_CTRL &= ~FLEXRAM_TCM_CTRL_FORCE_CLK_ON_MASK;
	}
}

/****************************************************************************
 * Function: imxrt_flexram_allocateram
 *
 * Description:
 *   brief FLEXRAM allocate on-chip ram for OCRAM,ITCM,DTCM
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
status_t imxrt_flexram_allocateram(flexram_allocate_ram_t *config);

/****************************************************************************
 * Function: imxrt_flexram_setallocateramsrc
 *
 * Description:
 *   brief FLEXRAM set allocate on-chip ram source
 *
 * Input Parameters:
 *   src bank config source select value.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static inline void imxrt_flexram_setallocateramsrc(flexram_bank_allocate_src_t src)
{
	IOMUXC_GPR->GPR16 &= ~IOMUXC_GPR_GPR16_FLEXRAM_BANK_CFG_SEL_MASK;
	IOMUXC_GPR->GPR16 |= IOMUXC_GPR_GPR16_FLEXRAM_BANK_CFG_SEL(src);
}

/****************************************************************************
 * Function: imxrt_flexram_settcmsize
 *
 * Description:
 *   brief FLEXRAM configure TCM size
 *   This function  is used to set the TCM to the target size. If a odd bank number is used,
 *   a new banknumber will be used which is bigger than target value, application can set tcm
 *   size to the biggest bank number always, then boundary access error can be captured by flexram only.
 *   When access to the TCM memory boundary ,hardfault will raised by core.
 *
 * Input Parameters:
 *   itcmBankNum itcm bank number to allocate
 *   dtcmBankNum dtcm bank number to allocate
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_flexram_settcmsize(uint8_t itcmBankNum, uint8_t dtcmBankNum);

/****************************************************************************
 * Function: imxrt_flexram_initialize
 *
 * Description:
 *   brief FLEXRAM initialize
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_flexram_initialize(void);

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif
