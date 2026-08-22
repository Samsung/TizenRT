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

/// @file stc_sched_common.c

/// @brief Shared harness for scheduling scenario test cases

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sched.h>

#include "stc_sched_common.h"

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* Live counters, written by the workers.  Cache line isolated; see the
 * comment on struct stc_worker_s.
 */

static struct stc_worker_s g_worker[STC_MAX_WORKERS];

/* Stable copy taken at the end of the measurement window.  The oracles read
 * this, never the live counters, so that a worker still running during
 * teardown cannot change a value that has already been evaluated.
 */

static uint32_t g_snapshot[STC_MAX_WORKERS];

/* Worker run gate and exit accounting. */

static volatile bool g_run;
static volatile int g_exited;

/* Parameters handed to the workers.  Written before any worker is created
 * and read only, from the workers, thereafter.
 */

static int g_policy;
static bool g_pinned;
static cpu_set_t g_affinity;
static bool g_sample_cpu;

/* Per worker parameters, resolved from the workload before any worker is
 * created so that a worker never has to dereference the caller's structure.
 */

static int g_prio[STC_MAX_WORKERS];
static cpu_set_t g_mask[STC_MAX_WORKERS];
static bool g_has_mask[STC_MAX_WORKERS];

/* Mask each worker is expected to stay within.  Separate from g_mask because
 * a scenario may change a worker's affinity mid flight, and samples taken
 * after that must be judged against the new mask.
 */

static volatile cpu_set_t g_allowed[STC_MAX_WORKERS];

static struct stc_cpuobs_s g_cpuobs[STC_MAX_WORKERS];
static uint32_t g_cpuobs_violations[STC_MAX_WORKERS];
static uint32_t g_cpuobs_seen[STC_MAX_WORKERS];

/* Caller priority saved across a start and finish pair. */

static struct sched_param g_saved_param;
static bool g_prio_restore;

/* PIDs of the created workers, for teardown. */

static pid_t g_pid[STC_MAX_WORKERS];

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stc_spin_worker
 *
 * Description:
 *   Applies its own scheduling policy and affinity, then increments its
 *   private counter until the harness clears the run gate.
 *
 *   The loop contains no blocking call, no system call and no console
 *   output, so the counter measures CPU time granted to this task and
 *   nothing else.  A worker applies its own policy and affinity rather than
 *   having the harness apply them, because doing it from inside the task
 *   cannot race with the task's own start-up.
 *
 * Input Parameters:
 *   argv[1] - decimal index of this worker's counter slot
 *
 ****************************************************************************/

static int stc_spin_worker(int argc, char *argv[])
{
	struct sched_param param;
	int idx;

	if (argc < 2) {
		return ERROR;
	}

	idx = atoi(argv[1]);
	if (idx < 0 || idx >= STC_MAX_WORKERS) {
		return ERROR;
	}

	/* Apply the requested policy at this worker's priority. */

	param.sched_priority = g_prio[idx];
	(void)sched_setscheduler(0, g_policy, &param);

#ifdef CONFIG_SMP
	if (g_has_mask[idx]) {
		cpu_set_t mask = g_mask[idx];
		(void)sched_setaffinity(0, sizeof(cpu_set_t), &mask);
	}
#endif

	/* Burn CPU.  volatile on the counter keeps the increment from being
	 * hoisted out of the loop.
	 */

	while (g_run) {
		g_worker[idx].count++;

#ifdef CONFIG_SMP
		if (g_sample_cpu) {
			int cpu = sched_getcpu();
			cpu_set_t allowed = g_allowed[idx];

			if (cpu >= 0) {
				g_cpuobs[idx].seen_mask |= (1u << cpu);

				/* An allowed mask of zero means the worker was
				 * never constrained, so any CPU is correct.
				 */

				if (allowed != 0 &&
				    ((allowed >> cpu) & 1) == 0) {
					g_cpuobs[idx].violations++;
				}
			}
		}
#endif
	}

	g_exited++;
	return OK;
}

/****************************************************************************
 * Name: stc_zero_counters
 ****************************************************************************/

static void stc_zero_counters(void)
{
	int i;

	for (i = 0; i < STC_MAX_WORKERS; i++) {
		g_worker[i].count = 0;
		g_cpuobs[i].seen_mask = 0;
		g_cpuobs[i].violations = 0;
	}
}

/****************************************************************************
 * Name: stc_wait_exit
 *
 * Description:
 *   Waits, bounded, for every worker to leave its loop.  A leaked spinner
 *   would poison every scenario that runs afterwards, so this is checked
 *   rather than assumed.
 *
 ****************************************************************************/

static int stc_wait_exit(int nworkers)
{
	int waited = 0;

	while (g_exited < nworkers && waited < STC_TEARDOWN_MS) {
		usleep(10 * 1000);
		waited += 10;
	}

	return (g_exited >= nworkers) ? OK : ERROR;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stc_ncpus
 ****************************************************************************/

int stc_ncpus(void)
{
#ifdef CONFIG_SMP
	int ncpus = sched_getcpucount();

	return (ncpus > 0) ? ncpus : 1;
#else
	return 1;
#endif
}

/****************************************************************************
 * Name: stc_workload_init
 ****************************************************************************/

void stc_workload_init(struct stc_workload_s *wl, int nworkers, int priority,
		       int policy)
{
	if (wl == NULL) {
		return;
	}

	memset(wl, 0, sizeof(struct stc_workload_s));
	wl->nworkers = nworkers;
	wl->priority = priority;
	wl->policy = policy;
	wl->window_ms = STC_WINDOW_MS;

#ifdef CONFIG_SMP
	wl->affinity = STC_ALL_CPUS_AFFINITY;
	wl->pinned = true;	/* Apply the affinity by default */
#endif
}

/****************************************************************************
 * Name: stc_workload_start
 ****************************************************************************/

int stc_workload_start(const struct stc_workload_s *wl)
{
	struct sched_param boosted;
	char idxbuf[STC_MAX_WORKERS][8];
	char *argv[2];
	int top;
	int i;

	if (wl == NULL || wl->nworkers <= 0 || wl->nworkers > STC_MAX_WORKERS) {
		return ERROR;
	}

	/* Resolve every per worker parameter now, so that a worker never has
	 * to reach back into the caller's structure.
	 */

	g_policy = wl->policy;
	g_pinned = wl->pinned;
	g_affinity = wl->affinity;
	g_sample_cpu = wl->sample_cpu;

	top = wl->priority;

	for (i = 0; i < STC_MAX_WORKERS; i++) {
		g_prio[i] = wl->priority;
		g_mask[i] = wl->affinity;
		g_has_mask[i] = wl->pinned;
		g_allowed[i] = wl->pinned ? wl->affinity : 0;
	}

	for (i = 0; i < wl->nworkers; i++) {
		if (wl->priorities != NULL) {
			g_prio[i] = wl->priorities[i];
			if (g_prio[i] > top) {
				top = g_prio[i];
			}
		}

		if (wl->affinities != NULL) {
			g_mask[i] = wl->affinities[i];
			g_has_mask[i] = (wl->affinities[i] != 0);
			g_allowed[i] = wl->affinities[i];
		}
	}

	stc_zero_counters();
	memset(g_snapshot, 0, sizeof(g_snapshot));
	memset(g_cpuobs_violations, 0, sizeof(g_cpuobs_violations));
	memset(g_cpuobs_seen, 0, sizeof(g_cpuobs_seen));
	memset(g_pid, 0, sizeof(g_pid));
	g_exited = 0;
	g_run = true;

	/* Lift the harness above the highest priority in the workload.  A
	 * harness at or below the workload competes with the thing it is
	 * measuring.
	 */

	g_prio_restore = false;

	if (sched_getparam(0, &g_saved_param) == OK) {
		boosted.sched_priority = top + STC_HARNESS_BOOST;
		if (boosted.sched_priority > SCHED_PRIORITY_MAX) {
			boosted.sched_priority = SCHED_PRIORITY_MAX;
		}

		if (sched_setparam(0, &boosted) == OK) {
			g_prio_restore = true;
		}
	}

	for (i = 0; i < wl->nworkers; i++) {
		snprintf(idxbuf[i], sizeof(idxbuf[i]), "%d", i);
		argv[0] = idxbuf[i];
		argv[1] = NULL;

		g_pid[i] = task_create("stcwrk", g_prio[i],
				       STC_WORKER_STACKSIZE, stc_spin_worker,
				       (char * const *)argv);
		if (g_pid[i] < 0) {
			return ERROR;
		}
	}

	/* Discard the warm-up.  Zeroing the counters afterwards is a cheaper
	 * and more reliable way to drop this period than trying to timestamp
	 * its end.
	 */

	usleep(STC_WARMUP_MS * 1000);
	stc_zero_counters();

	return OK;
}

/****************************************************************************
 * Name: stc_workload_sample
 ****************************************************************************/

void stc_workload_sample(int window_ms)
{
	int window = (window_ms > 0) ? window_ms : STC_WINDOW_MS;
	int i;

	/* Each sample starts from zero, so consecutive samples describe
	 * distinct intervals rather than accumulating.
	 */

	stc_zero_counters();

	usleep(window * 1000);

	for (i = 0; i < STC_MAX_WORKERS; i++) {
		g_snapshot[i] = g_worker[i].count;
		g_cpuobs_violations[i] = g_cpuobs[i].violations;
		g_cpuobs_seen[i] = g_cpuobs[i].seen_mask;
	}
}

/****************************************************************************
 * Name: stc_workload_finish
 ****************************************************************************/

int stc_workload_finish(int nworkers)
{
	int ret = OK;
	int i;

	g_run = false;

	if (stc_wait_exit(nworkers) != OK) {
		for (i = 0; i < nworkers && i < STC_MAX_WORKERS; i++) {
			if (g_pid[i] > 0) {
				(void)task_delete(g_pid[i]);
			}
		}

		ret = ERROR;
	}

	if (g_prio_restore) {
		(void)sched_setparam(0, &g_saved_param);
		g_prio_restore = false;
	}

	return ret;
}

/****************************************************************************
 * Name: stc_worker_pid
 ****************************************************************************/

pid_t stc_worker_pid(int idx)
{
	if (idx < 0 || idx >= STC_MAX_WORKERS) {
		return 0;
	}

	return g_pid[idx];
}

/****************************************************************************
 * Name: stc_set_allowed
 ****************************************************************************/

void stc_set_allowed(int idx, cpu_set_t mask)
{
	if (idx < 0 || idx >= STC_MAX_WORKERS) {
		return;
	}

	g_allowed[idx] = mask;
}

/****************************************************************************
 * Name: stc_cpu_violations
 ****************************************************************************/

uint32_t stc_cpu_violations(int idx)
{
	if (idx < 0 || idx >= STC_MAX_WORKERS) {
		return 0;
	}

	return g_cpuobs_violations[idx];
}

/****************************************************************************
 * Name: stc_cpu_seen_mask
 ****************************************************************************/

uint32_t stc_cpu_seen_mask(int idx)
{
	if (idx < 0 || idx >= STC_MAX_WORKERS) {
		return 0;
	}

	return g_cpuobs_seen[idx];
}

/****************************************************************************
 * Name: stc_run_workload
 ****************************************************************************/

int stc_run_workload(const struct stc_workload_s *wl)
{
	int ret;

	if (wl == NULL || wl->nworkers <= 0 || wl->nworkers > STC_MAX_WORKERS) {
		return ERROR;
	}

	ret = stc_workload_start(wl);
	if (ret == OK) {
		stc_workload_sample(wl->window_ms);
	}

	if (stc_workload_finish(wl->nworkers) != OK) {
		ret = ERROR;
	}

	return ret;
}

/****************************************************************************
 * Name: stc_count
 ****************************************************************************/

uint32_t stc_count(int idx)
{
	if (idx < 0 || idx >= STC_MAX_WORKERS) {
		return 0;
	}

	return g_snapshot[idx];
}

/****************************************************************************
 * Name: stc_min
 ****************************************************************************/

uint32_t stc_min(int nworkers)
{
	uint32_t lo = UINT32_MAX;
	int i;

	if (nworkers <= 0 || nworkers > STC_MAX_WORKERS) {
		return 0;
	}

	for (i = 0; i < nworkers; i++) {
		if (g_snapshot[i] < lo) {
			lo = g_snapshot[i];
		}
	}

	return lo;
}

/****************************************************************************
 * Name: stc_min_range
 ****************************************************************************/

uint32_t stc_min_range(int first, int last)
{
	uint32_t lo = UINT32_MAX;
	int i;

	if (first < 0 || last > STC_MAX_WORKERS || first >= last) {
		return 0;
	}

	for (i = first; i < last; i++) {
		if (g_snapshot[i] < lo) {
			lo = g_snapshot[i];
		}
	}

	return lo;
}

/****************************************************************************
 * Name: stc_max
 ****************************************************************************/

uint32_t stc_max(int nworkers)
{
	uint32_t hi = 0;
	int i;

	if (nworkers <= 0 || nworkers > STC_MAX_WORKERS) {
		return 0;
	}

	for (i = 0; i < nworkers; i++) {
		if (g_snapshot[i] > hi) {
			hi = g_snapshot[i];
		}
	}

	return hi;
}

/****************************************************************************
 * Name: stc_sum
 ****************************************************************************/

uint32_t stc_sum(int nworkers)
{
	uint32_t total = 0;
	int i;

	if (nworkers <= 0 || nworkers > STC_MAX_WORKERS) {
		return 0;
	}

	for (i = 0; i < nworkers; i++) {
		total += g_snapshot[i];
	}

	return total;
}

/****************************************************************************
 * Name: stc_fair_pct
 ****************************************************************************/

int stc_fair_pct(int nworkers)
{
	uint32_t lo = stc_min(nworkers);
	uint32_t hi = stc_max(nworkers);

	/* A maximum of zero means no worker ran at all.  Returning zero fails
	 * every threshold, which is the safe direction.
	 */

	if (hi == 0) {
		return 0;
	}

	/* Over a multi-second window the counters reach tens of millions, so
	 * lo * 100 would overflow 32 bits.  Dividing the denominator first
	 * keeps the arithmetic in 32 bits and avoids pulling in the 64 bit
	 * division helper; the small-hi branch preserves accuracy when the
	 * counters are too low for that to work.
	 */

	if (hi >= 100) {
		return (int)(lo / (hi / 100));
	}

	return (int)(lo * 100 / hi);
}

/****************************************************************************
 * Name: stc_report
 ****************************************************************************/

void stc_report(const char *tag, int nworkers)
{
	int i;

	if (nworkers <= 0 || nworkers > STC_MAX_WORKERS) {
		return;
	}

	printf("[%s] workers=%d fairness=%d%% ", tag, nworkers,
	       stc_fair_pct(nworkers));

	for (i = 0; i < nworkers; i++) {
		printf("w%d=%u ", i, (unsigned int)g_snapshot[i]);
	}

	printf("\n");
}
