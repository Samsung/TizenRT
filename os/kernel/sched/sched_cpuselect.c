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

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define IMPOSSIBLE_CPU 0xff
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
 *   Assumes g_active_cpus_mask is maintained globally, representing CPUs that are
 *   currently active (not halted or hotplug).
 *
 ****************************************************************************/
int sched_select_cpu(cpu_set_t affinity)
{
	uint16_t minprio;
	int cpu = IMPOSSIBLE_CPU;
	int i;

	// Create a mask of CPUs that are both in affinity and active
	cpu_set_t eligible_cpus;
	CPU_AND(&eligible_cpus, &affinity, &g_active_cpus_mask);

	// If no CPUs are both in affinity and active, use all active CPUs
	if (eligible_cpus == 0) {
		eligible_cpus = g_active_cpus_mask;
	}

	minprio = SCHED_PRIORITY_MAX + 1;

	/* Single loop through eligible CPUs only */
	for (i = 0; i < CONFIG_SMP_NCPUS; i++) {
		/* Is this CPU eligible? */
		if ((eligible_cpus & (1 << i)) != 0) {
			/* Evaluate this CPU for task assignment */
			FAR struct tcb_s *rtcb = (FAR struct tcb_s *)g_assignedtasks[i].head;

			/* If this CPU is executing its IDLE task, then use it.  The
			 * IDLE task is always the last task in the assigned task list.
			 */
			if (rtcb->flink == NULL) {
				/* The IDLE task should always be assigned to this CPU and have
				 * a priority of zero.
				 */
				DEBUGASSERT(rtcb->sched_priority == 0);
				return i;		// Found CPU running only IDLE task
			} else if (rtcb->sched_priority < minprio) {
				DEBUGASSERT(rtcb->sched_priority > 0);
				minprio = rtcb->sched_priority;
				cpu = i;		// Remember best non-IDLE CPU
			}
		}
	}

	DEBUGASSERT(cpu != IMPOSSIBLE_CPU);
	return cpu;
}

#endif							/* CONFIG_SMP */
