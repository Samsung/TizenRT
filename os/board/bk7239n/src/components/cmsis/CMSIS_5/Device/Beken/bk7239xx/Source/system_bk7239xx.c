/*
 * Copyright (c) 2009-2018 Arm Limited.
 * Copyright (c) 2018-2020 ArmChina.
 * All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "system_star.h"
#include "armstar.h"
#if defined (__ARM_FEATURE_CMSE) &&  (__ARM_FEATURE_CMSE == 3U)
  #include "partition_star.h"
#endif
#include <soc/bk7239n/soc.h>
#include "os/mem.h"

/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/
#define  SYSTEM_CLOCK    CONFIG_XTAL_FREQ

/*----------------------------------------------------------------------------
  Exception / Interrupt Vector table
 *----------------------------------------------------------------------------*/
//extern const VECTOR_TABLE_Type __VECTOR_TABLE[];

extern const VECTOR_TABLE_Type __VECTOR_TABLE_IRAM[];



/*----------------------------------------------------------------------------
  System initialization function
 *----------------------------------------------------------------------------*/
__FLASH_BOOT_CODE void SystemInit (void)
{
#if defined (__VTOR_PRESENT) && (__VTOR_PRESENT == 1U)
	SCB->VTOR = (uint32_t) &(__VECTOR_TABLE_IRAM[0]);
#endif

#ifdef UNALIGNED_SUPPORT_DISABLE
	SCB->CCR |= SCB_CCR_UNALIGN_TRP_Msk;
#endif

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
#if defined(CONFIG_SPE) && (CONFIG_SPE == 1)
	//TZ_SAU_Setup();
#endif
#endif

#if defined(CONFIG_SPE) && (CONFIG_SPE == 1)
    /* secureFault enable*/
	SCB->SHCSR |= SCB_SHCSR_SECUREFAULTENA_Msk;
#endif

	/* enable system fault exception*/
	SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk;
	SCB->SHCSR |= SCB_SHCSR_USGFAULTENA_Msk;
	SCB->SHCSR |= SCB_SHCSR_BUSFAULTENA_Msk;

	/* enable div_0_trp*/
	SCB->CCR |= SCB_CCR_DIV_0_TRP_Msk;

	/* disable unalign_trp */
	SCB->CCR &= (~SCB_CCR_UNALIGN_TRP_Msk);

#if (defined(CONFIG_ICACHE))
	if (SCB->CLIDR & SCB_CLIDR_IC_Msk)
		SCB_EnableICache();
#endif
}

__IRAM_SEC void SystemInit_iram (void)
{
#if defined (__VTOR_PRESENT) && (__VTOR_PRESENT == 1U)
	SCB->VTOR = (uint32_t) &(__VECTOR_TABLE_IRAM[0]);
#endif

#ifdef UNALIGNED_SUPPORT_DISABLE
	SCB->CCR |= SCB_CCR_UNALIGN_TRP_Msk;
#endif

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
#if defined(CONFIG_SPE) && (CONFIG_SPE == 1)
	//TZ_SAU_Setup();
#endif
#endif

#if defined(CONFIG_SPE) && (CONFIG_SPE == 1)
    /* secureFault enable*/
	SCB->SHCSR |= SCB_SHCSR_SECUREFAULTENA_Msk;
#endif

	/* enable system fault exception*/
	SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk;
	SCB->SHCSR |= SCB_SHCSR_USGFAULTENA_Msk;
	SCB->SHCSR |= SCB_SHCSR_BUSFAULTENA_Msk;

	/* enable div_0_trp*/
	SCB->CCR |= SCB_CCR_DIV_0_TRP_Msk;

	/* disable unalign_trp */
	SCB->CCR &= (~SCB_CCR_UNALIGN_TRP_Msk);

#if (defined(CONFIG_ICACHE))
	if (SCB->CLIDR & SCB_CLIDR_IC_Msk)
		SCB_EnableICache();
#endif
}

// eof

