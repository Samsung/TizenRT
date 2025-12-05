// Copyright 2022-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "platform_irq.h"
#include "STAR_SE.h"
#include "core_star.h"
#include "sdkconfig.h"
#include "mpu.h"
#include "region.h"
#include "cmsis.h"

#define  SYSTEM_CLOCK     CONFIG_XTAL_FREQ
#define  PERIPHERAL_CLOCK CONFIG_XTAL_FREQ

#define BK7236_PMU_BASE_ADDR         0x44000000
#define DEEP_SLEEP_RESTART_BIT		(0x1 << 1)
#define BK7236_PMU_RESET_REASON_GET (*((volatile unsigned int *)(BK7236_PMU_BASE_ADDR + 0x0*4)))
#define RUNNING_IS_FROM_DEEP_SLEEP      ((BK7236_PMU_RESET_REASON_GET & DEEP_SLEEP_RESTART_BIT ) != 0)

#define EFUSE_REG2_MODULE        *((volatile unsigned long *) (0x44880000 + 0x2*4))
#define EFUSE_REG4_CTRL          *((volatile unsigned long *) (0x44880000 + 0x4*4))
#define EFUSE_REG5_DATA          *((volatile unsigned long *) (0x44880000 + 0x5*4))
#define EFUSE_READ_IS_VALID       ((EFUSE_REG5_DATA & 0x100) >> 8)
#define EFUSE_FAST_BOOT_DISABLE_BIT         (1<<2)

#if defined (__VTOR_PRESENT) && (__VTOR_PRESENT == 1U)
#if CONFIG_BL2_DOWNLOAD
  extern const VECTOR_TABLE_Type __VECTOR_IRAM[];
//   extern uint32_t __Vectors_Iram;
#else
  extern const VECTOR_TABLE_Type __VECTOR_TABLE[];
#endif
#endif

uint32_t SystemCoreClock = SYSTEM_CLOCK;
uint32_t PeripheralClock = PERIPHERAL_CLOCK;

/* System Core Clock update function */
void SystemCoreClockUpdate (void)
{
  SystemCoreClock = SYSTEM_CLOCK;
}

uint32_t sys_is_enable_fast_boot(void)
{
    // volatile uint32_t loop_cnt = 0;   
    volatile uint32_t loop_cnt = 1020000;  //optimize code size
    uint32_t efuse_data;

    EFUSE_REG2_MODULE = 0x03;
    EFUSE_REG4_CTRL = 0x01;

    while((EFUSE_READ_IS_VALID == 0)) {  //optimize code size
        /* about 600ms*/
        if (loop_cnt -- == 0) {
            return 0;
        }
    }

    efuse_data = EFUSE_REG5_DATA;

    return (!(efuse_data & EFUSE_FAST_BOOT_DISABLE_BIT));
}

uint32_t sys_is_running_from_deep_sleep(void)
{
    return (RUNNING_IS_FROM_DEEP_SLEEP);
}

/* System initialization function */
void SystemInit (void)
{
#if __DOMAIN_NS != 1U
#if defined (__VTOR_PRESENT) && (__VTOR_PRESENT == 1U)
#if CONFIG_BL2_DOWNLOAD
	SCB->VTOR = (uint32_t) &__VECTOR_IRAM[0];
	// SCB->VTOR = (uint32_t) &__Vectors_Iram;
#else
	SCB->VTOR = (uint32_t) &__VECTOR_TABLE[0];
#endif
#endif

#ifdef UNALIGNED_SUPPORT_DISABLE
	SCB->CCR |= SCB_CCR_UNALIGN_TRP_Msk;
#endif
#endif /* __DOMAIN_NS != 1U */

#if defined __EN_ICACHE
	if (SCB->CLIDR & SCB_CLIDR_IC_Msk){
		SCB_EnableICache();
	}
#endif

#if defined __EN_DCACHE
	if (SCB->CLIDR & SCB_CLIDR_DC_Msk){
		/* Clearing platform cache state. Maybe a system/sdk has several projects,
		* the previous setting causes the remain items at the cache space.
		*/
		SCB_CleanInvalidateDCache();
		SCB_DisableDCache();
#if CONFIG_ENABLE_TEST_NS
		mpu_disable();
#else
		/* Disable mpu, and enable mpu based on mpu regions*/
		mpu_enable();
		SCB_EnableDCache();
#endif
	}
#endif

	SystemCoreClock = SYSTEM_CLOCK;
	PeripheralClock = PERIPHERAL_CLOCK;

#if CONFIG_SPE
	/*Only spe can configure tcm permission*/
#if defined (__ITCM_PRESENT) && (__ITCM_PRESENT == 1U)
	TCM->ITCMCR |= SCB_ITCMCR_EN_Msk;
	ITGU->TGU_CTRL |= TGU_DBFEN_Msk | TGU_DEREN_Msk;
#endif

#if defined (__DTCM_PRESENT) && (__DTCM_PRESENT == 1U)
	TCM->DTCMCR |= SCB_DTCMCR_EN_Msk;
#endif
#endif

#if defined(__FPU_USED) && (__FPU_USED == 1U)
        /* Enable Secure privileged and unprivilged access to the FP Extension */
        SCB->CPACR |= (3U << 10U*2U)     /* enable CP10 full access */
                      | (3U << 11U*2U);  /* enable CP11 full access */

#if defined(__ARM_ARCH_8_1M_MAIN__) || defined(__ARM_ARCH_8M_MAIN__)
        /* If the SPE will ever use the floating-point registers for sensitive data,
         * then FPCCR.TS, FPCCR.CLRONRET and FPCCR.CLRONRETS must be set at
         * initialisation and not changed again afterwards.
         */
        FPU->FPCCR |= FPU_FPCCR_TS_Msk
                      | FPU_FPCCR_CLRONRET_Msk
                      | FPU_FPCCR_CLRONRETS_Msk;
#endif
#endif

	/* Enable system exception about hardfault, for the hardfault can be 
	* triggered by the escalation of the memmanage fault, the bus fault,
	* the usage fault, the secure fault when these fault exceptions are
	* not available or not enable or of the same/lower priority than the
	* current exception priority level
	*/
	SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk;
	SCB->SHCSR |= SCB_SHCSR_SECUREFAULTENA_Msk;
	SCB->SHCSR |= SCB_SHCSR_USGFAULTENA_Msk;
	SCB->SHCSR |= SCB_SHCSR_BUSFAULTENA_Msk;
}

void tcm_spe()
{
#if CONFIG_SPE
#if defined (__ITCM_PRESENT) && (__ITCM_PRESENT == 1U)
	ITGU->TGU_LUT = 0xFFFF;
#endif

#if defined (__DTCM_PRESENT) && (__DTCM_PRESENT == 1U)
	DTGU->TGU_LUT = 0xFFFF;
#endif
#endif
}
// eof

