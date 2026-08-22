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

/// @file stc_sched_common.h

/// @brief Shared harness for scheduling scenario test cases
///
/// Scenario test cases assert on scheduler *behaviour* under a controlled
/// workload rather than on API return values.  This harness supplies the
/// pieces every scenario needs:
///
///   - a non-blocking spin worker whose progress counter is isolated in its
///     own cache line, so that counters updated from different CPUs do not
///     ping-pong through the coherency fabric and distort the measurement,
///   - a window runner that discards a warm-up period, applies the load for
///     a fixed interval, snapshots the counters and tears the workers down,
///   - the oracles (minimum, maximum, sum and fairness ratio) used to turn
///     those counters into a pass or fail decision.
///
/// Measurements are expressed in units of work completed, never in elapsed
/// time.  The system tick is 1 ms while CONFIG_RR_INTERVAL is typically
/// 10 ms, which leaves too little headroom for a timing based oracle.

#ifndef __EXAMPLES_TESTCASE_KERNEL_STC_SCHED_COMMON_H
#define __EXAMPLES_TESTCASE_KERNEL_STC_SCHED_COMMON_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdint.h>
#include <stdbool.h>
#include <sched.h>
#include <sys/types.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Upper bound on workers in a single scenario. */

#define STC_MAX_WORKERS         8

/* Cache line size in bytes.  Worker counters are padded to this so that no
 * two workers ever share a line.  On a target running from external DDR,
 * unpadded neighbouring counters written from two CPUs produce false sharing
 * that silently skews every ratio this harness computes.
 */

#define STC_CACHELINE           64

/* Worker stack.  The spin worker touches almost no stack; this is the
 * smallest comfortable size rather than a requirement.
 */

#define STC_WORKER_STACKSIZE    2048

/* Time, in milliseconds, discarded before the counters are zeroed.  Task
 * creation and first touch of the counter pages both land in this period.
 */

#define STC_WARMUP_MS           200

/* Default measurement window.  At CONFIG_RR_INTERVAL of 10 ms this is 300
 * timeslices, which is enough for slice boundary quantisation to average out.
 */

#define STC_WINDOW_MS           3000

/* The harness thread must not compete with the workers it is measuring, so
 * it runs this far above them and does nothing but sleep.
 */

#define STC_HARNESS_BOOST       10

/* Upper bound on how long the harness waits for workers to exit. */

#define STC_TEARDOWN_MS         2000

#define STC_ALL_CPUS_AFFINITY   ((1 << CONFIG_SMP_NCPUS) - 1)

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* One worker's progress counter, isolated in its own cache line. */

struct stc_worker_s {
	volatile uint32_t count;
	uint8_t pad[STC_CACHELINE - sizeof(uint32_t)];
} __attribute__((aligned(STC_CACHELINE)));

/* Per worker CPU placement observations, gathered when sample_cpu is set.
 * Isolated in its own cache line for the same reason the counters are.
 */

struct stc_cpuobs_s {
	volatile uint32_t seen_mask;	/* Bit set of CPUs this worker ran on */
	volatile uint32_t violations;	/* Samples taken outside its mask     */
	uint8_t pad[STC_CACHELINE - 2 * sizeof(uint32_t)];
} __attribute__((aligned(STC_CACHELINE)));

/* Description of the load a scenario wants applied.
 *
 * Always initialise with stc_workload_init() before setting fields.  The
 * structure has optional members, and a partially initialised one would take
 * its per worker arrays from whatever was on the stack.
 */

struct stc_workload_s {
	int nworkers;			/* Number of spin workers to create      */
	int priority;			/* Priority every worker runs at         */
	int policy;			/* SCHED_RR or SCHED_FIFO                */
	int window_ms;			/* Measurement window, 0 for the default */
	bool pinned;			/* true: apply the affinity mask below   */
	cpu_set_t affinity;		/* Mask applied when pinned is true      */

	/* Optional per worker overrides.  When non NULL each array must hold
	 * at least nworkers entries and takes precedence over the single
	 * values above.
	 */

	const int *priorities;		/* Per worker priority, or NULL          */
	const cpu_set_t *affinities;	/* Per worker affinity mask, or NULL     */

	/* When true each worker calls sched_getcpu() on every iteration and
	 * records where it ran.  This turns the progress counter into a count
	 * of CPU samples rather than of raw loop iterations, so do not compare
	 * fairness figures between a sampling and a non sampling run.
	 */

	bool sample_cpu;
};

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/* Number of CPUs the scheduler is managing.  Scenarios must use this rather
 * than a hard coded value so that the suite also runs on a single core build.
 */

int stc_ncpus(void);

/* Populate a workload with safe defaults: no per worker arrays, no CPU
 * sampling, not pinned, and the default measurement window.  Every scenario
 * must call this before setting any field.
 */

void stc_workload_init(struct stc_workload_s *wl, int nworkers, int priority,
		       int policy);

/* Split workload lifecycle, for scenarios that need to act while the load is
 * running, for example to change a task's priority or affinity mid flight.
 *
 *   stc_workload_start()   create the workers, discard the warm-up, raise
 *                          the caller above the workload
 *   stc_workload_sample()  zero the counters, run for window_ms, snapshot
 *                          (may be called more than once)
 *   stc_workload_finish()  stop the workers, wait for them, restore the
 *                          caller's priority
 *
 * stc_run_workload() below is simply the three called in order.
 */

int stc_workload_start(const struct stc_workload_s *wl);
void stc_workload_sample(int window_ms);
int stc_workload_finish(int nworkers);

/* PID of a running worker, for scenarios that act on one mid flight.
 * Returns 0 when the index is out of range or no worker was created.
 */

pid_t stc_worker_pid(int idx);

/* Update the CPU mask a worker is expected to stay within.  Call this
 * alongside sched_setaffinity() so that samples taken after the change are
 * judged against the new mask rather than the old one.
 */

void stc_set_allowed(int idx, cpu_set_t mask);

/* CPU placement results, valid when the workload set sample_cpu. */

uint32_t stc_cpu_violations(int idx);
uint32_t stc_cpu_seen_mask(int idx);

/* Apply a workload and record the result.
 *
 * Creates the workers, discards STC_WARMUP_MS, zeroes the counters, applies
 * the load for the window, snapshots the counters, stops the workers and
 * waits for them to exit.  The caller's priority is raised for the duration
 * so that the harness never competes with the workload.
 *
 * Returns OK, or ERROR if a worker could not be created or did not exit.
 */

int stc_run_workload(const struct stc_workload_s *wl);

/* Oracles.  All operate on the snapshot taken by stc_run_workload(). */

uint32_t stc_count(int idx);
uint32_t stc_min(int nworkers);

/* Minimum over a half open range of workers, [first, last).  Scenarios
 * that deliberately starve one worker must exclude it from the minimum,
 * or the oracle asserts the opposite of the property under test.
 */

uint32_t stc_min_range(int first, int last);
uint32_t stc_max(int nworkers);
uint32_t stc_sum(int nworkers);

/* Fairness as an integer percentage, 100 * min / max.  Returns 0 when the
 * maximum is zero, which cannot pass any threshold and so fails safe.
 */

int stc_fair_pct(int nworkers);

/* Print one line per worker.  Call this only outside a measurement window;
 * console output takes locks and raises interrupts.
 */

void stc_report(const char *tag, int nworkers);

#endif							/* __EXAMPLES_TESTCASE_KERNEL_STC_SCHED_COMMON_H */
