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
#include <tinyara/irq.h>
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
 * Name: health_monitor_ticks_to_ms
 *
 * Description:
 *   Convert a tick count to milliseconds for diagnostics.
 ****************************************************************************/

static uint64_t health_monitor_ticks_to_ms(uint64_t ticks)
{
	return ticks * USEC_PER_TICK / USEC_PER_MSEC;
}

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
 *   Print captured metadata, then stabilize and dump the live target.
 *   Called without the registry lock, immediately before PANIC. The critical
 *   section and paused CPUs are deliberately retained for the fatal path.
 ****************************************************************************/

static void health_monitor_dump(const struct health_monitor_snapshot_s *expired, bool dump_stack)
{
	struct tcb_s *tcb;
	clock_t now = clock_systimer();
	clock_t last_refresh = expired->deadline - expired->timeout;
	uint64_t now_ms = health_monitor_ticks_to_ms(now);
	uint64_t refresh_ms = health_monitor_ticks_to_ms(last_refresh);
	uint64_t deadline_ms = health_monitor_ticks_to_ms(expired->deadline);
	bool overdue = !health_monitor_is_tick_before(now, expired->deadline);
	clock_t gap = overdue ? now - expired->deadline : expired->deadline - now;

	if (IS_SECURE_STATE()) {
		return;
	}
	lldbg_noarg("\n===========================================================\n");
	lldbg_noarg("Health Monitor %s target\n", dump_stack ? "timeout" : "unexpected-exit");
	lldbg_noarg("===========================================================\n");
#if CONFIG_TASK_NAME_SIZE > 0
	lldbg("Monitored task name=%s pid=%d\n", expired->name, (int)expired->pid);
#endif
	lldbg("Monitored task state=%s(%u) priority=%u cpu=%u\n", health_monitor_task_state_name(expired->task_state),
		(uint32_t)expired->task_state, (uint32_t)expired->priority, (uint32_t)expired->cpu);
	lldbg("Monitored task timeout=%u ms last_kick=%u.%03u s (%u ms ago)\n",
		(uint32_t)health_monitor_ticks_to_ms(expired->timeout), (uint32_t)(refresh_ms / MSEC_PER_SEC),
		(uint32_t)(refresh_ms % MSEC_PER_SEC), (uint32_t)health_monitor_ticks_to_ms(now - last_refresh));
	lldbg("Monitored task deadline=%u.%03u s (%s %u ms) now=%u.%03u s\n", (uint32_t)(deadline_ms / MSEC_PER_SEC),
		(uint32_t)(deadline_ms % MSEC_PER_SEC), overdue ? "overdue" : "remaining",
		(uint32_t)health_monitor_ticks_to_ms(gap), (uint32_t)(now_ms / MSEC_PER_SEC),
		(uint32_t)(now_ms % MSEC_PER_SEC));

	/* Use the existing ASSERT lock order, after releasing the HM lock. */
	(void)enter_critical_section();
#ifdef CONFIG_SMP
	up_cpu_pause_all();
#endif
	tcb = sched_gettcb(expired->pid);
	if (tcb == NULL || (uintptr_t)tcb != expired->tcb_address) {
		lldbg("Monitored task is no longer available; detail dump skipped\n");
		return;
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
 * Name: health_monitor_fail
 *
 * Description:
 *   Report a captured failure and enter the existing panic path. Both timeout
 *   and lifecycle callers must release the HM lock before calling this helper.
 ****************************************************************************/

void health_monitor_fail(const struct health_monitor_failure_s *failure)
{
	struct tcb_s *current;
	bool timeout = failure->type == HEALTH_MONITOR_FAILURE_TIMEOUT;
#ifdef CONFIG_SYSTEM_REBOOT_REASON
	reboot_reason_code_t reason;

	if (timeout) {
		reason = failure->task.kernel_task ? REBOOT_SYSTEM_HEALTH_MONITOR_TIMEOUT : REBOOT_USER_HEALTH_MONITOR_TIMEOUT;
	} else {
		reason = failure->task.kernel_task ? REBOOT_SYSTEM_HEALTH_MONITOR_UNEXPECTED_EXIT : REBOOT_USER_HEALTH_MONITOR_UNEXPECTED_EXIT;
	}
	up_reboot_reason_write(reason);
#endif

	if (timeout) {
		lldbg("HEALTH MONITOR TIMEOUT pid=%d overdue=%u ms uptime=%u ms\n", (int)failure->task.pid,
			(uint32_t)health_monitor_ticks_to_ms(failure->detected_at - failure->task.deadline),
			(uint32_t)health_monitor_ticks_to_ms(failure->detected_at));
	} else {
		current = this_task();

		lldbg("HEALTH MONITOR UNEXPECTED_EXIT pid=%d current_pid=%d\n", (int)failure->task.pid, (int)current->pid);
#if CONFIG_TASK_NAME_SIZE > 0
		if (!IS_SECURE_STATE()) {
			lldbg("Scheduler current task name=%s\n", current->name);
		}
#endif
	}
	health_monitor_dump(&failure->task, timeout);
	if (!IS_SECURE_STATE()) {
		lldbg_noarg("===========================================================\n");
		lldbg_noarg("Health Monitor is entering PANIC; following context is the panic caller\n");
	}
	PANIC();
}
