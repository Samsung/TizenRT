/****************************************************************************
 * kernel/sched/sched_cpuselect.c
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

#include <sys/types.h>
#include <assert.h>

#include <tinyara/sched.h>

#include "sched/sched.h"

#ifdef CONFIG_SMP
#include "../arch/arm/src/armv7-a/smp.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define IMPOSSIBLE_CPU 0xff
/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name:  sched_evaluate_cpu
 *
 * Description:
 *   Evaluate a CPU for task assignment based on its current task load.
 *   Returns the CPU index if it's running an IDLE task (best case),
 *   otherwise returns -1 to indicate the CPU should be considered for
 *   its task priority.
 *
 * Input Parameters:
 *   cpu_index - The CPU to evaluate
 *   minprio   - Pointer to the current minimum priority found
 *
 * Returned Value:
 *   CPU index if IDLE task found, -1 otherwise
 *
 ****************************************************************************/
static int sched_evaluate_cpu(int cpu_index, uint16_t *minprio)
{
	FAR struct tcb_s *rtcb = (FAR struct tcb_s *)g_assignedtasks[cpu_index].head;

	/* If this CPU is executing its IDLE task, then use it.  The
	 * IDLE task is always the last task in the assigned task list.
	 */

	if (rtcb->flink == NULL) {
		/* The IDLE task should always be assigned to this CPU and have
		 * a priority of zero.
		 */

		DEBUGASSERT(rtcb->sched_priority == 0);
		return cpu_index;
	} else if (rtcb->sched_priority < *minprio) {
		DEBUGASSERT(rtcb->sched_priority > 0);
		*minprio = rtcb->sched_priority;
	}

	return -1;					// No immediate return, but this CPU is a candidate
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name:  sched_select_cpu
 *
 * Description:
 *   Return the index to the CPU with the lowest priority running task,
 *   possibly its IDLE task.
 *
 * Input Parameters:
 *   affinity - The set of CPUs on which the thread is permitted to run.
 *
 * Returned Value:
 *   Index of the CPU with the lowest priority running task
 *
 * Assumptions:
 *   Called from within a critical section.
 *
 ****************************************************************************/
int sched_select_cpu(cpu_set_t affinity)
{
	uint16_t minprio;
	int cpu = IMPOSSIBLE_CPU;
	int i, result;

	minprio = SCHED_PRIORITY_MAX + 1;

	/* First, try to find a CPU that is both in the affinity mask and available */
	for (i = 0; i < CONFIG_SMP_NCPUS; i++) {
		/* Is the thread permitted to run on this CPU and is the CPU available? */
		if ((affinity & (1 << i)) != 0 && !((up_get_cpu_state(i) == CPU_HALTED) || (up_get_cpu_state(i) == CPU_HOTPLUG))) {

			result = sched_evaluate_cpu(i, &minprio);
			if (result >= 0) {
				return result;	// Found IDLE task
			}
			cpu = i;			// Remember best non-IDLE CPU
		}
	}

	/* If we found a suitable CPU from the affinity mask, return it */
	if (cpu != IMPOSSIBLE_CPU) {
		return cpu;
	}

	/* If no CPUs with affinity are available, assign the task to any available CPU */
	for (i = 0; i < CONFIG_SMP_NCPUS; i++) {
		if (!((up_get_cpu_state(i) == CPU_HALTED) || (up_get_cpu_state(i) == CPU_HOTPLUG))) {
			result = sched_evaluate_cpu(i, &minprio);
			if (result >= 0) {
				return result;	// Found IDLE task
			}
			cpu = i;			// Remember best non-IDLE CPU
		}
	}

	DEBUGASSERT(cpu != IMPOSSIBLE_CPU);
	return cpu;
}

#endif							/* CONFIG_SMP */
