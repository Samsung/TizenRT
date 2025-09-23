/*
 * Copyright (c) 2020 Realtek Semiconductor Corp.	All rights reserved.
 *
 * Author: PSP Software Group
 */

#include "sheipa.h"
#include "ameba_soc.h"
#include "barriers.h"
#include "smp.h"

#define SECONDARY_CORE_ID     1

#ifndef CONFIG_PLATFORM_TIZENRT_OS
#include "task.h"
#endif

#ifdef CONFIG_PLATFORM_TIZENRT_OS
extern void __cpu1_start(void);
#else
extern void _boot(void);
extern void vPortRestoreTaskContext(void);
#endif

/*-----------------------------------------------------------*/

void rtk_core1_power_on(void)
{
	u32 val;
	CA32_TypeDef *ca32 = CA32_BASE;

	ca32->CA32_C0_RST_CTRL &= (~(CA32_NCOREPORESET(0x2) | CA32_NCORERESET(0x2)));

	val =  HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_ISO);
	val |= (HSYS_ISO_HP_AP_CORE(0x2));
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_ISO, val);
	DelayUs(50);

	val =  HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_PWC);
	val |= (HSYS_PSW_HP_AP_CORE(0x3));
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_PWC, val);
	DelayUs(50);

	val =  HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_PWC);
	val |= (HSYS_PSW_HP_AP_CORE_2ND(0x3));
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_PWC, val);
	DelayUs(500);

	val =  HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_ISO);
	val &= ~(HSYS_ISO_HP_AP_CORE(0x3));
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_ISO, val);
	DelayUs(50);

	ca32->CA32_C0_RST_CTRL |= (CA32_NCOREPORESET(0x2) | CA32_NCORERESET(0x2) | CA32_BIT_NRESETSOCDBG | CA32_BIT_NL2RESET | CA32_BIT_NGICRESET);

}


void rtk_core1_power_off(void)
{
	u32 val;

	val =  HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_ISO);
	val |= (HSYS_ISO_HP_AP_CORE(0x2));
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_ISO, val);

	val =  HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_PWC);
	val &= ~(HSYS_PSW_HP_AP_CORE_2ND(0x2));
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_PWC, val);

	val =  HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_PWC);
	val &= ~(HSYS_PSW_HP_AP_CORE(0x2));
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_PWC, val);
}

bool vPortGateOtherCore(void)
{
#if ( defined(CONFIG_SMP) && CONFIG_SMP_NCPUS > 1 )
	BaseType_t ulCoreID = up_cpu_index();
	ulCoreID = (ulCoreID + 1) % CONFIG_SMP_NCPUS;
	CA32_TypeDef *ca32 = CA32_BASE;
	/* Gating Flag should be checked here, it should only exists under 3 states:
	0: No flash operation / Just completed a flash operation
	1: A gating request has been sent out to another core, further checking shall be done at the while condition below
	2: The target core is already in gating state, proceed for flash operation
	*/
	if (!up_get_gating_flag_status(ulCoreID)) {
		up_set_gating_flag_status(ulCoreID, 1);

		up_cpu_gating(ulCoreID);
	}
  /* We already initiated a gating request previously, skip sending duplicated request */
  /* Before gating the other CPU, we have to check for pending pause request, as the target core
   * might have entered spinlock to wait for current core to pause itself. And currently
   * we are in a interrupt disabled status here, thus we should exit and handle
   * pause request first, before we proceed to gate another cpu for executing
   * flash operation
   */
	while ((up_get_gating_flag_status(ulCoreID) == 1) || CA32_GET_STANDBYWFE(ca32->CA32_C0_CPU_STATUS) != BIT(ulCoreID)) {
		/* If there is a pause request, we should handle it first */
		if (up_cpu_pausereq(up_cpu_index())) {
			return false;
		} else if (up_is_cpu_paused(ulCoreID)) {
			break;
		}
	}
	return true;
#endif
}

void vPortSecondaryStart(void)
{
	int count = 10;
	int state;

	/* take the lock, as we are about to put cpu1 idle task back active */
	sched_lock();
	
	/* turn on CPU1's power domain, require time to stabilize */
	rtk_core1_power_on();
	DelayMs(1);

	/* renable scu and invalidate tags on all processors */
	arm_enable_smp(0);

	/* cold-boot the core */
	lldbg("Booting secondary core... \n");
	BaseType_t err = psci_cpu_on(SECONDARY_CORE_ID, (unsigned long)__cpu1_start);
	DEBUGASSERT(err >= 0);

	/* await for PSCI to report cpu state */
	do {
		state  = psci_affinity_info(1, 0);
		if (state == 1) {
			break;
		}

		DelayUs(50);
	} while (count--);

	lldbg("Secondary core booted?... %d\n", err);
	up_set_cpu_state(SECONDARY_CORE_ID, CPU_RUNNING);
	
	/* fire SGI1 to kick the freshly booted CPU into TizenRT Idle Task */
	os_smp_start();

	/* set the idle task state of cpu1 back to running, after it has booted */
	os_set_cpu_idle_task(SECONDARY_CORE_ID, TSTATE_TASK_RUNNING);

	/* cpu1 is now alive again and can be scheduled on */
	sched_unlock();
}

void smp_full_powerdown_cpu(void)
{
	/* take the lock, as we are about to kill cpu1 */
	sched_lock();

	/* send SGI4 to make core enter halt state (not complete powerdown yet) */
	up_cpu_haltcore(SECONDARY_CORE_ID);

	/* release the lock, cpu1 is no longer part of SMP and the idle task is marked inactive */
	sched_unlock();
	while (up_get_cpu_state(SECONDARY_CORE_ID) != CPU_HALTED);
	lldbg("Secondary core shutdown?... %d\n", up_get_cpu_state(SECONDARY_CORE_ID));
	
	/* turn off CPU1's power domain */
	rtk_core1_power_off();
}

void vPortWakeOtherCore(void)
{
	BaseType_t ulCoreID = up_cpu_index();
	ulCoreID = (ulCoreID + 1) % CONFIG_SMP_NCPUS;
	up_set_gating_flag_status(ulCoreID, 0);
}

void vPortSecondaryOff(void)
{
	int state;
	int count = 10;
#if ( CONFIG_SMP_NCPUS > 1 )

#ifdef CONFIG_CPU_HOTPLUG
	/* Notify secondary core to migrate task to primary core and enter wfi*/
	up_set_cpu_state(1, CPU_HOTPLUG);
#endif
#ifndef CONFIG_PLATFORM_TIZENRT_OS
	arm_gic_raise_softirq(1, 1);
#else
	arm_cpu_sgi(GIC_IRQ_SGI4, (1 << 1));
#endif
#endif
	//add a delay to wait cpu1 enter wfi.
	DelayUs(100);

	/* Power off secondary core */
	do {
		state  = psci_affinity_info(1, 0);
		if (state == 1) {
			rtk_core1_power_off();
			return;
		}

		DelayUs(50);
	} while (count--);

	dbg("Secondary core power off fail: %d\n", state);
}

void smp_init(void)
{
	BaseType_t xCoreID;
	BaseType_t err;

#if ( CONFIG_SMP_NCPUS > 1 )
	rtk_core1_power_on();
	DelayUs(50);
#else
	/* power off core1 to avoid km4 has already open it */
	rtk_core1_power_off();
#endif

#if ( CONFIG_SMP_NCPUS > 1 )
	for (xCoreID = 0; xCoreID < CONFIG_SMP_NCPUS; xCoreID++) {
		if (xCoreID == up_cpu_index()) {
#ifdef CONFIG_CPU_HOTPLUG
			up_set_cpu_state(xCoreID, CPU_RUNNING);
#endif
			continue;
		}
		err = psci_cpu_on(xCoreID, (unsigned long)__cpu1_start);

		/* Wait for CPU1 to boot from ATF to SGI1 WFI loop, before os_smp_start is called */
		DelayUs(50);

		/* If we failed to boot secondary core here, it will be very likely
		issue is coming from ATF/kernel flow, and that operation is irreversible
		(ie. There is no way we can restore the secondary core to come
		back to Tizen Lite, thus we should trigger a crash here to check)
		*/
		DEBUGASSERT(err >= 0);
	}
#endif
	/* We should check whether it is a warm boot here
	   If yes, we should send SGI1 to secondary core
	 */
#ifdef CONFIG_CPU_HOTPLUG
	if (up_get_cpu_state(SECONDARY_CORE_ID) == CPU_WAKE_FROM_SLEEP) {
		os_smp_start();
	}
#endif
}
