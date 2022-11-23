/**************************************************************************//**
 * @file     rtl8721dlp_system.c
 * @brief    CMSIS Device System Source File for
 *           ARMCM3 Device Series
 * @version  V1.08
 * @date     23. November 2012
 *
 * @note
 *
 ******************************************************************************/
/* Copyright (c) 2011 - 2012 ARM LIMITED

   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   - Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   - Neither the name of ARM nor the names of its contributors may be used
     to endorse or promote products derived from this software without
     specific prior written permission.
   *
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
   ---------------------------------------------------------------------------*/


#include "basic_types.h"
#include "ameba_soc.h"

#define MHZ_TICK_CNT (1000000)

/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
uint32_t SystemCoreClock = 150000000;/*!< System Clock Frequency (Core Clock)*/

extern uint64_t ullNextTime;
extern size_t uxTimerIncrementsForOneTick;
extern volatile uint64_t *pullMachineTimerCompareRegister;
extern volatile uint64_t *pullMachineTimerCounterRegister;
extern uint32_t const ullMachineTimerCompareRegisterBase;

/*----------------------------------------------------------------------------
  Clock functions
 *----------------------------------------------------------------------------*/
void SystemCoreClockUpdate(void)             /* Get Core Clock Frequency      */
{
	SystemCoreClock = CPU_ClkGet();
}

/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System.
 */
void SystemInit(void)
{
	// TODO: Hardware initial
#ifdef UNALIGNED_SUPPORT_DISABLE
	SCB->CCR |= SCB_CCR_UNALIGN_TRP_Msk;
#endif
}

IMAGE2_RAM_TEXT_SECTION
void Systick_Cmd(u32 enable)
{
	u32 Rtemp;

	if (enable) {
		/* enablr mtick clock */
		Rtemp = HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_SYSTEM_CFG1);
		Rtemp |= LSYS_BIT_PLIC_TIMER_CKEN;
		HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_SYSTEM_CFG1, Rtemp);

	} else {
		/* Disable mtick clock */
		Rtemp = HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_SYSTEM_CFG1);
		Rtemp &= (~LSYS_BIT_PLIC_TIMER_CKEN);
		HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_SYSTEM_CFG1, Rtemp);
	}

	__DSB();
}

IMAGE2_RAM_TEXT_SECTION
u32 Systick_State(void)
{
	u32 Rtemp = HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_SYSTEM_CFG1);

	if (LSYS_GET_PLIC_TIMER_CKEN(Rtemp)) {
		return ENABLE;
	} else {
		return DISABLE;
	}
}

/* Notice: Maximum miss 1 tick because of cpu clock changed */
IMAGE2_RAM_TEXT_SECTION
void Systick_update(void)
{
	volatile uint32_t ulHartId;
	volatile uint64_t ullcurrenttick = 0ULL;
	volatile uint64_t *const pulTime = (volatile uint64_t *const)(configMTIME_BASE_ADDRESS);

	__asm volatile("csrr %0, mhartid" : "=r"(ulHartId));
	pullMachineTimerCompareRegister  = (volatile uint64_t *)(ullMachineTimerCompareRegisterBase + (ulHartId * sizeof(uint64_t)));


	ullcurrenttick = (uint64_t)(*pulTime);

	/* update one tick count */
	uxTimerIncrementsForOneTick = (size_t)((configCPU_CLOCK_HZ) / (configTICK_RATE_HZ));
	*pullMachineTimerCompareRegister = ullcurrenttick + uxTimerIncrementsForOneTick;

	ullNextTime = *pullMachineTimerCompareRegister + uxTimerIncrementsForOneTick;
}

/**
  * @brief  Set CPU clock Source.
  * @param  Source: This parameter can be one of the following values:
  *		 @arg CLK_CPU_DPLL
  *		 @arg CLK_CPU_MPLL
  *		 @arg CLK_CPU_LBUS
   *		 @arg CLK_CPU_XTAL
  */
IMAGE2_RAM_TEXT_SECTION
void CPU_ClkSet(u32 Source)
{
	u32 CpuClk, PreState;
	u32 Temp = HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_CKSL_GRP0);

	/* 1. Select CPU clock source */
	if (Source != LSYS_GET_CKSL_PLFM(Temp)) {
		Temp &= ~(LSYS_MASK_CKSL_PLFM);
		Temp |=  LSYS_CKSL_PLFM(Source);
		HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_CKSL_GRP0, Temp);

		__DSB();
		__NOP();
	}

	CpuClk = CPU_ClkGet(); /* CPU Clock is changed*/

	/* 2. update for DelayUs */
	RBSS_UDELAY_CLK = CpuClk / MHZ_TICK_CNT;

	/* 3. update for OS */
	SystemCoreClock = CpuClk;

	/* 4. update systick */
	PreState = Systick_State();
	Systick_Cmd(DISABLE);
	Systick_update();
	Systick_Cmd(PreState);
}

/**
 * Disable IRQ Interrupts and Save Previous Status
 *
 * @param  none
 * @return the previous value of IQR status
 *
 * @brief  disables interrupts by clearing the MIE-bit in the
 * mstatus register, and returns the previous value of that register.
 * irq_disable_save and irq_enable_restore normally appear in pairs
 */
uint32_t irq_disable_save()
{
	return __disable_irq_save();
}

/**
 * enable IRQ Interrupts according to Previous Status
 *
 * @param  the previous value of IQR status
 * @return none
 *
 * @brief  This function enables interrupt by restoring the saved interrupt status register.
 * irq_disable_save and irq_enable_restore normally appear in pairs
 */
void irq_enable_restore(uint32_t PrevStatus)
{
	__enable_irq_restore(PrevStatus);
}

