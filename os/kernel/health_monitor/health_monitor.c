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

/****************************************************************************
 * Global Variables
 ****************************************************************************/

static struct tcb_s *g_health_heap[HEALTH_MONITOR_HEAP_CAPACITY];
static uint32_t g_health_count;
static spinlock_t g_health_lock;

static volatile uint32_t g_health_sequence;
static volatile uint32_t g_health_next_count;
static volatile clock_t g_health_next_tick;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static struct health_monitor_s *health_monitor_get_state(struct tcb_s *tcb);

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

static void health_monitor_dump(const struct health_monitor_snapshot_s *expired)
{
	struct tcb_s *tcb;
	clock_t last_refresh = expired->deadline - expired->timeout;

	if (IS_SECURE_STATE()) {
		return;
	}
#if CONFIG_TASK_NAME_SIZE > 0
	lldbg("Monitored task name=%s\n", expired->name);
#endif
	lldbg("Monitored task state=%u priority=%u cpu=%u timeout_ticks=%lu last_refresh=0x%08lx%08lx\n",
		(unsigned int)expired->task_state, (unsigned int)expired->priority, (unsigned int)expired->cpu,
		(unsigned long)expired->timeout, (unsigned long)((uint64_t)last_refresh >> 32), (unsigned long)(uint32_t)last_refresh);

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
	clock_t next = g_health_count ? health_monitor_get_state(g_health_heap[0])->deadline : 0;

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
 *   Return whether lhs is an earlier deadline than rhs relative to now.
 *
 * Inputs:
 *   lhs  - First deadline tick.
 *   rhs  - Second deadline tick.
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
	struct tcb_s *entry = g_health_heap[index];
	clock_t deadline = health_monitor_get_state(entry)->deadline;
	uint32_t parent;

	while (index > 0) {
		parent = (index - 1) / 2;
		if (!health_monitor_earlier(deadline, health_monitor_get_state(g_health_heap[parent])->deadline, now)) {
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
	struct tcb_s *entry = g_health_heap[index];
	clock_t deadline = health_monitor_get_state(entry)->deadline;
	uint32_t child;

	while ((child = index * 2 + 1) < g_health_count) {
		if (child + 1 < g_health_count && health_monitor_earlier(health_monitor_get_state(g_health_heap[child + 1])->deadline,
			health_monitor_get_state(g_health_heap[child])->deadline, now)) {
			child++;
		}

		if (!health_monitor_earlier(health_monitor_get_state(g_health_heap[child])->deadline, deadline, now)) {
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
		if (index > 0 && health_monitor_earlier(health_monitor_get_state(g_health_heap[index])->deadline,
			health_monitor_get_state(g_health_heap[(index - 1) / 2])->deadline, now)) {
			health_monitor_sift_up(index, now);
		} else {
			health_monitor_sift_down(index, now);
		}
	}

	g_health_heap[g_health_count] = NULL;
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
		if (g_health_heap[index] == tcb) {
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
 *   Inspect only the earliest deadline. START/KICK/STOP maintain heap order
 *   in thread context; the ISR never repairs or walks the heap. Capture timeout
 *   details under the lock without diagnostics or watchdog I/O.
 *
 * Assumptions:
 *   Called by CPU0 from the tick ISR. Contention defers to a later tick.
 ****************************************************************************/

static int health_monitor_check(clock_t now, struct health_monitor_timeout_s *failure)
{
	struct tcb_s *tcb;
	clock_t deadline;
	clock_t check_at;
	irqstate_t flags;
	int status;
	int result = 0;

	status = health_monitor_get_next_check_time(&check_at);
	if (status < 0) {
		return -EAGAIN;
	}
	if (status == 0 || health_monitor_is_tick_before(now, check_at)) {
		return 0;
	}

	if (!health_monitor_trylock(&flags)) {
		return -EAGAIN;
	}

	now = clock_systimer();
	if (g_health_count > 0) {
		tcb = g_health_heap[0];
		deadline = health_monitor_get_state(tcb)->deadline;
		if (!health_monitor_is_tick_before(now, deadline)) {
			result = -ETIMEDOUT;
			failure->detected_at = now;
			health_monitor_snapshot(tcb, &failure->task);
		}
	}

	spin_unlock_irqrestore(&g_health_lock, flags);
	return result;
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

	flags = spin_lock_irqsave(&g_health_lock);

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
		g_health_heap[index] = tcb;
		health_monitor_sift_up(index, now);
		health_monitor_publish();
	}

	spin_unlock_irqrestore(&g_health_lock, flags);
	return ret;
}

/****************************************************************************
 * Name: health_monitor_kick
 *
 * Description:
 *   Renew the calling thread's deadline and restore heap order immediately.
 *   No-op if unregistered.
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
	uint32_t index;
	clock_t now;

	flags = spin_lock_irqsave(&g_health_lock);

	tcb = this_task();
	state = health_monitor_get_state(tcb);
	if (state->timeout != 0) {
		for (index = 0; index < g_health_count; index++) {
			if (g_health_heap[index] == tcb) {
				now = clock_systimer();
				state->deadline = now + state->timeout;
				health_monitor_sift_down(index, now);
				health_monitor_publish();
				break;
			}
		}
	}

	spin_unlock_irqrestore(&g_health_lock, flags);
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

	flags = spin_lock_irqsave(&g_health_lock);

	tcb = this_task();
	ret = health_monitor_unregister(tcb);

	spin_unlock_irqrestore(&g_health_lock, flags);
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
	struct health_monitor_snapshot_s info;
	struct tcb_s *current;
	irqstate_t flags;
	bool active;
#ifdef CONFIG_SYSTEM_REBOOT_REASON
	reboot_reason_code_t reason;
#endif

	if (tcb == NULL) {
		return;
	}

	flags = spin_lock_irqsave(&g_health_lock);
	active = health_monitor_get_state(tcb)->timeout != 0;
	if (active) {
		health_monitor_snapshot(tcb, &info);
#ifdef CONFIG_SYSTEM_REBOOT_REASON
		reason = (tcb->flags & TCB_FLAG_TTYPE_MASK) == TCB_FLAG_TTYPE_KERNEL ?
			REBOOT_SYSTEM_HEALTH_MONITOR_UNEXPECTED_EXIT : REBOOT_USER_HEALTH_MONITOR_UNEXPECTED_EXIT;
#endif
	}
	spin_unlock_irqrestore(&g_health_lock, flags);
	if (!active) {
		return;
	}

#ifdef CONFIG_SYSTEM_REBOOT_REASON
	up_reboot_reason_write(reason);
#endif
	current = this_task();
	lldbg("HEALTH MONITOR UNEXPECTED_EXIT pid=%d current_pid=%d\n", (int)info.pid, (int)current->pid);
#if CONFIG_TASK_NAME_SIZE > 0
	if (!IS_SECURE_STATE()) {
		lldbg("Scheduler current task name=%s\n", current->name);
	}
#endif
	health_monitor_dump(&info);
	PANIC();
}

/****************************************************************************
 * Name: health_monitor_get_next_check_time
 *
 * Description:
 *   Read the published earliest deadline tick via lockless snapshot.
 *
 * Inputs:
 *   check_at - Output pointer for earliest deadline tick. Must not be NULL.
 *
 * Return Value:
 *   1 and write check_at if registered threads exist; 0 if empty; -EAGAIN if a
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
	if (count == 0) {
		if (sequence != g_health_sequence) {
			return -EAGAIN;
		}
		return 0;
	}

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

	*check_at = next;
	return 1;
}



/****************************************************************************
 * Name: health_monitor_timer
 *
 * Description:
 *   Check thread health each tick and report failures synchronously. Return
 *   true only when a successful check permits a hardware watchdog refresh.
 *
 * Assumptions:
 *   Called after the clock update and before global locks. CPU0 only on SMP.
 ****************************************************************************/

bool health_monitor_timer(void)
{
	struct health_monitor_timeout_s failure;
	int result;
#ifdef CONFIG_SYSTEM_REBOOT_REASON
	reboot_reason_code_t reason;
#endif

	result = health_monitor_check(clock_systimer(), &failure);
	if (result == -ETIMEDOUT) {
#ifdef CONFIG_SYSTEM_REBOOT_REASON
		reason = failure.task.kernel_task ? REBOOT_SYSTEM_HEALTH_MONITOR_TIMEOUT : REBOOT_USER_HEALTH_MONITOR_TIMEOUT;

		up_reboot_reason_write(reason);
#endif
		lldbg("HEALTH MONITOR TIMEOUT pid=%d now=0x%08lx%08lx deadline=0x%08lx%08lx\n", (int)failure.task.pid,
			(unsigned long)((uint64_t)failure.detected_at >> 32), (unsigned long)(uint32_t)failure.detected_at,
			(unsigned long)((uint64_t)failure.task.deadline >> 32), (unsigned long)(uint32_t)failure.task.deadline);
		health_monitor_dump(&failure.task);
		PANIC();
		return false;
	}

	return result == 0;
}
