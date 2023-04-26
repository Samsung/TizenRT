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
 * kernel/sched/sched_processtimer.c
 *
 *   Copyright (C) 2007, 2009, 2014-2018 Gregory Nutt. All rights reserved.
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
#include <tinyara/compiler.h>
#include <time.h>

#if CONFIG_RR_INTERVAL > 0
#include <sched.h>
#include <tinyara/arch.h>
#include <tinyara/ttrace.h>
#endif

#include "sched/sched.h"
#include "wdog/wdog.h"
#include "clock/clock.h"

/************************************************************************
 * Pre-processor Definitions
 ************************************************************************/

#ifndef CONFIG_SCHED_CPULOAD_TIMECONSTANT
#define CONFIG_SCHED_CPULOAD_TIMECONSTANT 2
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

/************************************************************************
 * Private Functions
 ************************************************************************/

/************************************************************************
 * Name:  sched_process_timeslice
 *
 * Description:
 *   Check if the currently executing task has exceeded its time slice.
 *
 * Inputs:
 *   None
 *
 * Return Value:
 *   None
 *
 ************************************************************************/

#if CONFIG_RR_INTERVAL > 0
static inline void sched_process_timeslice(void)
{
//PORTNOTE: In the counterpart for this function in Nuttx -> nxsched_process_scheduler
//if SMP is enabled then the below work is done for all the CPUs instead of just one.

#ifdef CONFIG_SMP
	irqstate_t flags = enter_critical_section();

	for (int i = 0; i < CONFIG_SMP_NCPUS; i++) {
#endif

		FAR struct tcb_s *rtcb = this_task();

		/* Check if the currently executing task uses round robin
		 * scheduling.
		 */

		if ((rtcb->flags & TCB_FLAG_ROUND_ROBIN) != 0) {
			/* Yes, check if decrementing the timeslice counter
			 * would cause the timeslice to expire
			 */

			if (rtcb->timeslice <= 1) {
				/* Yes, Now check if the task has pre-emption disabled.
				 * If so, then we will freeze the timeslice count at
				 * the value until the next tick after pre-emption
				 * has been enabled.
				 */

				if (!rtcb->lockcount) {
					/* Reset the timeslice in any case. */

					rtcb->timeslice = MSEC2TICK(CONFIG_RR_INTERVAL);

					/* We know we are at the head of the ready to run
					 * prioritized list.  We must be the highest priority
					 * task eligible for execution.  Check the next task
					 * in the ready to run list.  If it is the same
					 * priority, then we need to relinquish the CPU and
					 * give that task a shot.
					 */

					if (rtcb->flink && rtcb->flink->sched_priority >= rtcb->sched_priority) {
						/* Just resetting the task priority to its current
						 * value.  This this will cause the task to be
						 * rescheduled behind any other tasks at the same
						 * priority.
						 */

						up_reprioritize_rtr(rtcb, rtcb->sched_priority);
					}
				}
			} else {
				/* Decrement the timeslice counter */

				rtcb->timeslice--;
			}
		}
#ifdef CONFIG_SMP
	}

	leave_critical_section(flags);
#endif
}
#else
#define sched_process_timeslice()
#endif

/************************************************************************
 * Public Functions
 ************************************************************************/

/************************************************************************
 * System Timer Hooks
 *
 * These are standard interfaces that are exported by the OS
 * for use by the architecture specific logic
 *
 ************************************************************************/

/************************************************************************
 * Name:  sched_process_timer
 *
 * Description:
 *   This function handles system timer events.
 *   The timer interrupt logic itself is implemented in the
 *   architecture specific code, but must call the following OS
 *   function periodically -- the calling interval must be
 *   USEC_PER_TICK
 *
 * Inputs:
 *   None
 *
 * Return Value:
 *   None
 *
 ************************************************************************/

void sched_process_timer(void)
{
#ifdef CONFIG_WATCHDOG_FOR_IRQ
	up_wdog_keepalive();
#endif
	/* Increment the system time (if in the link) */

#ifdef CONFIG_HAVE_WEAKFUNCTIONS
	if (clock_timer != NULL)
#endif
	{
		clock_timer();
	}
#if defined(CONFIG_SCHED_CPULOAD) && !defined(CONFIG_SCHED_CPULOAD_EXTCLK)
	/* Perform CPU load measurements (before any timer-initiated context
	 * switches can occur)
	 */

#ifdef CONFIG_HAVE_WEAKFUNCTIONS
	if (sched_process_cpuload != NULL)
#endif
	{
		sched_process_cpuload();
	}
#endif

	/* Check if the currently executing task has exceeded its
	 * timeslice.
	 */

	sched_process_timeslice();

	/* Process watchdogs */

//PORTNOTE: In the function nxsched_process_wdtimer in Nuttx, the call is directly
//transferred to wd_timer() if SMP is disabled and it is transferred after enter
//_critical_section if SMP is enabled. Here we have a few more lines of code so
//we need to see if only wd_timer() should be wrapped between enter and leave critical
//section or all the LOC should be wrapped.
#ifdef CONFIG_SMP
	irqstate_t flags = enter_critical_section();
#endif
	wd_timer();
#ifdef CONFIG_SMP
	leave_critical_section(flags);
#endif
}
