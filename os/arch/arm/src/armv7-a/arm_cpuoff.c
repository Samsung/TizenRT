/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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
/****************************************************************************
 * arch/arm/src/armv7-a/arm_cpuoff.c
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdint.h>
#include <assert.h>
#include <debug.h>
#include <errno.h>

#include <tinyara/arch.h>
#include <tinyara/sched.h>
#include <tinyara/sched_note.h>

#include "up_internal.h"
#include "cp15_cacheops.h"
#include "gic.h"
#include "sched/sched.h"
#include "barriers.h"
#include "smp.h"
#include "arm.h"

#ifdef CONFIG_CPU_HOTPLUG

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* Spinlocks for synchronizing CPU hotplug requests and handling */
static volatile spinlock_t g_cpu_hotpluged[CONFIG_SMP_NCPUS];
static volatile spinlock_t g_cpu_hotplughandled[CONFIG_SMP_NCPUS];

/* CPU hotplug state flags for each core */
static volatile cpu_state_t g_cpuhp_flag[CONFIG_SMP_NCPUS];

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_set_cpu_state
 *
 * Description:
 *   Set the hotplug state of a specific CPU core.
 *
 * Input Parameters:
 *   CoreID   - The ID of the CPU core
 *   NewStatus - The new state to set (CPU_RUNNING, CPU_HOTPLUG, etc.)
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void up_set_cpu_state(uint32_t CoreID, cpu_state_t NewStatus)
{
	g_cpuhp_flag[CoreID] = NewStatus;
	ARM_DSB();
}

/****************************************************************************
 * Name: up_get_cpu_state
 *
 * Description:
 *   Get the current hotplug state of a specific CPU core.
 *
 * Input Parameters:
 *   CoreID - The ID of the CPU core
 *
 * Returned Value:
 *   The current state of the CPU core
 *
 ****************************************************************************/
cpu_state_t up_get_cpu_state(uint32_t CoreID)
{
	return g_cpuhp_flag[CoreID];
}

/****************************************************************************
 * Name: up_cpu_hotplugreq
 *
 * Description:
 *   Check if there is a pending hotplug request for the specified CPU.
 *
 * Input Parameters:
 *   cpu - The CPU index to check
 *
 * Returned Value:
 *   true if there is a pending hotplug request, false otherwise
 *
 ****************************************************************************/
bool up_cpu_hotplugreq(int cpu)
{
	return spin_is_locked(&g_cpu_hotpluged[cpu]);
}

/****************************************************************************
 * Name: up_cpu_hotplugabort
 *
 * Description:
 *   Abort a pending hotplug request for the specified CPU.
 *
 * Input Parameters:
 *   cpu - The CPU index for which to abort the hotplug request
 *
 * Returned Value:
 *   OK (0) on success
 *
 * Assumptions:
 *   Called from within a critical section with proper synchronization
 *
 ****************************************************************************/
int up_cpu_hotplugabort(int cpu)
{
	ASSERT(spin_is_locked(&g_cpu_hotpluged[cpu]) && !spin_is_locked(&g_cpu_hotplughandled[cpu]));

	/* Release the g_cpu_hotpluged spinlock to synchronize with the requesting CPU. */
	spin_unlock(&g_cpu_hotpluged[cpu]);

	return OK;
}

/****************************************************************************
 * Name: arm_hotplug_handler
 *
 * Description:
 *   This is the handler for SGI3.  It performs the following operations:
 *
 *   1. It saves the current task state at the head of the current assigned
 *      task list.
 *   2. CPU goes to low power state (ie. CPU Shutdown)
 *   3. CPU returns from low power, restoring the state of the new task at the
 *      head of the ready to run list. (ie. We assume the SGI to be triggered
 *      by the primary core)
 *
 * Input Parameters:
 *   Standard interrupt handling
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/
int arm_hotplug_handler(int irq, void *context, void *arg)
{
	/* Remark: Currently this handler only handles case which after secondary core
	           shutdown. we will expect primary core to shutdown as well (ie. Not handling tasks anymore)
			   It is because current kernel is not aware of secondary core has shutdown 
			   (ie. TizenRT Kernel logic is not ready to handle such case yet)
			   Thus, we will be facing a deadlock when cpuA fire SGI2 to cpuB (ie. pausing CPU)
	*/

	int cpu = this_cpu();

	/* Prevent duplicate execution: If the IRQ was triggered but already handled
	 * in irq_csection, we should not execute again.
	 */
	if (up_cpu_hotplugreq(cpu)) {
		ASSERT(spin_is_locked(&g_cpu_hotpluged[cpu]) && !spin_is_locked(&g_cpu_hotplughandled[cpu])
			&& up_get_cpu_state(cpu) == CPU_RUNNING);

		/* Set secondary core to hotplug state */
		up_set_cpu_state(cpu, CPU_HOTPLUG);

		/* Notification hotplug is handled */
		spin_lock(&g_cpu_hotplughandled[cpu]);
		/* Release the spinlock to synchronize with the requesting CPU. */
		spin_unlock(&g_cpu_hotpluged[cpu]);

		/* Save the tcb state */
		struct tcb_s *rtcb = this_task();
		arm_savestate(rtcb->xcp.regs);

		/* Mark this cpu's idle task as inactive to prevent the scheduler from putting things on it */
		rtcb->task_state = TSTATE_TASK_INACTIVE;
		CURRENT_REGS = NULL;

		/*
		* clear GIC APR0 as when this gets set during restore by ATF boot flow,
		* When this register is set, it cause GIC to not serve other requests as it thinks there is an
		* active ISR being served currently.
		*
		* On powerup, this register is restored by ATF from previous saved state.
		* If hard reset, then state is re-initialized, otherwise it will save the state when psci_cpu_off
		*/
		putreg32(0x00000000, GIC_ICCNSAPR1);    /* clear SGI1 nonsecure APR0 */
		putreg32(0x00000000, GIC_ICCAPR1);      /* clear SGI1 secure APR0 */

		/*
		* manually clear SGI4 interrupt active
		* As psci_cpu_off is a noreturn function it will trap the CPU in a WFI loop
		* this means that it will never exit back to arm_decodeirq where EOIR will be set
		*/
		putreg32(irq, GIC_ICCEOIR);

		/* Shut down the secondary core */
		up_cpu_die();
	}

	return OK;
}

/****************************************************************************
 * Name: up_cpu_off
 *
 * Description:
 *   Stop and power down a secondary CPU core. This function performs a
 *   complete CPU shutdown sequence by first hot-plugging the CPU and
 *   then powering it down completely. The CPU will be removed from the
 *   SMP system and can be restarted later with up_cpu_on().
 *
 * Input Parameters:
 *   cpu - The index of the CPU to stop (must be > 0 and < CONFIG_SMP_NCPUS)
 *
 * Returned Value:
 *   Zero (OK) on success; a negated errno value on failure.
 *
 * Assumptions:
 *   - Called from a different CPU than the target
 *   - Called from within a critical section
 *   - Target CPU is currently running and can be safely stopped
 *   - Scheduler is in a state that allows CPU hot-plug operations
 *
 ****************************************************************************/

int up_cpu_off(int cpu)
{
	struct tcb_s * tcb;
	int ret = OK;

	smpvdbg("Disabling CPU%d\n", cpu);

	DEBUGASSERT(cpu > 0 && cpu < CONFIG_SMP_NCPUS && cpu != this_cpu());

	/* Check the target cpu idle */
	tcb = current_task(cpu);
	if (tcb->pid != cpu) {
		return -EBUSY;
	}

	/* Lock spinlock to synchronize with the target CPU */
	spin_lock(&g_cpu_hotpluged[cpu]);

	/* Fire SGI for cpu to enter hotplug */
	arm_cpu_sgi(GIC_IRQ_SGI3, (1 << cpu));

	spin_lock(&g_cpu_hotpluged[cpu]);
	spin_unlock(&g_cpu_hotpluged[cpu]);

	/* Check whether the hotplug is handled */
	if (spin_trylock(&g_cpu_hotplughandled[cpu]) == SP_UNLOCKED) {
		/* The cpu hotplug is failed, cpu is in enter_critical_section */
		ASSERT(up_get_cpu_state(cpu) != CPU_HOTPLUG);
		ret = -EBUSY;
	} else {
		/* Hotplug is sucessful */
		ASSERT(up_get_cpu_state(cpu) == CPU_HOTPLUG);
	}

	/* Release the handled spinlock since we've completed the state check */
	spin_unlock(&g_cpu_hotplughandled[cpu]);

	/* Check if hotplug failed */
	if (ret < 0) {
		smpdbg("Secondary core hotplug failed CPU%d\n", cpu);
		return ret;
	}

	smpvdbg("Secondary core hotplug complete CPU%d\n", cpu);

	/* Power down the core completely */
	ret = up_cpu_down(cpu);
	if (ret < 0) {
		smpdbg("Failed to powerdown secondary core CPU%d\n", cpu);
		return ret;
	}

	smpvdbg("Secondary core powerdown complete CPU%d\n", cpu);

	return ret;
}

#endif /* CONFIG_CPU_HOTPLUG */
