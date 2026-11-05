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
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <tinyara/arch.h>
#include <tinyara/clock.h>
#include <tinyara/irq.h>
#include <tinyara/kthread.h>
#include <tinyara/sched.h>
#include <tinyara/security_level.h>
#include <tinyara/spinlock.h>
#include <tinyara/watchdog.h>
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

#define HEALTH_MONITOR_THREAD_NAME                "health_monitor_watchdog"
#define HEALTH_MONITOR_WATCHDOG_THREAD_TIMEOUT_MS 60000U
#define HEALTH_MONITOR_WATCHDOG_THREAD_PRIORITY   100
#define HEALTH_MONITOR_WATCHDOG_THREAD_STACKSIZE  2048

/****************************************************************************
 * Global Variables
 ****************************************************************************/

static struct tcb_s *g_health_heap[HEALTH_MONITOR_HEAP_CAPACITY];
static uint32_t g_health_count;
static spinlock_t g_health_lock;

static volatile uint32_t g_health_sequence;
static volatile uint32_t g_health_next_count;
static volatile clock_t g_health_next_tick;
static bool g_health_watchdog_started;

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

static int health_monitor_check(clock_t now, struct health_monitor_failure_s *failure)
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
			failure->type = HEALTH_MONITOR_FAILURE_TIMEOUT;
			failure->detected_at = now;
			health_monitor_snapshot(tcb, &failure->task);
		}
	}

	spin_unlock_irqrestore(&g_health_lock, flags);
	return result;
}

/****************************************************************************
 * Name: health_monitor_watchdog
 *
 * Description:
 *   Own watchdog keepalive in thread context and renew the thread's own HM.
 *   The timer ISR checks task deadlines.
 ****************************************************************************/

static int health_monitor_watchdog(int argc, char *argv[])
{
	uint32_t remaining;
	int fd;
	int ret;

	fd = open(CONFIG_WATCHDOG_DEVPATH, O_RDONLY);
	if (fd < 0) {
		lldbg("Watchdog thread open failed: %d\n", get_errno());
		return ERROR;
	}
	ret = health_monitor_start(HEALTH_MONITOR_WATCHDOG_THREAD_TIMEOUT_MS);
	if (ret < 0) {
		lldbg("Watchdog thread HM registration failed: %d\n", ret);
		close(fd);
		return ERROR;
	}

	for (;;) {
		health_monitor_kick();
		if (ioctl(fd, WDIOC_KEEPALIVE, 0) < 0) {
			lldbg("Watchdog keepalive failed: %d\n", get_errno());
			return ERROR;
		}

		remaining = CONFIG_HEALTH_MONITOR_WATCHDOG_INTERVAL_SEC;
		while (remaining > 0) {
			remaining = sleep(remaining);
		}
	}
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
	struct health_monitor_failure_s failure;
	irqstate_t flags;
	bool active;

	if (tcb == NULL) {
		return;
	}

	flags = spin_lock_irqsave(&g_health_lock);
	active = health_monitor_get_state(tcb)->timeout != 0;
	if (active) {
		failure.type = HEALTH_MONITOR_FAILURE_UNEXPECTED_EXIT;
		failure.detected_at = clock_systimer();
		health_monitor_snapshot(tcb, &failure.task);
	}
	spin_unlock_irqrestore(&g_health_lock, flags);
	if (!active) {
		return;
	}

	health_monitor_fail(&failure);
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
 * Name: health_monitor_initialize
 *
 * Description:
 *   Arm the watchdog device registered by board initialization before creating
 *   the monitored feeder thread. Closing the device does not stop WDT.
 *
 * Assumptions:
 *   Called on CPU0 after board initialization.
 ****************************************************************************/

void health_monitor_initialize(void)
{
	irqstate_t flags;
	int fd;
	int ret;

	fd = open(CONFIG_WATCHDOG_DEVPATH, O_RDONLY);
	if (fd < 0) {
		lldbg("Watchdog open failed: %d\n", get_errno());
		return;
	}
	if (ioctl(fd, WDIOC_SETTIMEOUT, CONFIG_HEALTH_MONITOR_WATCHDOG_TIMEOUT_MS) < 0 ||
		ioctl(fd, WDIOC_START, 0) < 0) {
		lldbg("Watchdog start failed: %d\n", get_errno());
		close(fd);
		return;
	}
	close(fd);

	ret = kernel_thread(HEALTH_MONITOR_THREAD_NAME, HEALTH_MONITOR_WATCHDOG_THREAD_PRIORITY, HEALTH_MONITOR_WATCHDOG_THREAD_STACKSIZE, health_monitor_watchdog, NULL);
	ASSERT(ret >= 0);

	flags = irqsave();
	g_health_watchdog_started = true;
	irqrestore(flags);
}

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
	struct health_monitor_failure_s failure;
	int result;

	if (!g_health_watchdog_started) {
		return;
	}

	result = health_monitor_check(clock_systimer(), &failure);
	if (result == -ETIMEDOUT) {
		health_monitor_fail(&failure);
		return;
	}
}
