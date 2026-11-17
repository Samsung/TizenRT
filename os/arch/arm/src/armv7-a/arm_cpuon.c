/****************************************************************************
 *
 * Copyright 2023 Samsung Electronics All Rights Reserved.
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
 * arch/arm/src/armv7-a/arm_cpuon.c
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <errno.h>

#include <tinyara/arch.h>
#include <debug.h>
#include <tinyara/sched.h>

#ifdef CONFIG_SCHED_INSTRUMENTATION
#include <tinyara/sched_note.h>
#endif

#ifdef CONFIG_SMP

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_cpu_on
 *
 * Description:
 *   In an SMP configuration, only one CPU is initially active (CPU 0).
 *   System initialization occurs on that single thread.  At the completion
 *   of the initialization of the OS, just before beginning normal
 *   multitasking, the additional CPUs are brought up by sched_cpuon(),
 *   which calls this function.
 *
 *   This is the generic armv7-a part of the bring-up: it validates the
 *   request and delegates the hardware sequence to the board hook
 *   up_cpu_up().  On a board whose cores are already powered and only wait
 *   to be released (imx6: imx_cpu_enable() parks them in arm_cpu_boot()),
 *   up_cpu_up() is simply up_cpu_start(), the CPUSTART SGI.  Where the
 *   secondary core is powered off at cold boot (amebasmart: power domain,
 *   then PSCI CPU_ON) up_cpu_up() performs the power-on, and where the
 *   firmware offers PSCI (qemu) up_cpu_up() is up_cpu_start() issuing that
 *   CPU_ON call.  A PSCI request carries the entry point, so no CPUSTART
 *   SGI is needed after it.
 *
 *   Each CPU is provided the entry point to its IDLE task when started.  A
 *   TCB for each CPU's IDLE task has been initialized and placed in the
 *   CPU's g_assignedtasks[cpu] list.  No stack has been allocated or
 *   initialized.
 *
 *   The OS initialization logic calls this function repeatedly until each
 *   CPU has been brought up, 1 through (CONFIG_SMP_NCPUS-1).
 *
 * Input Parameters:
 *   cpu - The index of the CPU being powered on.  This will be a numeric
 *         value in the range of one to (CONFIG_SMP_NCPUS-1).
 *         (CPU 0 is already active)
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure.
 *
 * Assumptions:
 *   - Called from a different CPU than the target
 *   - The target CPU is in a state where it can be started
 *
 ****************************************************************************/

int up_cpu_on(int cpu)
{
	int ret;

	smpllvdbg("Starting CPU%d\n", cpu);

	if (cpu <= 0 || cpu >= CONFIG_SMP_NCPUS) {
		smplldbg("Invalid CPU index: %d (valid: 1 ~ %d)\n", cpu,
			CONFIG_SMP_NCPUS - 1);
		return -EINVAL;
	}

	if (cpu == this_cpu()) {
		smplldbg("Cannot start current CPU%d from itself\n", cpu);
		return -EINVAL;
	}

	/* Bring the core up through the board hook.  There is deliberately no
	 * weak default for up_cpu_up(): with --whole-archive disabled a weak
	 * default would satisfy the reference here and the linker would
	 * silently drop the board implementation instead of booting without
	 * its secondary core.
	 */

	ret = up_cpu_up(cpu);
	if (ret < 0) {
		smplldbg("Failed to boot the secondary core CPU%d\n", cpu);
		return ret;
	}

#ifdef CONFIG_SCHED_INSTRUMENTATION
	/* Notify of the start event */

	sched_note_cpu_start(this_task(), cpu);
#endif

	return OK;
}

#endif /* CONFIG_SMP */
