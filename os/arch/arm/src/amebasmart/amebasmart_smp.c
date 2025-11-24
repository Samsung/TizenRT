/*
 * Copyright (c) 2020 Realtek Semiconductor Corp.	All rights reserved.
 *
 * Author: PSP Software Group
 */

#include "sheipa.h"
#include "ameba_soc.h"
#include "barriers.h"
#include "smp.h"
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
 *   Powers on the CPU domain and initializes hardware for the secondary CPU.
 *   This function handles the low-level power management and hardware
 *   initialization of the cpu.
 *
 * Input Parameters:
 *   cpu - The CPU ID to power on
 *
 * Returned Value:
 *   OK (0) on success, negated error no on failure
 *
 * Assumptions:
 *   - Called from CPU0
 *   - Target CPU is currently powered off
 *
 ****************************************************************************/

int cpu_core_poweron(int cpu)
{
	/* Currently only supports CPU1 */
	if (cpu != 1) {
		smpdbg("%s only supports CPU1, got CPU%d\n", __func__, cpu);
		return -ENOTSUP;
	}

	/* turn on CPU1's power domain, require time to stabilize */
	rtk_core1_power_on();
	DelayUs(50);

	/* renable scu and invalidate tags on all processors */
	arm_enable_smp(0);

	return OK;
}

/****************************************************************************
 * Name: vPortSecondaryBoot
 *
 * Description:
 *   Starts the secondary CPU execution via PSCI and waits for it to come
 *   online. This function handles the complete boot sequence from powered-off
 *   state to CPU online, including PSCI boot and affinity validation.
 *
 * Input Parameters:
 *   cpu - The CPU ID to start
 *
 * Returned Value:
 *   OK (0) on success, negated error no on failure
 *
 * Assumptions:
 *   - Called from CPU0
 *   - Target CPU core has been powered on
 *
 ****************************************************************************/

int vPortSecondaryBoot(int cpu)
{
	BaseType_t err;
	int state;
	int count = 10;

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
		smpdbg("Secondary core boot timeout affinfo: %d\n", state);
		return -ETIMEDOUT;
	}

	return OK;
}

/****************************************************************************
 * Name: cpu_core_powerdown
 *
 * Description:
 *   Powers down the specified CPU core after ensuring it's in a safe state.
 *   This function handles the core power down operations including PSCI
 *   coordination and RTK power off.
 *
 * Input Parameters:
 *   cpu - The CPU ID to power down
 *
 * Returned Value:
 *   OK (0) on success, negated error no on failure
 *
 * Assumptions:
 *   - CPU has already been hot-plugged
 *   - Called from CPU0
 *
 ****************************************************************************/

int cpu_core_powerdown(int cpu)
{
	int count = 10; // 10 * 50us = 500us
	int state;

	/* Currently only supports CPU1 */
	if (cpu != 1) {
		smpdbg("%s only supports CPU1, got CPU%d\n", __func__, cpu);
		return -ENOTSUP;
	}

	/* wait for PSCI to report that CPU1 is now safe to power off */
	do {
		state = psci_affinity_info(cpu, 0);
		if (state == AFF_STATE_OFF) {
			/* turn off CPU1's power domain */
			rtk_core1_power_off();
			return OK;
		}

		DelayUs(50);
	} while (count--);

	smpdbg("Core powerdown timeout for CPU%d, affinfo: %d\n", cpu, state);
	return -ETIMEDOUT;
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

	smpdbg("Secondary core power off fail: %d\n", state);
}

/****************************************************************************
 * Name: up_cpu_die
 *
 * Description:
 *   Shut down the current CPU core using PSCI. This function is called by a
 *   CPU that needs to power itself down as part of the hot-plug sequence.
 *   It invokes the PSCI CPU_OFF service which puts the calling CPU into
 *   a low-power state and prevents it from being scheduled for execution.
 *
 *   This is a terminal function that does not return. The CPU will remain
 *   in the powered-down state until it is explicitly powered back on by
 *   another CPU via PSCI_CPU_ON.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   Does not return to the caller.
 *
 * Assumptions:
 *   - Called from the target CPU that needs to shut down
 *   - CPU has already completed all necessary cleanup and state saving
 *   - Task scheduling has been disabled for this CPU
 *   - All interrupts have been properly handled on this CPU
 *
 ****************************************************************************/

void up_cpu_die(void)
{
	/* MINOR: PSCI is a general interface for controlling power of cortex-A cores
	It is one of the feature in arm-trusted-firmware (ie. Trustzone-A)
	Currently, the definitions are provided by chip specific arch layer
	If the cpu off operation failed, we should face a problem during wakeup
	booting the secondary core, thus we can ignore the return value here first
	*/

	/* Shut down the cpu */
	(void)psci_cpu_off();
}

/****************************************************************************
 * Name: up_cpu_down
 *
 * Description:
 *   Power down a CPU core by turning off its power domain. This function
 *   waits for the target CPU to enter OFF state via PSCI affinity info,
 *   then completely cuts power to the CPU domain.
 *
 * Input Parameters:
 *   cpu - The index of the CPU to power down
 *
 * Returned Value:
 *   Zero (OK) on success, negated error no on failure
 *
 * Assumptions:
 *   - Called from CPU0
 *   - Target CPU has been hot-plugged out
 *   - All tasks have been migrated away from target CPU
 *
 ****************************************************************************/

int up_cpu_down(int cpu)
{
	int ret;

	if (cpu <= 0 || cpu >= CONFIG_SMP_NCPUS) {
		smpdbg("cpu number out of range\n");
		return -EINVAL;
	}

	smpvdbg("Starting core powerdown for CPU%d\n", cpu);

	ret = cpu_core_powerdown(cpu);
	if (ret < 0) {
		smpdbg("Failed to powerdown CPU%d, errno: %d\n", cpu, ret);
		return ret;
	}

	smpvdbg("Secondary core CPU%d powerdown complete\n", cpu);
	return OK;
}

/****************************************************************************
 * Name: up_cpu_up
 *
 * Description:
 *   Power on and boot a CPU core that was previously powered down. This function
 *   performs the complete power-on sequence: enables the CPU power domain,
 *   reinitializes the SCU for cache coherency, and cold-boots the secondary
 *   core via PSCI to the specified entry point.
 *
 * Input Parameters:
 *   cpu - The index of the CPU to power on
 *
 * Returned Value:
 *   Zero (OK) on success, negated error no on failure
 *
 * Assumptions:
 *   - Called from CPU0
 *   - Target CPU is currently powered off
 *   - System is ready to bring additional CPU online
 *
 ****************************************************************************/

int up_cpu_up(int cpu)
{
	int ret;

	if (cpu <= 0 || cpu >= CONFIG_SMP_NCPUS) {
		smpdbg("cpu number out of range\n");
		return -EINVAL;
	}

	/* Power on the CPU core */
	ret = cpu_core_poweron(cpu);
	if (ret != OK) {
		smpdbg("Failed to power on CPU%d: errno = %d\n", cpu, ret);
		return ret;
	}

	smpvdbg("Booting secondary core... \n");

	/* cold-boot the core via PSCI */
	ret = vPortSecondaryBoot(cpu);
	if (ret != OK) {
		smpdbg("Failed to boot CPU%d: %d\n", cpu, ret);
		return ret;
	}

	smpvdbg("Secondary core booted successfully\n");
	return OK;
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
