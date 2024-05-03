/*
 * Copyright (c) 2020 Realtek Semiconductor Corp.	All rights reserved.
 *
 * Author: PSP Software Group
 */

#include "sheipa.h"
#include "ameba_soc.h"
#include "barriers.h"

#ifndef CONFIG_PLATFORM_TIZENRT_OS
#include "task.h"
#endif

#ifdef CONFIG_PLATFORM_TIZENRT_OS
#ifdef CONFIG_CPU_GATING
extern volatile uint32_t ulFlashPG_Flag;
#endif
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
	/* ulFlashPG_Flag should be checked here, it should only exists under 3 states:
	0: No flash operation / Just completed a flash operation
	1: A gating request has been sent out to another core, further checking shall be done at the while condition below
	2: The target core is already in gating state, proceed for flash operation
	*/
	if (!ulFlashPG_Flag) {
		ulFlashPG_Flag = 1;
		ARM_DSB();

#ifndef CONFIG_PLATFORM_TIZENRT_OS
		arm_gic_raise_softirq(ulCoreID, IPI_FLASHPG_IRQ);
#else
		arm_cpu_sgi(GIC_IRQ_SGI3, (1 << ulCoreID));
#endif
	}
  /* We already initiated a gating request previously, skip sending duplicated request */
  /* Before gating the other CPU, we have to check for pending pause request, as the target core
   * might have entered spinlock to wait for current core to pause itself. And currently
   * we are in a interrupt disabled status here, thus we should exit and handle
   * pause request first, before we proceed to gate another cpu for executing
   * flash operation
   */
	while ((ulFlashPG_Flag == 1) || CA32_GET_STANDBYWFE(ca32->CA32_C0_CPU_STATUS) != BIT(ulCoreID)) {
		/* If there is a pause request, we should handle it first */
		if (up_cpu_pausereq(up_cpu_index())) {
			return false;
		}
	}
	return true;
#endif
}

void vPortWakeOtherCore(void)
{
	ulFlashPG_Flag = 0;
	ARM_DSB();
	__asm__ __volatile__ ("sev" : : : "memory");
}

void vPortSecondaryOff(void)
{
	int state;
	int count = 10;
#if ( CONFIG_SMP_NCPUS > 1 )

	/* Notify secondary core to migrate task to primary core and enter wfi*/
	// pmu_set_secondary_cpu_state(1, CPU1_HOTPLUG);
#ifndef CONFIG_PLATFORM_TIZENRT_OS
	arm_gic_raise_softirq(1, 1);
#else
	/* TODO: SGI4 for Task Migration 
	arm_cpu_sgi(GIC_IRQ_SGI4, (1 << 1));
	*/
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

	// printf("Secondary core power off fail: %d\n", state);
}

#ifndef CONFIG_PLATFORM_TIZENRT_OS
void vPortSecondaryStart(void)
{
	printf("CPU%d: on\n", (int)portGET_CORE_ID());

	/* Wait until scheduler starts */
	if (pmu_get_secondary_cpu_state(portGET_CORE_ID()) == CPU1_RUNNING)
		while (xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED);

#if ( configNUM_CORES > 1 )
	/* Configure the hardware ready to run the demo. */
	prvSetupHardwareSecondary();

	/* Enable ipi for yield core */
	configSETUP_IPI_INTERRUPT();
#endif
	/* Start the timer that generates the tick ISR. */
	configSETUP_TICK_INTERRUPT();

	/* Secondary core is up, set cpu state to CPU1_RUNNING */
	pmu_set_secondary_cpu_state(1, CPU1_RUNNING);

	/* Start the first task executing. */
	vPortRestoreTaskContext();
}
#endif
/*-----------------------------------------------------------*/

void smp_init(void)
{
	BaseType_t xCoreID;
	BaseType_t err;

	// lldbg("smp: Bringing up secondary CPUs ...\n");
	// DBG_PRINTF(MODULE_BOOT, LEVEL_INFO, "smp: Bringing up secondary CPUs ...\n");

#if ( CONFIG_SMP_NCPUS > 1 )
	// if (SYSCFG_CHIPType_Get() != CHIP_TYPE_RTLSIM) {//RTL sim shall not use delayus before core1 ready
		/* power on core1 to avoid km4 not open it */
	rtk_core1_power_on();
	DelayUs(50);
	// }
#else
	/* power off core1 to avoid km4 has already open it */
	rtk_core1_power_off();
#endif

#if ( CONFIG_SMP_NCPUS > 1 )
	for (xCoreID = 0; xCoreID < CONFIG_SMP_NCPUS; xCoreID++) {
		if (xCoreID == up_cpu_index()) {
			// pmu_set_secondary_cpu_state(xCoreID, CPU1_RUNNING);
			continue;
		}

		err = psci_cpu_on(xCoreID, (unsigned long)__cpu1_start);
		if (err < 0) {
			printf("CPU%d: failed to boot: %d\n", (int)xCoreID, (int)err);
		}
	}
#endif

}
