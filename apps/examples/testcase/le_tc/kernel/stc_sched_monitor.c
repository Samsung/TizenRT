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

/// @file stc_sched_monitor.c

/// @brief Background starvation invariant monitor

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <tinyara/sched.h>
#include <tinyara/os_api_test_drv.h>

#include "tc_internal.h"
#include "stc_sched_common.h"
#include "stc_sched_monitor.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Grace period converted from slices to samples.  Clamped to at least two
 * samples so that a very small CONFIG_RR_INTERVAL cannot reduce it to a
 * single sample, where ordinary jitter would start to register.
 */

#define STC_MON_GRACE_MS        (STC_MON_GRACE_SLICES * CONFIG_RR_INTERVAL)
#define STC_MON_GRACE_SAMPLES   ((STC_MON_GRACE_MS / STC_MON_PERIOD_MS) > 2 ? \
				 (STC_MON_GRACE_MS / STC_MON_PERIOD_MS) : 2)

/****************************************************************************
 * Private Types
 ****************************************************************************/

/* One task as observed during a single walk of the task lists. */

struct stc_mon_obs_s {
	pid_t pid;
	uint8_t state;
	uint8_t priority;
};

/* Per task dwell accounting, carried across samples. */

struct stc_mon_dwell_s {
	pid_t pid;
	uint32_t samples;		/* Consecutive samples seen ready to run */
	bool active;			/* Slot in use                           */
	bool seen;			/* Observed during the current sample     */
	bool reported;			/* Episode already counted as a violation */
};

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* Filled by the sampling callback, consumed after the walk returns. */

static struct stc_mon_obs_s g_obs[STC_MON_MAX_TASKS];
static volatile int g_obs_count;

static struct stc_mon_dwell_s g_dwell[STC_MON_MAX_TASKS];

/* Monitor lifecycle. */

static volatile bool g_mon_run;
static volatile bool g_mon_exited;
static pid_t g_mon_pid;
static int g_mon_minprio;

/* Results. */

static volatile uint32_t g_violations;
static volatile uint32_t g_samples;
static volatile uint32_t g_max_dwell;
static volatile pid_t g_worst_pid;

/* Set when the monitor could not open the test driver.  Reported rather
 * than silently ignored, because a monitor that cannot read the task
 * lists produces a zero violation count that means nothing.
 */

static volatile bool g_mon_open_failed;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stc_mon_sample_cb
 *
 * Description:
 *   Called once per task, from inside sched_foreach(), which holds a
 *   critical section for the whole walk.  This does the minimum possible:
 *   copy three fields and return.  Every decision is made later, outside
 *   the critical section, in stc_mon_evaluate().
 *
 ****************************************************************************/

static void stc_mon_sample_cb(struct tcb_s *tcb, void *arg)
{
	int n = g_obs_count;

	if (tcb == NULL || n >= STC_MON_MAX_TASKS) {
		return;
	}

	g_obs[n].pid = tcb->pid;
	g_obs[n].state = tcb->task_state;
	g_obs[n].priority = tcb->sched_priority;
	g_obs_count = n + 1;
}

/****************************************************************************
 * Name: stc_mon_find_slot
 *
 * Description:
 *   Returns the dwell slot for a PID, allocating one if the PID is not
 *   already tracked.  Returns NULL when the table is full, in which case
 *   that task is simply not tracked this sample rather than displacing
 *   another.
 *
 ****************************************************************************/

static struct stc_mon_dwell_s *stc_mon_find_slot(pid_t pid)
{
	int free_slot = -1;
	int i;

	for (i = 0; i < STC_MON_MAX_TASKS; i++) {
		if (g_dwell[i].active && g_dwell[i].pid == pid) {
			return &g_dwell[i];
		}

		if (!g_dwell[i].active && free_slot < 0) {
			free_slot = i;
		}
	}

	if (free_slot < 0) {
		return NULL;
	}

	g_dwell[free_slot].pid = pid;
	g_dwell[free_slot].samples = 0;
	g_dwell[free_slot].active = true;
	g_dwell[free_slot].seen = false;
	g_dwell[free_slot].reported = false;

	return &g_dwell[free_slot];
}

/****************************************************************************
 * Name: stc_mon_evaluate
 *
 * Description:
 *   Applies the invariant to one sample.
 *
 *   A task counts towards its dwell when it is ready to run but not running
 *   and its priority is at or above the band under test.  Any other state
 *   resets the count, so the figure is always a *consecutive* run of samples
 *   in which the task was passed over.
 *
 *   A violation is raised once per episode rather than once per sample.
 *   A permanently starved task would otherwise contribute a violation every
 *   10 ms, which says nothing more than the first one did and makes the
 *   count depend on how long the scenario happened to run.
 *
 *   Note on scope: the catalogue's SCN-SMP-01 also compares the waiting
 *   task's priority against the task occupying each CPU in its affinity
 *   mask.  That comparison is not made here.  The monitor runs above the
 *   workload, so on a two CPU target it is frequently the task occupying a
 *   CPU at the instant it samples, which makes the per CPU occupant
 *   unreliable evidence.  Bounding the watch to a priority band achieves
 *   the same protection against false positives without depending on it.
 *
 ****************************************************************************/

static void stc_mon_evaluate(void)
{
	struct stc_mon_dwell_s *slot;
	uint32_t dwell_ms;
	int i;

	for (i = 0; i < STC_MON_MAX_TASKS; i++) {
		g_dwell[i].seen = false;
	}

	for (i = 0; i < g_obs_count; i++) {
		/* Skip the monitor itself and anything below the band. */

		if (g_obs[i].pid == g_mon_pid) {
			continue;
		}

		if ((int)g_obs[i].priority < g_mon_minprio) {
			continue;
		}

		if (g_obs[i].state != TSTATE_TASK_READYTORUN) {
			continue;
		}

		slot = stc_mon_find_slot(g_obs[i].pid);
		if (slot == NULL) {
			continue;
		}

		slot->seen = true;
		slot->samples++;

		dwell_ms = slot->samples * STC_MON_PERIOD_MS;
		if (dwell_ms > g_max_dwell) {
			g_max_dwell = dwell_ms;
			g_worst_pid = slot->pid;
		}

		if (slot->samples > STC_MON_GRACE_SAMPLES && !slot->reported) {
			slot->reported = true;
			g_violations++;
		}
	}

	/* Any tracked task that was not ready to run this sample got a CPU, or
	 * blocked, or exited.  Either way its episode has ended.
	 */

	for (i = 0; i < STC_MON_MAX_TASKS; i++) {
		if (g_dwell[i].active && !g_dwell[i].seen) {
			g_dwell[i].samples = 0;
			g_dwell[i].reported = false;
			g_dwell[i].active = false;
		}
	}
}

/****************************************************************************
 * Name: stc_mon_task
 ****************************************************************************/

static int stc_mon_task(int argc, char *argv[])
{
	int fd;

	/* Open the driver from inside this task rather than borrowing the
	 * descriptor held by the task that started us.
	 *
	 * task_create() gives the new task its own task group, and with
	 * CONFIG_FDCLONE_STDIO only descriptors 0, 1 and 2 are cloned into
	 * it; see NFDS_TOCLONE in group_setuptaskfiles.c.  The driver
	 * descriptor opened by the test task is well above that, so its
	 * numeric value is simply not valid here.  Reusing it makes every
	 * ioctl fail, the sample loop collect nothing, and the violation
	 * count stay at zero for want of any data at all.
	 */

	fd = open(OS_API_TEST_DRVPATH, O_WRONLY);
	if (fd < 0) {
		g_mon_open_failed = true;
		g_mon_exited = true;
		return ERROR;
	}

#ifdef CONFIG_SMP
	/* Keep the monitor on CPU0 so it does not migrate around the workload
	 * it is observing.
	 */

	cpu_set_t mask = 0;

	CPU_SET(0, &mask);
	(void)sched_setaffinity(0, sizeof(cpu_set_t), &mask);
#endif

	while (g_mon_run) {
		usleep(STC_MON_PERIOD_MS * 1000);

		if (!g_mon_run) {
			break;
		}

		g_obs_count = 0;
		(void)ioctl(fd, TESTIOC_SCHED_FOREACH,
			    (unsigned long)stc_mon_sample_cb);

		if (g_obs_count > 0) {
			stc_mon_evaluate();
			g_samples++;
    			sched_yield();
		}
	}

	(void)close(fd);

	g_mon_exited = true;
	return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stc_monitor_start
 ****************************************************************************/

int stc_monitor_start(int min_priority)
{
	int prio;

	if (g_mon_run) {
		return ERROR;
	}

	memset(g_dwell, 0, sizeof(g_dwell));
	memset(g_obs, 0, sizeof(g_obs));
	g_obs_count = 0;
	g_violations = 0;
	g_samples = 0;
	g_max_dwell = 0;
	g_worst_pid = 0;
	g_mon_minprio = min_priority;
	g_mon_open_failed = false;
	g_mon_exited = false;
	g_mon_run = true;

	/* Above the workload and above the harness boost, so that a sample is
	 * never delayed by the load being measured.
	 */

	prio = SCHED_PRIORITY_MAX - 1;

	g_mon_pid = task_create("stcmon", prio, STC_MON_STACKSIZE,
				stc_mon_task, (char * const *)NULL);
	if (g_mon_pid < 0) {
		g_mon_run = false;
		return ERROR;
	}

	return OK;
}

/****************************************************************************
 * Name: stc_monitor_stop
 ****************************************************************************/

int stc_monitor_stop(void)
{
	int waited = 0;

	if (!g_mon_run) {
		return ERROR;
	}

	g_mon_run = false;

	while (!g_mon_exited && waited < STC_TEARDOWN_MS) {
		usleep(10 * 1000);
		waited += 10;
	}

	if (!g_mon_exited) {
		if (g_mon_pid > 0) {
			(void)task_delete(g_mon_pid);
		}

		return ERROR;
	}

	return OK;
}

/****************************************************************************
 * Name: stc_monitor_violations
 ****************************************************************************/

uint32_t stc_monitor_violations(void)
{
	return g_violations;
}

/****************************************************************************
 * Name: stc_monitor_samples
 ****************************************************************************/

uint32_t stc_monitor_samples(void)
{
	return g_samples;
}

/****************************************************************************
 * Name: stc_monitor_max_dwell_ms
 ****************************************************************************/

uint32_t stc_monitor_max_dwell_ms(void)
{
	return g_max_dwell;
}

/****************************************************************************
 * Name: stc_monitor_worst_pid
 ****************************************************************************/

pid_t stc_monitor_worst_pid(void)
{
	return g_worst_pid;
}

/****************************************************************************
 * Name: stc_monitor_report
 ****************************************************************************/

void stc_monitor_report(const char *tag)
{
	printf("[%s] monitor samples=%u violations=%u max_dwell=%ums (grace=%dms) worst_pid=%d%s\n",
	       tag, (unsigned int)g_samples, (unsigned int)g_violations,
	       (unsigned int)g_max_dwell,
	       (int)(STC_MON_GRACE_SAMPLES * STC_MON_PERIOD_MS),
	       (int)g_worst_pid,
	       g_mon_open_failed ? " DRIVER-OPEN-FAILED" : "");
}
