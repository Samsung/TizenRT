/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
/************************************************************************
 * kernel/sched/sched_cpuload.c
 *
 *   Copyright (C) 2014 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ************************************************************************/

/************************************************************************
 * Included Files
 ************************************************************************/

#include <tinyara/config.h>

#include <errno.h>
#include <assert.h>

#include <tinyara/clock.h>
#include <arch/irq.h>

#include "sched/sched.h"

#ifdef CONFIG_SCHED_CPULOAD

/************************************************************************
 * Pre-processor Definitions
 ************************************************************************/
/* Are we using the system timer, or an external clock?  Get the rate
 * of the sampling in ticks per second for the selected timer.
 */

#ifdef CONFIG_SCHED_CPULOAD_EXTCLK
#ifndef CONFIG_SCHED_CPULOAD_TICKSPERSEC
#error CONFIG_SCHED_CPULOAD_TICKSPERSEC is not defined
#endif
#define CPULOAD_TICKSPERSEC CONFIG_SCHED_CPULOAD_TICKSPERSEC
#else
#define CPULOAD_TICKSPERSEC CLOCKS_PER_SEC
#endif

/************************************************************************
 * Private Type Declarations
 ************************************************************************/

/************************************************************************
 * Public Variables
 ************************************************************************/

/************************************************************************
 * Private Variables
 ************************************************************************/

/* This is the total number of clock tick counts.  Essentially the
 * 'denominator' for all CPU load calculations.
 */

volatile uint32_t g_cpuload_total[SCHED_NCPULOAD];
volatile uint32_t g_cpuload_timeconstant[SCHED_NCPULOAD] = {
#ifdef CONFIG_SCHED_MULTI_CPULOAD
	CONFIG_SCHED_CPULOAD_TIMECONSTANT_SHORT,
	CONFIG_SCHED_CPULOAD_TIMECONSTANT_MID,
	CONFIG_SCHED_CPULOAD_TIMECONSTANT_LONG
#else
	CONFIG_SCHED_CPULOAD_TIMECONSTANT
#endif
};

/************************************************************************
 * Private Functions
 ************************************************************************/

/************************************************************************
 * Public Functions
 ************************************************************************/

/************************************************************************
 * Name: sched_process_cpuload
 *
 * Description:
 *   Collect data that can be used for CPU load measurements.
 *
 * Inputs:
 *   None
 *
 * Return Value:
 *   None
 *
 * Assumptions/Limitations:
 *   This function is called from a timer interrupt handler with all
 *   interrupts disabled.
 *
 ************************************************************************/

void weak_function sched_process_cpuload(void)
{
	FAR struct tcb_s *rtcb = this_task();
	int hash_index;
	int i;
	int cpuload_idx;

	/* Increment the count on the currently executing thread
	 *
	 * NOTE also that CPU load measurement data is retained in the g_pidhash
	 * table vs. in the TCB which would seem to be the more logic place.  It
	 * is place in the hash table, instead, to facilitate CPU load adjustments
	 * on all threads during timer interrupt handling. sched_foreach() could
	 * do this too, but this would require a little more overhead.
	 */

	hash_index = PIDHASH(rtcb->pid);

	for (cpuload_idx = 0; cpuload_idx < SCHED_NCPULOAD; cpuload_idx++) {
		g_pidhash[hash_index].ticks[cpuload_idx]++;

		/* Increment tick count.  If the accumulated tick value exceed a time
		 * constant, then shift the accumulators.
		 */

		if (++g_cpuload_total[cpuload_idx] > (g_cpuload_timeconstant[cpuload_idx] * CPULOAD_TICKSPERSEC)) {
			uint32_t total = 0;

			/* Divide the tick count for every task by two and recalculate the
			 * total.
			 */
			for (i = 0; i < CONFIG_MAX_TASKS; i++) {
				g_pidhash[i].ticks[cpuload_idx] >>= 1;
				total += g_pidhash[i].ticks[cpuload_idx];
			}

			/* Save the new total. */

			g_cpuload_total[cpuload_idx] = total;
		}
	}
}

/****************************************************************************
 * Function:  clock_cpuload
 *
 * Description:
 *   Return load measurement data for the select PID.
 *
 * Parameters:
 *   pid - The task ID of the thread of interest.  pid == 0 is the IDLE thread.
 *   cpuload - The location to return the CPU load
 *
 * Return Value:
 *   OK (0) on success; a negated errno value on failure.  The only reason
 *   that this function can fail is if 'pid' no longer refers to a valid
 *   thread.
 *
 * Assumptions:
 *
 ****************************************************************************/

int clock_cpuload(int pid, int index, FAR struct cpuload_s *cpuload)
{
	irqstate_t flags;
	int hash_index = PIDHASH(pid);
	int ret = -ESRCH;

	DEBUGASSERT(cpuload && index >= 0 && index < SCHED_NCPULOAD);

	/* Momentarily disable interrupts.  We need (1) the task to stay valid
	 * while we are doing these operations and (2) the tick counts to be
	 * synchronized when read.
	 */

	flags = irqsave();

	/* Make sure that the entry is valid (TCB field is not NULL) and matches
	 * the requested PID.  The first check is needed if the thread has exited.
	 * The second check is needed for the case where the task associated with
	 * the requested PID has exited and the slot has been taken by another
	 * thread with a different PID.
	 *
	 * NOTE also that CPU load measurement data is retained in the g_pidhash
	 * table vs. in the TCB which would seem to be the more logic place.  It
	 * is place in the hash table, instead, to facilitate CPU load adjustments
	 * on all threads during timer interrupt handling. sched_foreach() could
	 * do this too, but this would require a little more overhead.
	 */

	if (g_pidhash[hash_index].tcb && g_pidhash[hash_index].pid == pid) {
		cpuload->total = g_cpuload_total[index];
		cpuload->active = g_pidhash[hash_index].ticks[index];
		ret = OK;
	}

	irqrestore(flags);
	return ret;
}
#endif							/* CONFIG_SCHED_CPULOAD */
