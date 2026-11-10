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
#include <debug.h>
#include <stdbool.h>
#include <stdint.h>
#include <tinyara/arch.h>
#include <tinyara/clock.h>
#include <tinyara/security_level.h>
#ifdef CONFIG_SYSTEM_REBOOT_REASON
#include <arch/reboot_reason.h>
#endif
#include "sched/sched.h"
#include "task/task.h"
#include "health_monitor/health_monitor.h"

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: health_monitor_task_state_name
 *
 * Description:
 *   Return a readable name for a task state.
 ****************************************************************************/

static const char *health_monitor_task_state_name(uint8_t state)
{
	static const char *const names[NUM_TASK_STATES] = {
		"INVALID", "PENDING", "READYTORUN", "ASSIGNED", "RUNNING", "INACTIVE",
		"WAIT_SEM", "WAIT_FIN", "WAIT_SIG", "WAIT_MQNOTEMPTY", "WAIT_MQNOTFULL", "WAIT_PAGEFILL"
	};

	return state < NUM_TASK_STATES ? names[state] : "UNKNOWN";
}

/****************************************************************************
 * Name: health_monitor_dump
 *
 * Description:
 *   Print the target's live task information in a critical section with
 *   the registry lock released.
 ****************************************************************************/

static void health_monitor_dump(struct tcb_s *tcb, bool dump_stack)
{
	const struct health_monitor_s *state = health_monitor_get_state(tcb);
	uint32_t timeout = 0;
	clock_t deadline = 0;
	clock_t now;
	clock_t last_refresh;
	clock_t gap;
	uint64_t now_ms;
	uint64_t refresh_ms;
	uint64_t deadline_ms;
	bool overdue;
	bool state_protected;
#ifdef CONFIG_SMP
	uint8_t cpu = tcb->cpu;
#else
	uint8_t cpu = 0;
#endif

	lldbg_noarg("\n===========================================================\n");
	lldbg_noarg("Health Monitor %s target\n", dump_stack ? "timeout" : "unexpected-exit");
	lldbg_noarg("===========================================================\n");
#if CONFIG_TASK_NAME_SIZE > 0
	lldbg("Monitored task name=%s pid=%d\n", tcb->name, (int)tcb->pid);
#endif
	lldbg("Monitored task state=%s(%u) priority=%u cpu=%u\n", health_monitor_task_state_name(tcb->task_state), (uint32_t)tcb->task_state, (uint32_t)tcb->sched_priority, (uint32_t)cpu);

#ifdef CONFIG_SMP
	state_protected = spin_trylock(&g_health_lock) != SP_LOCKED;
#else
	state_protected = true;
#endif
	if (state_protected) {
		timeout = state->timeout;
		deadline = state->deadline;
#ifdef CONFIG_SMP
		spin_unlock(&g_health_lock);
#endif
	}
	if (timeout != 0) {
		now = clock_systimer();
		last_refresh = deadline - timeout;
		now_ms = TICK2MSEC((uint64_t)now);
		refresh_ms = TICK2MSEC((uint64_t)last_refresh);
		deadline_ms = TICK2MSEC((uint64_t)deadline);
		overdue = !health_monitor_is_tick_before(now, deadline);
		gap = overdue ? now - deadline : deadline - now;

		lldbg("Monitored task timeout=%u ms last_kick=%u.%03u s (%u ms ago)\n",
			(uint32_t)TICK2MSEC((uint64_t)timeout), (uint32_t)(refresh_ms / MSEC_PER_SEC),
			(uint32_t)(refresh_ms % MSEC_PER_SEC), (uint32_t)TICK2MSEC((uint64_t)(now - last_refresh)));
		lldbg("Monitored task deadline=%u.%03u s (%s %u ms) now=%u.%03u s\n",
			(uint32_t)(deadline_ms / MSEC_PER_SEC), (uint32_t)(deadline_ms % MSEC_PER_SEC),
			overdue ? "overdue" : "remaining", (uint32_t)TICK2MSEC((uint64_t)gap),
			(uint32_t)(now_ms / MSEC_PER_SEC), (uint32_t)(now_ms % MSEC_PER_SEC));
	} else {
		lldbg_noarg("Monitored task timing unavailable; %s\n", state_protected ? "monitoring stopped" : "HM lock busy");
	}
	task_show_tcbinfo(tcb);
	if (dump_stack) {
		lldbg_noarg("===========================================================\n");
		lldbg_noarg("Monitored task stack details\n");
		lldbg_noarg("===========================================================\n");
		lldbg("  base: %08x\n", (uint32_t)(uintptr_t)tcb->adj_stack_ptr);
		lldbg("  size: %08x\n", (uint32_t)tcb->adj_stack_size);
#ifdef CONFIG_STACK_COLORATION
		if (tcb->stack_alloc_ptr && tcb->stack_base_ptr && tcb->adj_stack_size > 0 &&
			(uintptr_t)tcb->stack_base_ptr >= (uintptr_t)tcb->stack_alloc_ptr &&
			(uintptr_t)tcb->stack_base_ptr < (uintptr_t)tcb->adj_stack_ptr) {
			lldbg("  used: %08x\n", (uint32_t)up_check_tcbstack(tcb));
		} else {
			lldbg("  used: unavailable\n");
		}
#endif
		task_dump(tcb);
	} else {
		/* Lifecycle cleanup may already have removed the stack mapping. */
		task_dump_registers(tcb);
	}
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: health_monitor_fail_timeout
 *
 * Description:
 *   Report a confirmed timeout and enter PANIC. The caller holds a critical
 *   section with the registry lock released. A NULL TCB is ignored.
 ****************************************************************************/

void health_monitor_fail_timeout(struct tcb_s *tcb)
{
	if (tcb == NULL) {
		return;
	}

#ifdef CONFIG_SYSTEM_REBOOT_REASON
	up_reboot_reason_write((tcb->flags & TCB_FLAG_TTYPE_MASK) == TCB_FLAG_TTYPE_KERNEL ? REBOOT_SYSTEM_HEALTH_MONITOR_TIMEOUT : REBOOT_USER_HEALTH_MONITOR_TIMEOUT);
#endif
	if (!IS_SECURE_STATE()) {
		lldbg("HEALTH MONITOR TIMEOUT pid=%d uptime=%u ms\n", (int)tcb->pid, (uint32_t)TICK2MSEC((uint64_t)clock_systimer()));
		if ((tcb->flags & TCB_FLAG_EXIT_PROCESSING) == 0) {
			health_monitor_dump(tcb, true);
		} else {
			lldbg_noarg("Monitored task unavailable for detail dump\n");
		}
		lldbg_noarg("===========================================================\n");
		lldbg_noarg("Health Monitor is entering PANIC; following context is the panic caller\n");
	}
	PANIC();
}

/****************************************************************************
 * Name: health_monitor_fail_unexpected_exit
 *
 * Description:
 *   Report an unexpected exit and enter PANIC. The caller holds a critical
 *   section with the registry lock released and keeps the TCB alive.
 ****************************************************************************/

void health_monitor_fail_unexpected_exit(struct tcb_s *tcb)
{
	struct tcb_s *current;
#ifdef CONFIG_SYSTEM_REBOOT_REASON
	up_reboot_reason_write((tcb->flags & TCB_FLAG_TTYPE_MASK) == TCB_FLAG_TTYPE_KERNEL ? REBOOT_SYSTEM_HEALTH_MONITOR_UNEXPECTED_EXIT : REBOOT_USER_HEALTH_MONITOR_UNEXPECTED_EXIT);
#endif

	if (!IS_SECURE_STATE()) {
		current = this_task();
		lldbg("HEALTH MONITOR UNEXPECTED_EXIT pid=%d current_pid=%d\n", (int)tcb->pid, (int)current->pid);
#if CONFIG_TASK_NAME_SIZE > 0
		lldbg("Scheduler current task name=%s\n", current->name);
#endif
		health_monitor_dump(tcb, false);
		lldbg_noarg("===========================================================\n");
		lldbg_noarg("Health Monitor is entering PANIC; following context is the panic caller\n");
	}
	PANIC();
}
