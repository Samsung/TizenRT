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

#include <assert.h>
#include <debug.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <tinyara/arch.h>
#include <tinyara/clock.h>
#include <tinyara/irq.h>
#include <tinyara/sched.h>
#include <tinyara/security_level.h>
#include <tinyara/spinlock.h>
#ifdef CONFIG_SYSTEM_REBOOT_REASON
#include <arch/reboot_reason.h>
#endif

#include "sched/sched.h"
#include "task/task.h"
#include "health_monitor/health_monitor.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Keep the timeout range valid for both 32-bit and 64-bit system clocks. */
#define HEALTH_MONITOR_MAX_TIMEOUT_TICKS INT32_MAX

/* Refresh once per second, expressed in system ticks. */

#define HEALTH_MONITOR_WATCHDOG_REFRESH_TICKS SEC2TICK(1)

/****************************************************************************
 * Global Variables
 ****************************************************************************/

static struct health_monitor_entry_s g_health_heap[HEALTH_MONITOR_HEAP_CAPACITY];
static uint32_t g_health_count;
#ifdef CONFIG_SMP
static spinlock_t g_health_lock = SP_UNLOCKED;
#endif

static volatile uint32_t g_health_sequence;
static volatile uint32_t g_health_next_count;
static volatile clock_t g_health_next_tick;

/* Protect startup with the HM lock. CPU0 caches readiness once and owns the
 * refresh schedule afterwards, without taking the lock on the normal path.
 */

static bool g_health_watchdog_started;
static bool g_health_watchdog_ready;
static clock_t g_health_watchdog_next_refresh;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static struct health_monitor_s *health_monitor_get_state(struct tcb_s *tcb);

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
 * Name: health_monitor_snapshot
 *
 * Description:
 *   Capture target metadata while the registry lock protects its HM state.
 ****************************************************************************/

static void health_monitor_snapshot(struct tcb_s *tcb, struct health_monitor_snapshot_s *info)
{
	info->tcb_address = (uintptr_t)tcb;
	info->pid = tcb->pid;
	info->deadline = health_monitor_get_state(tcb)->deadline;
	info->timeout = health_monitor_get_state(tcb)->timeout;
	info->task_state = tcb->task_state;
	info->priority = tcb->sched_priority;
	info->kernel_task = (tcb->flags & TCB_FLAG_TTYPE_MASK) == TCB_FLAG_TTYPE_KERNEL;
#ifdef CONFIG_SMP
	info->cpu = tcb->cpu;
#else
	info->cpu = 0;
#endif
#if CONFIG_TASK_NAME_SIZE > 0
	memcpy(info->name, tcb->name, sizeof(info->name));
	info->name[CONFIG_TASK_NAME_SIZE] = '\0';
#endif
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
#if CONFIG_TASK_NAME_SIZE > 0
	lldbg("Monitored task name=%s pid=%d\n", expired->name, (int)expired->pid);
#endif
	lldbg("Monitored task state=%s(%u) priority=%u cpu=%u\n", health_monitor_task_state_name(expired->task_state),
		(unsigned int)expired->task_state, (unsigned int)expired->priority, (unsigned int)expired->cpu);
	lldbg("Monitored task timeout=%lu ms last_kick=%lu.%03lu s (%lu ms ago)\n",
		(unsigned long)health_monitor_ticks_to_ms(expired->timeout), (unsigned long)(refresh_ms / MSEC_PER_SEC),
		(unsigned long)(refresh_ms % MSEC_PER_SEC), (unsigned long)health_monitor_ticks_to_ms(now - last_refresh));
	lldbg("Monitored task deadline=%lu.%03lu s (%s %lu ms) now=%lu.%03lu s\n", (unsigned long)(deadline_ms / MSEC_PER_SEC),
		(unsigned long)(deadline_ms % MSEC_PER_SEC), overdue ? "overdue" : "remaining",
		(unsigned long)health_monitor_ticks_to_ms(gap), (unsigned long)(now_ms / MSEC_PER_SEC),
		(unsigned long)(now_ms % MSEC_PER_SEC));

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
		up_dump_task(tcb);
	} else {
		/* Lifecycle cleanup may already have removed the stack mapping. */
		up_dump_task_registers(tcb);
	}
}

/****************************************************************************
 * Name: health_monitor_fail
 *
 * Description:
 *   Report a captured failure and enter the existing panic path. Both timeout
 *   and lifecycle callers must release the HM lock before calling this helper.
 ****************************************************************************/

static void health_monitor_fail(const struct health_monitor_failure_s *failure)
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
		lldbg("HEALTH MONITOR TIMEOUT pid=%d overdue=%lu ms uptime=%lu ms\n", (int)failure->task.pid,
			(unsigned long)health_monitor_ticks_to_ms(failure->detected_at - failure->task.deadline),
			(unsigned long)health_monitor_ticks_to_ms(failure->detected_at));
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
	PANIC();
}

/****************************************************************************
 * Name: health_monitor_lock
 *
 * Description:
 *   Mask local interrupts and acquire the health monitor lock.
 *
 * Inputs:
 *   None.
 *
 * Return Value:
 *   The saved interrupt state, to be passed to health_monitor_unlock().
 *
 * Assumptions:
 *   Called from thread or lifecycle context. Callers must not acquire the
 *   scheduler lock while holding this lock.
 *
 ****************************************************************************/

static irqstate_t health_monitor_lock(void)
{
	irqstate_t flags;

	flags = irqsave();
#ifdef CONFIG_SMP
	spin_lock(&g_health_lock);
#endif

	return flags;
}

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
 *   Called by the timer path. A failed attempt is deferred to a later tick.
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
 * Name: health_monitor_unlock
 *
 * Description:
 *   Release the health monitor lock and restore the caller's interrupt state.
 *
 * Inputs:
 *   flags - Interrupt state saved by lock or a successful trylock.
 *
 * Return Value:
 *   None.
 *
 * Assumptions:
 *   The caller previously acquired the lock with lock or trylock.
 *
 ****************************************************************************/

static void health_monitor_unlock(irqstate_t flags)
{
#ifdef CONFIG_SMP
	spin_unlock(&g_health_lock);
#endif
	irqrestore(flags);
}

/****************************************************************************
 * Name: health_monitor_publish
 *
 * Description:
 *   Publish the heap root as a lockless sequence/count/tick snapshot.
 *
 * Inputs:
 *   None.
 *
 * Return Value:
 *   None.
 *
 * Assumptions:
 *   Caller holds the health monitor lock.
 *
 ****************************************************************************/

static void health_monitor_publish(void)
{
	uint32_t sequence = g_health_sequence;
	clock_t next = g_health_count ? g_health_heap[0].check_at : 0;

	/* Mark sequence odd to indicate an in-progress update. */
	g_health_sequence = sequence + 1;

	g_health_next_count = g_health_count;
	g_health_next_tick = next;

	/* Mark sequence even after writing the payload. */
	g_health_sequence = sequence + 2;
}

/****************************************************************************
 * Name: health_monitor_earlier
 *
 * Description:
 *   Return whether lhs is an earlier reservation than rhs relative to now.
 *
 * Inputs:
 *   lhs  - First reservation tick.
 *   rhs  - Second reservation tick.
 *   now  - Reference tick for comparison.
 *
 * Return Value:
 *   True if lhs is earlier than rhs; false otherwise.
 *
 * Assumptions:
 *   Ticks must be within the supported half-range of now.
 *
 ****************************************************************************/

static bool health_monitor_earlier(clock_t lhs, clock_t rhs, clock_t now)
{
#ifdef CONFIG_SYSTEM_TIME64
	return (int64_t)(lhs - now) < (int64_t)(rhs - now);
#else
	return (int32_t)(lhs - now) < (int32_t)(rhs - now);
#endif
}

/****************************************************************************
 * Name: health_monitor_sift_up
 *
 * Description:
 *   Restore min-heap order by moving an entry toward the root.
 *
 * Inputs:
 *   index - Heap index to move toward the root.
 *   now   - Reference tick for comparison.
 *
 * Return Value:
 *   None.
 *
 * Assumptions:
 *   Caller holds the health monitor lock.
 *
 ****************************************************************************/

static void health_monitor_sift_up(uint32_t index, clock_t now)
{
	struct health_monitor_entry_s entry = g_health_heap[index];
	uint32_t parent;

	while (index > 0) {
		parent = (index - 1) / 2;
		if (!health_monitor_earlier(entry.check_at, g_health_heap[parent].check_at, now)) {
			break;
		}

		g_health_heap[index] = g_health_heap[parent];
		index = parent;
	}

	g_health_heap[index] = entry;
}

/****************************************************************************
 * Name: health_monitor_sift_down
 *
 * Description:
 *   Restore min-heap order by moving an entry toward the leaves.
 *
 * Inputs:
 *   index - Heap index to move toward the leaves.
 *   now   - Reference tick for comparison.
 *
 * Return Value:
 *   None.
 *
 * Assumptions:
 *   Caller holds the health monitor lock.
 *
 ****************************************************************************/

static void health_monitor_sift_down(uint32_t index, clock_t now)
{
	struct health_monitor_entry_s entry = g_health_heap[index];
	uint32_t child;

	while ((child = index * 2 + 1) < g_health_count) {
		if (child + 1 < g_health_count && health_monitor_earlier(g_health_heap[child + 1].check_at, g_health_heap[child].check_at, now)) {
			child++;
		}

		if (!health_monitor_earlier(g_health_heap[child].check_at, entry.check_at, now)) {
			break;
		}

		g_health_heap[index] = g_health_heap[child];
		index = child;
	}

	g_health_heap[index] = entry;
}

/****************************************************************************
 * Name: health_monitor_remove
 *
 * Description:
 *   Remove an entry from the heap and restore min-heap order.
 *
 * Inputs:
 *   index - Heap slot to remove.
 *   now   - Reference tick for comparison.
 *
 * Return Value:
 *   None.
 *
 * Assumptions:
 *   Caller holds the health monitor lock, index < g_health_count.
 *
 ****************************************************************************/

static void health_monitor_remove(uint32_t index, clock_t now)
{
	g_health_count--;
	if (index < g_health_count) {
		g_health_heap[index] = g_health_heap[g_health_count];
		if (index > 0 && health_monitor_earlier(g_health_heap[index].check_at, g_health_heap[(index - 1) / 2].check_at, now)) {
			health_monitor_sift_up(index, now);
		} else {
			health_monitor_sift_down(index, now);
		}
	}

	g_health_heap[g_health_count].tcb = NULL;
	g_health_heap[g_health_count].check_at = 0;
}

/****************************************************************************
 * Name: health_monitor_unregister
 *
 * Description:
 *   Unregister a target TCB, remove its heap entry, and publish the new hint.
 *
 * Inputs:
 *   tcb - Target TCB to unregister.
 *
 * Return Value:
 *   OK if the target was registered and removed; -ENOENT if it was not
 *   registered.
 *
 * Assumptions:
 *   Caller holds the health monitor lock and target TCB remains alive.
 *
 ****************************************************************************/

static int health_monitor_unregister(struct tcb_s *tcb)
{
	struct health_monitor_s *state = health_monitor_get_state(tcb);
	uint32_t index;

	if (state->timeout == 0) {
		return -ENOENT;
	}

	for (index = 0; index < g_health_count; index++) {
		if (g_health_heap[index].tcb == tcb) {
			break;
		}
	}

	DEBUGASSERT(index < g_health_count);
	if (index < g_health_count) {
		health_monitor_remove(index, clock_systimer());
	}

	state->timeout = 0;
	state->deadline = 0;
	health_monitor_publish();
	return OK;
}

/****************************************************************************
 * Name: health_monitor_get_state
 *
 * Description:
 *   Return the health monitor state embedded in a TCB.
 *
 * Inputs:
 *   tcb - Target TCB.
 *
 * Return Value:
 *   Pointer to the TCB's health monitor state.
 *
 * Assumptions:
 *   Caller must protect TCB lifetime and serialize access to its state.
 *
 ****************************************************************************/

static struct health_monitor_s *health_monitor_get_state(struct tcb_s *tcb)
{
	DEBUGASSERT(tcb != NULL);
	return &tcb->health_monitor;
}

/****************************************************************************
 * Name: health_monitor_ms_to_ticks
 *
 * Description:
 *   Convert a timeout in milliseconds to ticks, rounding up.
 *
 * Inputs:
 *   timeout_ms - Timeout in milliseconds.
 *
 * Return Value:
 *   Timeout in ticks, or 0 if timeout_ms is invalid or out of range.
 *
 ****************************************************************************/

static uint32_t health_monitor_ms_to_ticks(uint32_t timeout_ms)
{
	uint64_t ticks;

	if (timeout_ms < HEALTH_MONITOR_MIN_TIMEOUT_MS) {
		return 0;
	}

	ticks = ((uint64_t)timeout_ms * USEC_PER_MSEC + USEC_PER_TICK - 1) / USEC_PER_TICK;
	if (ticks > HEALTH_MONITOR_MAX_TIMEOUT_TICKS) {
		return 0;
	}

	return (uint32_t)ticks;
}

/****************************************************************************
 * Name: health_monitor_check
 *
 * Description:
 *   Inspect due reservations and repair kicked deadlines. The normal path
 *   returns without taking the HM lock. Capture timeout details under the lock
 *   and return without any diagnostics or watchdog I/O.
 *
 * Assumptions:
 *   Called by CPU0 from the tick ISR. Contention defers to a later tick.
 ****************************************************************************/

static enum health_monitor_check_result_e health_monitor_check(clock_t now, struct health_monitor_failure_s *failure)
{
	struct tcb_s *tcb;
	clock_t deadline;
	clock_t check_at;
	irqstate_t flags;
	bool changed = false;
	int status;
	enum health_monitor_check_result_e result = HEALTH_MONITOR_CHECK_OK;

	status = health_monitor_get_next_check_time(&check_at);
	if (status < 0) {
		return HEALTH_MONITOR_CHECK_DEFERRED;
	}
	if (status == 0 || health_monitor_is_tick_before(now, check_at)) {
		return HEALTH_MONITOR_CHECK_OK;
	}

	if (!health_monitor_trylock(&flags)) {
		return HEALTH_MONITOR_CHECK_DEFERRED;
	}

	now = clock_systimer();
	while (g_health_count > 0 && !health_monitor_is_tick_before(now, g_health_heap[0].check_at)) {
		deadline = health_monitor_get_state(g_health_heap[0].tcb)->deadline;

		if (!health_monitor_is_tick_before(now, deadline)) {
			tcb = g_health_heap[0].tcb;

			result = HEALTH_MONITOR_CHECK_TIMEOUT;
			failure->type = HEALTH_MONITOR_FAILURE_TIMEOUT;
			failure->detected_at = now;
			health_monitor_snapshot(tcb, &failure->task);
			break;
		}

		g_health_heap[0].check_at = deadline;
		health_monitor_sift_down(0, now);
		changed = true;
	}

	if (changed) {
		health_monitor_publish();
	}
	health_monitor_unlock(flags);
	return result;
}

/****************************************************************************
 * Name: health_monitor_watchdog_maybe_refresh
 *
 * Description:
 *   Refresh only after a successful check and once the interval has elapsed.
 *   CPU0 owns this schedule after initialization; no HM lock is needed.
 ****************************************************************************/

static void health_monitor_watchdog_maybe_refresh(void)
{
	clock_t now = clock_systimer();

	if (health_monitor_is_tick_before(now, g_health_watchdog_next_refresh)) {
		return;
	}

	up_wdog_keepalive();
	g_health_watchdog_next_refresh = clock_systimer() + HEALTH_MONITOR_WATCHDOG_REFRESH_TICKS;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: health_monitor_is_tick_before
 *
 * Description:
 *   Return whether lhs tick is before rhs tick.
 *
 * Inputs:
 *   lhs - First tick.
 *   rhs - Second tick.
 *
 * Return Value:
 *   True if lhs is before rhs; false otherwise.
 *
 * Assumptions:
 *   Separation must be less than half the clock range.
 *
 ****************************************************************************/

bool health_monitor_is_tick_before(clock_t lhs, clock_t rhs)
{
#ifdef CONFIG_SYSTEM_TIME64
	return (int64_t)(lhs - rhs) < 0;
#else
	return (int32_t)(lhs - rhs) < 0;
#endif
}

/****************************************************************************
 * Name: health_monitor_init_task
 *
 * Description:
 *   Initialize a newly created TCB to the unregistered state. Does not start
 *   monitoring.
 *
 * Inputs:
 *   tcb - TCB of the newly created task or pthread.
 *
 * Return Value:
 *   None.
 *
 * Assumptions:
 *   Called during task creation before activation. Only for new, unregistered
 *   TCBs.
 *
 ****************************************************************************/

void health_monitor_init_task(struct tcb_s *tcb)
{
	struct health_monitor_s *state;

	state = health_monitor_get_state(tcb);
	state->deadline = 0;
	state->timeout = 0;
}

/****************************************************************************
 * Name: health_monitor_start
 *
 * Description:
 *   Register the calling thread for health monitoring.
 *
 * Inputs:
 *   timeout_ms - Requested timeout in milliseconds.
 *
 * Return Value:
 *   OK on success; -EINVAL if timeout is invalid; -EEXIST if already registered;
 *   -ENOSPC if the heap is full.
 *
 * Assumptions:
 *   Called from thread context.
 *
 ****************************************************************************/

int health_monitor_start(uint32_t timeout_ms)
{
	struct health_monitor_s *state;
	struct tcb_s *tcb;
	irqstate_t flags;
	uint32_t index;
	uint32_t timeout;
	clock_t now;
	int ret = OK;

	timeout = health_monitor_ms_to_ticks(timeout_ms);
	if (timeout == 0) {
		return -EINVAL;
	}

	flags = health_monitor_lock();

	tcb = this_task();
	state = health_monitor_get_state(tcb);
	if (state->timeout != 0) {
		ret = -EEXIST;
	} else if (g_health_count >= HEALTH_MONITOR_HEAP_CAPACITY) {
		ret = -ENOSPC;
	} else {
		now = clock_systimer();
		state->timeout = timeout;
		state->deadline = now + timeout;
		index = g_health_count++;
		g_health_heap[index].tcb = tcb;
		g_health_heap[index].check_at = state->deadline;
		health_monitor_sift_up(index, now);
		health_monitor_publish();
	}

	health_monitor_unlock(flags);
	return ret;
}

/****************************************************************************
 * Name: health_monitor_kick
 *
 * Description:
 *   Renew the calling thread's deadline. No-op if unregistered.
 *
 * Inputs:
 *   None.
 *
 * Return Value:
 *   None.
 *
 * Assumptions:
 *   Called from thread context.
 *
 ****************************************************************************/

void health_monitor_kick(void)
{
	struct health_monitor_s *state;
	struct tcb_s *tcb;
	irqstate_t flags;

	flags = health_monitor_lock();

	tcb = this_task();
	state = health_monitor_get_state(tcb);
	if (state->timeout != 0) {
		state->deadline = clock_systimer() + state->timeout;
	}

	health_monitor_unlock(flags);
}

/****************************************************************************
 * Name: health_monitor_stop
 *
 * Description:
 *   Unregister the calling thread from health monitoring.
 *
 * Inputs:
 *   None.
 *
 * Return Value:
 *   OK if registered and removed; -ENOENT if not registered.
 *
 * Assumptions:
 *   Called from thread context.
 *
 ****************************************************************************/

int health_monitor_stop(void)
{
	struct tcb_s *tcb;
	irqstate_t flags;
	int ret;

	flags = health_monitor_lock();

	tcb = this_task();
	ret = health_monitor_unregister(tcb);

	health_monitor_unlock(flags);
	return ret;
}

/****************************************************************************
 * Name: health_monitor_deinit_task
 *
 * Description:
 *   Require STOP before a supplied TCB is torn down or restarted. A registered
 *   target is fatal; stopped and partially created targets need no HM cleanup.
 *   Lifecycle callbacks may STOP before reaching this common boundary.
 *
 *   The caller must keep the TCB alive and prevent re-registration. Capture
 *   the active verdict under the HM lock, then release it before diagnostics.
 *   Earlier cleanup may have changed task state or removed stack mappings.
 *
 * Inputs:
 *   tcb - Target TCB. NULL is ignored.
 *
 * Return Value:
 *   None. An active target is diagnosed and enters PANIC.
 *
 ****************************************************************************/

void health_monitor_deinit_task(struct tcb_s *tcb)
{
	struct health_monitor_failure_s failure;
	irqstate_t flags;
	bool active;

	if (tcb == NULL) {
		return;
	}

	flags = health_monitor_lock();
	active = health_monitor_get_state(tcb)->timeout != 0;
	if (active) {
		failure.type = HEALTH_MONITOR_FAILURE_UNEXPECTED_EXIT;
		failure.detected_at = clock_systimer();
		health_monitor_snapshot(tcb, &failure.task);
	}
	health_monitor_unlock(flags);
	if (!active) {
		return;
	}

	health_monitor_fail(&failure);
}

/****************************************************************************
 * Name: health_monitor_get_next_check_time
 *
 * Description:
 *   Read the published earliest reservation tick via lockless snapshot.
 *
 * Inputs:
 *   check_at - Output pointer for earliest reservation tick. Must not be NULL.
 *
 * Return Value:
 *   1 and write check_at if reservations exist; 0 if empty; -EAGAIN if a
 *   concurrent update was detected.
 *   Leaves output unchanged on 0/-EAGAIN.
 *
 * Assumptions:
 *   Returned tick is a hint; caller must not assume expiry without inspection.
 *
 ****************************************************************************/

int health_monitor_get_next_check_time(clock_t *check_at)
{
	uint32_t sequence;
	uint32_t count;
	clock_t next;
#ifdef CONFIG_SYSTEM_TIME64
	const volatile uint32_t *addr;
	uint32_t high;
	uint32_t low;
	uint32_t high_check;
#endif

	DEBUGASSERT(check_at != NULL);

	/* Reject snapshot if writer is currently updating (odd sequence). */
	sequence = g_health_sequence;
	if (sequence & 1) {
		return -EAGAIN;
	}

	count = g_health_next_count;
#ifdef CONFIG_SYSTEM_TIME64
	/* Sample high/low/high */
	addr = (const volatile uint32_t *)&g_health_next_tick;
	high = addr[1];
	low = addr[0];
	high_check = addr[1];
	if (high != high_check) {
		return -EAGAIN;
	}
	next = ((clock_t)high << 32) | low;
#else
	next = g_health_next_tick;
#endif

	/* Check whether the sequence changed during the read. */
	if (sequence != g_health_sequence) {
		return -EAGAIN;
	}

	if (count == 0) {
		return 0;
	}

	*check_at = next;
	return 1;
}

/****************************************************************************
 * Name: health_monitor_initialize
 *
 * Description:
 *   Start the hardware watchdog once during bring-up, then publish its refresh
 *   schedule under the HM lock. Hardware operations run without the lock.
 ****************************************************************************/

void health_monitor_initialize(void)
{
	irqstate_t flags;

	up_wdog_init(CONFIG_HEALTH_MONITOR_WATCHDOG_TIMEOUT_MS);
	up_wdog_start();
	flags = health_monitor_lock();
	g_health_watchdog_next_refresh = clock_systimer() + HEALTH_MONITOR_WATCHDOG_REFRESH_TICKS;
	g_health_watchdog_started = true;
	health_monitor_unlock(flags);
}

/****************************************************************************
 * Name: health_monitor_timer
 *
 * Description:
 *   Check thread health each tick, report failures synchronously and refresh
 *   the hardware watchdog only when a successful check permits it.
 *
 * Assumptions:
 *   Called after the clock update and before global locks. CPU0 only on SMP.
 ****************************************************************************/

void health_monitor_timer(void)
{
	struct health_monitor_failure_s failure;
	enum health_monitor_check_result_e result;
	irqstate_t flags;

#ifdef CONFIG_SMP
	if (this_cpu() != 0) {
		return;
	}
#endif
	if (!g_health_watchdog_ready) {
		if (!health_monitor_trylock(&flags)) {
			return;
		}
		g_health_watchdog_ready = g_health_watchdog_started;
		health_monitor_unlock(flags);
		if (!g_health_watchdog_ready) {
			return;
		}
	}

	result = health_monitor_check(clock_systimer(), &failure);
	if (result == HEALTH_MONITOR_CHECK_DEFERRED) {
		return;
	}
	if (result == HEALTH_MONITOR_CHECK_TIMEOUT) {
		health_monitor_fail(&failure);
		return;
	}

	health_monitor_watchdog_maybe_refresh();
}
