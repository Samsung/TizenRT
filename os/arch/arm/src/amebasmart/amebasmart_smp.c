/*
 * Copyright (c) 2020 Realtek Semiconductor Corp.	All rights reserved.
 *
 * Author: PSP Software Group
 */

#include "sheipa.h"
#include "ameba_soc.h"
#include "barriers.h"
#include "smp.h"
#include <tinyara/cpu_state.h>
#include <errno.h>

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

/* from psci.h aff_info_state_t */
typedef enum {
	AFF_STATE_ON = 0,
	AFF_STATE_OFF = 1,
	AFF_STATE_ON_PENDING = 2
} aff_info_state_t;

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

/****************************************************************************
 * Name: cpu_core_poweron
 *
 * Description:
 *   Powers on the CPU domain, initializes hardware, starts the CPU
 *   execution via PSCI, and waits for it to come online. This function
 *   handles the complete power-on and boot sequence from powered-off state
 *   to CPU online, but does not integrate it with the TizenRT scheduler.
 *
 * Input Parameters:
 *   cpu - The CPU ID to power on
 *
 * Returned Value:
 *   OK (0) on success, negative error on failure
 *
 * Assumptions:
 *   - Called from CPU0
 *   - Target CPU is currently powered off
 *
 ****************************************************************************/

int cpu_core_poweron(int cpu)
{
	BaseType_t err;
	int count = 10;
	int state;

	if(cpu != 1) {
		lldbg("cpu number out of range");
		return -EINVAL;
	}

	/* turn on CPU1's power domain, require time to stabilize */
	rtk_core1_power_on();
	DelayMs(1);

	/* renable scu and invalidate tags on all processors */
	arm_enable_smp(0);

	cpu_set_state(cpu, CPU_HOTPLUG);

	/* cold-boot the core via PSCI */
	lldbg("Starting secondary core... \n");
	err = psci_cpu_on(cpu, (unsigned long)__cpu1_start);
	DEBUGASSERT(err >= 0);

	/* await for PSCI to report cpu state */
	do {
		state = psci_affinity_info(cpu, 0);
		if (state == AFF_STATE_ON) {
			break;
		}

		DelayUs(50);
	} while (count--);

	if (count <= 0) {
		lldbg("Secondary core boot timeout affinfo: %d\n", state);
		return -ETIMEDOUT;
	} else {
		lldbg("Secondary core booted successfully\n");
		return OK;
	}
}

/****************************************************************************
 * Name: vPortSecondaryStart
 *
 * Description:
 *   Integrates the already online secondary CPU with the TizenRT scheduler.
 *   This function should be called after vPortSecondaryPowerOn() to complete
 *   the CPU bring-up process by setting the CPU state and kicking it into
 *   the TizenRT idle task.
 *
 * Input Parameters:
 *   cpu - The CPU ID to boot
 *
 * Returned Value:
 *   OK (0) on success, negative error on failure
 *
 * Assumptions:
 *   - Called from CPU0
 *   - Target CPU has been powered on
 *
 ****************************************************************************/

int vPortSecondaryStart(int cpu)
{
	int ret;

	if(cpu != 1) {
		lldbg("cpu number out of range");
		return -EINVAL;
	}

	cpu_set_state(cpu, CPU_SCHED_OFF);

	/* fire SGI1 to kick the freshly booted CPU into TizenRT Idle Task */
	ret = up_cpu_start(cpu);
	if (ret != OK) {
		lldbg("Failed to start CPU%d via SGI1: %d\n", cpu, ret);
		return ret;
	}

	return OK;
}

/****************************************************************************
 * Name: cpu_core_powerdown
 *
 * Description:
 *   Powers down the specified CPU core after ensuring it's in a safe state.
 *   This function handles the core power down operations including PSCI
 *   coordination and RTK power off. It assumes the CPU has already been
 *   hot-plugged (tasks migrated, CPU in halt state).
 *
 * Input Parameters:
 *   cpu - The CPU ID to power down
 *
 * Returned Value:
 *   None
 *
 * Assumptions:
 *   - CPU has already been hot-plugged and is in halt state
 *   - Called from CPU0
 *
 ****************************************************************************/

void cpu_core_powerdown(int cpu)
{
	int count = 10; // 10 * 50us = 500us
	int state;

	if(cpu != 1) {
		lldbg("cpu number out of range");
		return;
	}

#ifdef CONFIG_CPU_HOTPLUG
	lldbg("Starting core powerdown for CPU%d\n", cpu);

	/* wait for PSCI to report that CPU1 is now safe to power off */
	do {
		state = psci_affinity_info(cpu, 0);
		if (state == AFF_STATE_OFF) {
			cpu_set_state(cpu, CPU_HALTED);

			lldbg("Secondary core powerdown complete: %d\n", cpu_get_state(cpu));
			/* turn off CPU1's power domain */
			rtk_core1_power_off();
			return;
		}

		DelayUs(50);
	} while (count--);

	lldbg("Core powerdown timeout for CPU%d, state: %d\n", cpu, state);
#endif
}

/****************************************************************************
 * Name: smp_hotplug_cpu
 *
 * Description:
 *   Performs a hot-plug operation on a CPU without full power-down. This
 *   function temporarily takes a CPU offline for power management purposes
 *   by putting the CPU in a halt state. This function should be called after
 *   task migration has been completed. The CPU remains powered but inactive,
 *   allowing for faster resume compared to full power cycling.
 *
 * Input Parameters:
 *   cpu - The CPU ID to hot-plug
 *
 * Returned Value:
 *   OK (0) on success, negative error on failure
 *
 * Assumptions:
 *   - Called from CPU0
 *   - Target CPU is currently running and can be safely halted
 *   - Target CPU tasks have been migrated
 *
 ****************************************************************************/

int smp_hotplug_cpu(int cpu)
{
	int ret = OK;

	if(cpu != 1) {
		lldbg("cpu number out of range");
		return -EINVAL;
	}

#ifdef CONFIG_CPU_HOTPLUG
	/* take the lock, as we are about to kill cpu1 */
	sched_lock();

	/* send SGI3 to make core enter halt state (not complete powerdown yet) */
	ret = up_cpu_hotplug(cpu);
	if(ret != OK) {
		sched_unlock();
		return ret;
	}

	/* release the lock, cpu1 is no longer part of SMP and the idle task is marked inactive */
	sched_unlock();

	lldbg("Secondary core hotplug complete: %d\n", cpu_get_state(cpu));
	return OK;
#endif
}

void vPortSecondaryOff(void)
{
	int state;
	int count = 10;
#if ( CONFIG_SMP_NCPUS > 1 )

#ifdef CONFIG_CPU_HOTPLUG
	/* Notify secondary core to migrate task to primary core and enter wfi*/
	cpu_set_state(1, CPU_HOTPLUG);
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
			cpu_set_state(xCoreID, CPU_RUNNING);
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
}
