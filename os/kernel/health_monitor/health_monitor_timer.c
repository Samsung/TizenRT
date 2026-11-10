/****************************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
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
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdbool.h>
#include <stdint.h>
#include <tinyara/clock.h>
#include <tinyara/irq.h>
#include "sched/sched.h"
#include "health_monitor/health_monitor.h"

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: health_monitor_trylock
 *
 * Description:
 *   Mask local interrupts and try to acquire the registry lock.
 *
 * Inputs:
 *   flags - Output pointer for the saved interrupt state.
 *
 * Return Value:
 *   True with the lock held; false with the interrupt state restored.
 *
 * Assumptions:
 *   A failed attempt is retried later without refreshing the watchdog.
 *
 ****************************************************************************/

static bool health_monitor_trylock(irqstate_t *flags)
{
	*flags = irqsave();
#ifdef CONFIG_SMP
	if (spin_trylock(&g_health_lock) == SP_LOCKED) {
		irqrestore(*flags);
		return false;
	}
#endif
	return true;
}

/****************************************************************************
 * Name: health_monitor_check
 *
 * Description:
 *   Inspect only the earliest deadline. START/KICK/STOP maintain heap order
 *   in thread context; the ISR never repairs or walks the heap. Confirm a
 *   timeout under the registry lock and handle it after releasing that lock.
 *
 * Assumptions:
 *   Called by CPU0 from the tick ISR. Contention defers to a later tick.
 ****************************************************************************/

static inline void health_monitor_check(clock_t now)
{
	struct tcb_s *tcb;
	clock_t deadline;
	clock_t check_at;
	irqstate_t spin_flags;
	irqstate_t critical_flags;
	struct tcb_s *tcb_addr;
	pid_t pid;
	int status;

	status = health_monitor_get_next_check_time(&check_at);
	if (status < 0) {
		return;
	}
	if (status == 0 || health_monitor_is_tick_before(now, check_at)) {
		return;
	}

	if (!health_monitor_trylock(&spin_flags)) {
		return;
	}

	now = clock_systimer();
	if (g_health_count > 0) {
		tcb = g_health_heap[0];
		deadline = health_monitor_get_state(tcb)->deadline;
		if (!health_monitor_is_tick_before(now, deadline)) {
			pid = tcb->pid;
			tcb_addr = tcb;
			spin_unlock_irqrestore(&g_health_lock, spin_flags);

			critical_flags = enter_critical_section();
			/* The target may have exited after the registry lock was released. */
			tcb = sched_gettcb(pid);
			if (tcb != tcb_addr) {
				tcb = NULL;
			}
			health_monitor_fail_timeout(tcb);
			leave_critical_section(critical_flags);
			return;
		}
	}

	spin_unlock_irqrestore(&g_health_lock, spin_flags);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: health_monitor_timer
 *
 * Description:
 *   Check thread health each tick and report failures synchronously.
 *   Hardware watchdog refresh belongs exclusively to the monitored thread.
 *
 * Assumptions:
 *   Called after the clock update and before global locks. CPU0 only on SMP.
 ****************************************************************************/

void health_monitor_timer(void)
{
	if (!g_health_watchdog_started) {
		return;
	}

	health_monitor_check(clock_systimer());
}
