/*
 * Copyright (c) 2020 Realtek Semiconductor Corp.	All rights reserved.
 *
 * Author: PSP Software Group
 */

#include <stdio.h>

#ifndef CONFIG_PLATFORM_TIZENRT_OS
#include "FreeRTOS.h"
#include "task.h"
#endif

#include "sheipa.h"
#include "ameba_soc.h"
#include "cpu.h"

extern uint32_t ulPortInterruptNesting[ configNUM_CORES ];

uint64_t GenericTimerFreq;
#define pdTICKS_TO_CNT	(GenericTimerFreq/configTICK_RATE_HZ)

static volatile InterruptTable_t xInterruptTable[ GIC_MAX_NUM_INTR  ];

void vRegisterIRQHandler(uint32_t ulID, ISRCallback_t pxHandler, void *pvContext);
void vConfigureIPIInterrupt(void);

/*-----------------------------------------------------------*/
uint64_t vGetGenericTimerFreq(void)
{
	if (SYSCFG_CHIPType_Get() == CHIP_TYPE_FPGA) {
		return 12500000;
	} else {
		return 50000000;
	}
}

void prvSetupHardware(void)
{
	/* Ensure no interrupts execute while the scheduler is in an inconsistent
	state.  Interrupts are automatically enabled when the scheduler is
	started. */
	portDISABLE_INTERRUPTS();

	GenericTimerFreq = vGetGenericTimerFreq();

	arm_gic_init();

	/* Booting other cores */
#if ( configNUM_CORES > 1 )
	{
		psci_init();
		smp_init();

#ifndef AMEBAD2_TODO
		if ((SYSCFG_CUT_VERSION_A == SYSCFG_RLVersion()) && (SYSCFG_CHIPType_Get() != CHIP_TYPE_RTLSIM)) {
			DelayUs(150);/* add delay to wait cpu1 set gic in atf done */
		}
#endif
	}
#endif
}

#if ( configNUM_CORES > 1 )
void prvSetupHardwareSecondary(void)
{
	portDISABLE_INTERRUPTS();

	arm_gic_init_secondary();
}
#endif
/*-----------------------------------------------------------*/

#if ( configNUM_CORES > 1 )

void vDisableIPIInterrupt(void)
{
	vRegisterIRQHandler(IPI_CPUHP_IRQ, NULL, 0);
	arm_gic_irq_disable(IPI_CPUHP_IRQ);
	arm_gic_clear_pending_irq(IPI_CPUHP_IRQ);

}


void vConfigureIPIInterrupt(void)
{
	vRegisterIRQHandler(IPI_IRQ, (ISRCallback_t)FreeRTOS_IPI_Handler, 0);

	arm_gic_irq_set_priority(IPI_IRQ, INT_PRI_MIDDLE, IRQ_TYPE_LEVEL);
	arm_gic_irq_enable(IPI_IRQ);

	vRegisterIRQHandler(IPI_CPUHP_IRQ, (ISRCallback_t)FreeRTOS_IPI_CPUHP_Handler, 0);

	arm_gic_irq_set_priority(IPI_CPUHP_IRQ, INT_PRI_MIDDLE, IRQ_TYPE_LEVEL);
	arm_gic_irq_enable(IPI_CPUHP_IRQ);
}

void vClearIPIInterrupt(void)
{
	arm_gic_deactive_irq(IPI_IRQ);
}

void vConfigureSMPSendIPI(uint32_t ulCoreID)
{
	if (pmu_get_secondary_cpu_state(ulCoreID) == CPU1_RUNNING) {
		arm_gic_raise_softirq(ulCoreID, IPI_IRQ);
	}
}
#endif
/*-----------------------------------------------------------*/

//#define CYCLES_PER_TICK		(configCPU_CLOCK_HZ / configTICK_RATE_HZ)

#if( configUSE_TICKLESS_IDLE == 1 )
static uint64_t xMaximumPossibleSuppressedTicks = 0;
#endif

void vConfigureTickInterrupt(void)
{
#if( configUSE_TICKLESS_IDLE == 1 )
	xMaximumPossibleSuppressedTicks = -1ULL / pdTICKS_TO_CNT;
#endif

	vRegisterIRQHandler(ARM_ARCH_TIMER_IRQ, (ISRCallback_t)FreeRTOS_Tick_Handler, 0);

	arm_arch_timer_set_compare(arm_arch_timer_count() + pdTICKS_TO_CNT);
	arm_arch_timer_enable(pdTRUE);

	arm_gic_irq_set_priority(ARM_ARCH_TIMER_IRQ, INT_PRI_LOWEST, IRQ_TYPE_LEVEL);
	arm_gic_irq_enable(ARM_ARCH_TIMER_IRQ);
}

void vClearTickInterrupt(void)
{
	uint32_t delta_ticks;
	uint64_t last_cycle;

	last_cycle = arm_arch_timer_compare();

#if( configUSE_TICKLESS_IDLE == 1 )
	if (arm_arch_timer_count() < last_cycle) {
		return;
	} else {
		delta_ticks = (uint32_t)((arm_arch_timer_count() - last_cycle) / pdTICKS_TO_CNT) + 1;
	}
#else
	delta_ticks = 1;
#endif

	arm_arch_timer_set_compare(last_cycle + delta_ticks * pdTICKS_TO_CNT);
}

#if( configUSE_TICKLESS_IDLE == 1 )
#if 0
void vPortSuppressTicksAndSleep(TickType_t xExpectedIdleTime)
{
	uint64_t ulCompleteTickPeriods, ulLeftTickPeriods;
	BaseType_t xCoreID = portGET_CORE_ID();

	/* Make sure the Generic Timer reload value does not overflow the counter. */
	if (xExpectedIdleTime > xMaximumPossibleSuppressedTicks) {
		xExpectedIdleTime = xMaximumPossibleSuppressedTicks;
	}

	/* Enter a critical section but don't use the taskENTER_CRITICAL()
	method as that will mask interrupts that should exit sleep mode. */
	portDISABLE_INTERRUPTS();
	portGET_TASK_LOCK();

	/* If a context switch is pending or a task is waiting for the scheduler
	to be unsuspended then abandon the low power entry. */
	if ((xCoreID == 0) && (eTaskConfirmSleepModeStatus() == eAbortSleep)) {
		portRELEASE_TASK_LOCK();
		portENABLE_INTERRUPTS();

		return;
	}

	portRELEASE_TASK_LOCK();

	/* Calculate the reload value required to wait xExpectedIdleTime
	tick periods.  -1 is used because this code will execute part way
	through one of the tick periods. */
	arm_arch_timer_set_compare(arm_arch_timer_compare() + (xExpectedIdleTime - 1) * pdTICKS_TO_CNT);

	__asm("	DSB");
	__asm("	WFI");
	__asm("	ISB");

	if (arm_arch_timer_condition_met()) {
		/* As the pending tick will be processed as soon as this
		function exits, the tick value maintained by the tick is stepped
		forward by one less than the time spent waiting. */
		ulCompleteTickPeriods = xExpectedIdleTime - 1UL;
	} else {
		/* Something other than the tick interrupt ended the sleep.
		Work out how long the sleep lasted rounded to complete tick
		periods. */
		ulLeftTickPeriods = (arm_arch_timer_compare() - arm_arch_timer_count()) / pdTICKS_TO_CNT;
		arm_arch_timer_set_compare(arm_arch_timer_compare() - ulLeftTickPeriods * pdTICKS_TO_CNT);
		ulCompleteTickPeriods = xExpectedIdleTime - ulLeftTickPeriods - 1;
	}

	if (xCoreID == 0) {
		vTaskStepTick(ulCompleteTickPeriods);
	}

	/* Re-enable interrupts to allow the interrupt that brought the MCU
	out of sleep mode to execute immediately. */
	portENABLE_INTERRUPTS();
}
#endif
#endif /* configUSE_TICKLESS_IDLE */

/*-----------------------------------------------------------*/

void vRegisterIRQHandler(uint32_t ulID, ISRCallback_t pxHandler, void *pvContext)
{
	if (ulID < GIC_MAX_NUM_INTR) {
		xInterruptTable[ ulID ].pxISR = pxHandler;
		xInterruptTable[ ulID ].pvContext = pvContext;
	}
}
/*-----------------------------------------------------------*/

void vApplicationIRQHandler(void)
{
	uint32_t ulInterruptStat, ulInterruptID;
	UBaseType_t ulCoreID = 0;
	ISRCallback_t pxISR;

	UBaseType_t ulPrioMask;
	ulCoreID = portGET_CORE_ID();

	do {
		ulInterruptStat = arm_gic_get_active();
		ulInterruptID = ulInterruptStat & 0x3FF;

		if (ulInterruptID == 0x3FF) {
			break;
		} else if (ulInterruptID < GIC_MAX_NUM_INTR) {
			arm_gic_freq_switch();

			/* Store Current ICC_PMR*/
			ulPrioMask = portICCPMR_PRIORITY_MASK_REGISTER;

			/* Update ICC_PMR, For Nesting interrupt,
			 irq priority lower than current irq will be masked */
			portICCPMR_PRIORITY_MASK_REGISTER = portICCRPR_RUNNING_PRIORITY_REGISTER;

			arm_gic_freq_restore();

			asm volatile("DSB \n");
			asm volatile("ISB \n");

			/* Restrict irq Nesting depth to MaxNsetingDepth */
			if (ulPortInterruptNesting[ ulCoreID ] < MAX_IRQ_NESTING_DEPTH) {
				portCPU_IRQ_ENABLE();
			}

			/* Call the function installed in the array of installed handler
			functions. */
			pxISR = xInterruptTable[ ulInterruptID ].pxISR;
			if (pxISR) {
				pxISR(xInterruptTable[ ulInterruptID ].pvContext);
			} else {
				printf("ISR for interrupt id(%lu) is not registered!\n ", (unsigned long)ulInterruptID);
				for (;;);
			}

			/* Disable interrupt on exception return,
			 Interrupt will be enabled after restore context */
			portCPU_IRQ_DISABLE();
			arm_gic_eoi(ulInterruptID);

			arm_gic_freq_switch();
			/* Restore ICC_PMR */
			portICCPMR_PRIORITY_MASK_REGISTER = ulPrioMask;
			arm_gic_freq_restore();
			continue;
		}
		break;
	} while (1);
}
