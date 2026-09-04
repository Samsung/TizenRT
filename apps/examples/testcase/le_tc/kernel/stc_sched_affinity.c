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

/// @file stc_sched_affinity.c

/// @brief Scenario test cases for affinity and CPU placement (family C)
///
/// The existing API level test for affinity sets a mask and reads the same
/// mask back.  That confirms the value was stored; it says nothing about
/// whether the scheduler honoured it.  These scenarios ask the running task
/// itself, repeatedly and across many preemptions, which CPU it is actually
/// on.
///
/// Every scenario here needs more than one CPU to mean anything, so the
/// whole file compiles away without CONFIG_SMP and the entry point reports
/// a skip.

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <tinyara/sched.h>
#include <tinyara/os_api_test_drv.h>

#include "tc_internal.h"
#include "stc_sched_common.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifdef CONFIG_SMP

/* Window for placement scenarios.  Placement is a yes or no property, so no
 * averaging is needed and a shorter window than the fairness scenarios use
 * is enough.  At a few million samples per second this still produces far
 * more than the thousand samples the scenario calls for.
 */

#define STC_AFF_WINDOW_MS       1500

/* Minimum samples that must be taken before a zero violation count is
 * treated as evidence rather than as an absence of measurement.
 */

#define STC_AFF_MIN_SAMPLES     1000

/* Time allowed for a task to actually move after its affinity has been
 * narrowed, before its placement is judged against the new mask.
 *
 * sched_setaffinity() returning does not mean the task has migrated; the
 * move happens when the scheduler next handles it.  Samples taken in
 * that interval are legitimately still on the old CPU, and counting them
 * as violations would fail a correct implementation.  The settle period
 * is discarded because stc_workload_sample() zeroes the counters before
 * its window begins.
 */

#define STC_AFF_SETTLE_MS       100

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* Filled by the idle task scan.  sched_foreach() passes a NULL argument
 * through the test driver, so the result has to come back in a static.
 */

static volatile pid_t g_idle_pid;
static volatile pid_t g_scan_self;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stc_aff_find_idle_cb
 *
 * Description:
 *   Records the PID of an IDLE task.  The IDLE tasks are the only ones in
 *   the system at priority zero, and they are the only tasks flagged as
 *   locked to a CPU, which is what makes them the correct subject for the
 *   rejection scenario.
 *
 *   Runs inside sched_foreach(), which holds a critical section, so this
 *   does nothing but compare and store.
 *
 ****************************************************************************/

static void stc_aff_find_idle_cb(struct tcb_s *tcb, void *arg)
{
	if (tcb == NULL || g_idle_pid != 0) {
		return;
	}

	/* Skip the caller, and pick an IDLE task that is not PID 0, because
	 * PID 0 means "this task" to sched_setaffinity() and would test
	 * something else entirely.
	 */

	if (tcb->pid == g_scan_self || tcb->pid == 0) {
		return;
	}

	if (tcb->sched_priority == 0) {
		g_idle_pid = tcb->pid;
	}
}

/****************************************************************************
 * Name: stc_aff_find_idle
 ****************************************************************************/

static pid_t stc_aff_find_idle(void)
{
	int fd = tc_get_drvfd();

	g_idle_pid = 0;
	g_scan_self = getpid();

	(void)ioctl(fd, TESTIOC_SCHED_FOREACH,
		    (unsigned long)stc_aff_find_idle_cb);

	return g_idle_pid;
}

/****************************************************************************
 * Name: stc_sched_aff01_pinned_stays_put
 *
 * @testcase             :stc_sched_aff01_pinned_stays_put
 * @brief                :SCN-AFF-01, a pinned task never runs elsewhere
 * @scenario             :Pin two CPU bound tasks to the highest numbered
 *                        CPU and have each of them call sched_getcpu() on
 *                        every iteration, recording both the set of CPUs it
 *                        was observed on and any sample taken outside its
 *                        mask.  Because the two share one CPU they are
 *                        preempted constantly, so the samples span a large
 *                        number of context switches rather than a single
 *                        placement decision.
 *                        The oracle is absolute: not one sample outside the
 *                        mask, over at least a thousand samples.  A single
 *                        violation means the mask was not honoured.
 * @apicovered           :sched_setaffinity, sched_getcpu, task_create
 * @precondition         :CONFIG_SMP_NCPUS greater than one
 * @postcondition        :all workers exited
 * @return               :void
 *
 ****************************************************************************/

static void stc_sched_aff01_pinned_stays_put(void)
{
	struct stc_workload_s wl;
	int ncpus = stc_ncpus();
	int nworkers = 2;
	int target = ncpus - 1;
	int run_ret;
	int i;

	stc_workload_init(&wl, nworkers, SCHED_PRIORITY_DEFAULT, SCHED_RR);
	wl.window_ms = STC_AFF_WINDOW_MS;
	wl.pinned = true;
	wl.sample_cpu = true;
	CPU_ZERO(&wl.affinity);
	CPU_SET(target, &wl.affinity);

	run_ret = stc_run_workload(&wl);

	printf("[SCN-AFF-01] target=CPU%d seen=0x%x,0x%x violations=%u,%u\n",
	       target, (unsigned int)stc_cpu_seen_mask(0),
	       (unsigned int)stc_cpu_seen_mask(1),
	       (unsigned int)stc_cpu_violations(0),
	       (unsigned int)stc_cpu_violations(1));

	TC_ASSERT_EQ("aff01_workload", run_ret, OK);

	for (i = 0; i < nworkers; i++) {
		/* A zero violation count only means something if samples were
		 * actually taken.
		 */

		TC_ASSERT_GT("aff01_sampled", stc_count(i),
			     STC_AFF_MIN_SAMPLES);

		TC_ASSERT_EQ("aff01_no_violation", stc_cpu_violations(i), 0);

		/* And it was seen on the CPU it was pinned to, not merely
		 * never seen anywhere.
		 */

		TC_ASSERT_EQ("aff01_seen_target", stc_cpu_seen_mask(i),
			     (uint32_t)(1u << target));
	}

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: stc_sched_aff02_migrate_running_task
 *
 * @testcase             :stc_sched_aff02_migrate_running_task
 * @brief                :SCN-AFF-02, narrowing affinity migrates a running
 *                        task
 * @scenario             :Start one CPU bound task with no restriction and
 *                        observe where it runs.  Then narrow its mask to
 *                        exclude the CPU it is on and observe a second
 *                        window.
 *                        This is the harder half of affinity: the task is
 *                        not merely placed at creation but is already
 *                        running somewhere it is no longer permitted to be,
 *                        so the scheduler has to move it.  After the change
 *                        it must never be observed on the excluded CPU
 *                        again.
 * @apicovered           :sched_setaffinity, sched_getcpu, task_create
 * @precondition         :CONFIG_SMP_NCPUS greater than one
 * @postcondition        :all workers exited
 * @return               :void
 *
 ****************************************************************************/

static void stc_sched_aff02_migrate_running_task(void)
{
	struct stc_workload_s wl;
	cpu_set_t narrowed = 0;
	int ncpus = stc_ncpus();
	int target = ncpus - 1;
	int start_ret;
	int set_ret = ERROR;
	int finish_ret;
	uint32_t seen_after = 0;
	uint32_t violations_after = 0;
	uint32_t samples_after = 0;

	stc_workload_init(&wl, 1, SCHED_PRIORITY_DEFAULT, SCHED_RR);
	wl.window_ms = STC_AFF_WINDOW_MS;
	wl.sample_cpu = true;

	start_ret = stc_workload_start(&wl);

	if (start_ret == OK) {
		/* First window: unrestricted, mask of zero means every sample
		 * is acceptable.
		 */

		stc_workload_sample(STC_AFF_WINDOW_MS);

		CPU_SET(target, &narrowed);
		set_ret = sched_setaffinity(stc_worker_pid(0),
					    sizeof(cpu_set_t), &narrowed);

		/* Judge subsequent samples against the new mask. */

		stc_set_allowed(0, narrowed);

		/* Let the migration actually happen before measuring. */

		usleep(STC_AFF_SETTLE_MS * 1000);

		stc_workload_sample(STC_AFF_WINDOW_MS);

		seen_after = stc_cpu_seen_mask(0);
		violations_after = stc_cpu_violations(0);
		samples_after = stc_count(0);
	}

	finish_ret = stc_workload_finish(1);

	printf("[SCN-AFF-02] narrowed to CPU%d seen_after=0x%x violations=%u\n",
	       target, (unsigned int)seen_after,
	       (unsigned int)violations_after);

	TC_ASSERT_EQ("aff02_workload_start", start_ret, OK);
	TC_ASSERT_EQ("aff02_setaffinity", set_ret, OK);
	TC_ASSERT_EQ("aff02_workload_finish", finish_ret, OK);

	TC_ASSERT_GT("aff02_sampled", samples_after, STC_AFF_MIN_SAMPLES);

	/* It kept running, and only where it was now allowed to. */

	TC_ASSERT_EQ("aff02_no_violation", violations_after, 0);
	TC_ASSERT_EQ("aff02_seen_target", seen_after,
		     (uint32_t)(1u << target));

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: stc_sched_aff03_uneven_pinning
 *
 * @testcase             :stc_sched_aff03_uneven_pinning
 * @brief                :SCN-AFF-03, a crowded CPU does not starve a quiet
 *                        one
 * @scenario             :Pin two CPU bound tasks to CPU0 and one to the
 *                        highest numbered CPU, so the two run queues are
 *                        deliberately unbalanced.  Every task must still
 *                        make progress, and the lone task on the quiet CPU
 *                        should complete at least as much work as either of
 *                        the two sharing the busy one.
 *                        This catches a scheduler that lets queue depth on
 *                        one CPU influence decisions on another, and it also
 *                        confirms that pinned tasks are not silently
 *                        migrated to balance the load, which would violate
 *                        their masks.
 * @apicovered           :sched_setaffinity, sched_getcpu, task_create
 * @precondition         :CONFIG_SMP_NCPUS greater than one
 * @postcondition        :all workers exited
 * @return               :void
 *
 ****************************************************************************/

static void stc_sched_aff03_uneven_pinning(void)
{
	struct stc_workload_s wl;
	cpu_set_t masks[3];
	int ncpus = stc_ncpus();
	int quiet = ncpus - 1;
	int nworkers = 3;
	int run_ret;
	int i;

	masks[0] = 0;
	masks[1] = 0;
	masks[2] = 0;
	CPU_SET(0, &masks[0]);
	CPU_SET(0, &masks[1]);
	CPU_SET(quiet, &masks[2]);

	stc_workload_init(&wl, nworkers, SCHED_PRIORITY_DEFAULT, SCHED_RR);
	wl.window_ms = STC_AFF_WINDOW_MS;
	wl.affinities = masks;
	wl.sample_cpu = true;

	run_ret = stc_run_workload(&wl);

	stc_report("SCN-AFF-03", nworkers);

	TC_ASSERT_EQ("aff03_workload", run_ret, OK);

	/* Nobody starved, and nobody strayed. */

	for (i = 0; i < nworkers; i++) {
		TC_ASSERT_GT("aff03_all_ran", stc_count(i),
			     STC_AFF_MIN_SAMPLES);
		TC_ASSERT_EQ("aff03_no_violation", stc_cpu_violations(i), 0);
	}

	/* The lone task on the quiet CPU has it to itself, so it cannot have
	 * done less work than either task sharing the busy one.
	 */

	TC_ASSERT_GEQ("aff03_quiet_not_starved", stc_count(2), stc_count(0));
	TC_ASSERT_GEQ("aff03_quiet_not_starved", stc_count(2), stc_count(1));

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: stc_sched_aff04_spread_across_cpus
 *
 * @testcase             :stc_sched_aff04_spread_across_cpus
 * @brief                :SCN-AFF-04, unpinned tasks spread across the CPUs
 * @scenario             :Create exactly as many unpinned CPU bound tasks as
 *                        there are CPUs.  With no affinity restriction and
 *                        no oversubscription the scheduler should place one
 *                        on each CPU, leaving none idle while work is
 *                        available.
 *                        The union of the CPUs observed across all workers
 *                        must therefore cover every CPU.  A scheduler that
 *                        crowds every task onto one CPU still produces
 *                        healthy looking per task counters, so counting work
 *                        alone would not detect it; only the placement
 *                        observation does.
 * @apicovered           :sched_getcpu, task_create, sched_getcpucount
 * @precondition         :CONFIG_SMP_NCPUS greater than one
 * @postcondition        :all workers exited
 * @return               :void
 *
 ****************************************************************************/

static void stc_sched_aff04_spread_across_cpus(void)
{
	struct stc_workload_s wl;
	uint32_t union_mask = 0;
	uint32_t all_cpus;
	int ncpus = stc_ncpus();
	int run_ret;
	int i;

	if (ncpus > STC_MAX_WORKERS) {
		ncpus = STC_MAX_WORKERS;
	}

	all_cpus = (ncpus >= 32) ? 0xffffffffu : ((1u << ncpus) - 1u);

	stc_workload_init(&wl, ncpus, SCHED_PRIORITY_DEFAULT, SCHED_RR);
	wl.window_ms = STC_AFF_WINDOW_MS;
	wl.sample_cpu = true;

	run_ret = stc_run_workload(&wl);

	for (i = 0; i < ncpus; i++) {
		union_mask |= stc_cpu_seen_mask(i);
	}

	printf("[SCN-AFF-04] union=0x%x expected=0x%x\n",
	       (unsigned int)union_mask, (unsigned int)all_cpus);

	TC_ASSERT_EQ("aff04_workload", run_ret, OK);

	for (i = 0; i < ncpus; i++) {
		TC_ASSERT_GT("aff04_all_ran", stc_count(i),
			     STC_AFF_MIN_SAMPLES);
	}

	/* Every CPU carried some of the load. */

	TC_ASSERT_EQ("aff04_all_cpus_used", union_mask, all_cpus);

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: stc_sched_aff05_idle_rejected
 *
 * @testcase             :stc_sched_aff05_idle_rejected
 * @brief                :SCN-AFF-05, the affinity of an IDLE task cannot be
 *                        changed
 * @scenario             :Locate an IDLE task by walking the task lists and
 *                        picking the one at priority zero, then attempt to
 *                        change its affinity.
 *                        The IDLE tasks are the only tasks in the system
 *                        locked to a CPU, and moving one would leave a CPU
 *                        with nothing to fall back to, so the request has to
 *                        be refused with EINVAL.  The task is found by scan
 *                        rather than by assuming a PID, because the mapping
 *                        of IDLE tasks to low PIDs is an implementation
 *                        detail this test should not depend on.
 * @apicovered           :sched_setaffinity, sched_foreach via the test driver
 * @precondition         :CONFIG_SMP_NCPUS greater than one
 * @postcondition        :none
 * @return               :void
 *
 ****************************************************************************/

static void stc_sched_aff05_idle_rejected(void)
{
	cpu_set_t mask = 0;
	pid_t idle;
	int ret;

	idle = stc_aff_find_idle();

	printf("[SCN-AFF-05] idle pid=%d\n", (int)idle);

	TC_ASSERT_GT("aff05_found_idle", idle, 0);

	CPU_SET(0, &mask);
	ret = sched_setaffinity(idle, sizeof(cpu_set_t), &mask);

	/* TizenRT's sched_setaffinity() returns the negative errno directly
	 * rather than returning ERROR and setting errno, which is what POSIX
	 * specifies.  The existing API level test asserts the same thing on
	 * its non-SMP path, so the convention is settled even though it is
	 * not the standard one; assert what the implementation does.
	 */

	TC_ASSERT_EQ("aff05_rejected", ret, -EINVAL);

	TC_SUCCESS_RESULT();
}

#endif							/* CONFIG_SMP */

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stc_sched_affinity_main
 ****************************************************************************/

int stc_sched_affinity_main(void)
{
#ifdef CONFIG_SMP
	if (stc_ncpus() < 2) {
		printf("[SCN-AFF] skipped, single CPU configuration\n");
		return 0;
	}

	printf("[SCN-AFF] affinity and placement scenarios, ncpus=%d\n",
	       stc_ncpus());

	stc_sched_aff01_pinned_stays_put();
	stc_sched_aff02_migrate_running_task();
	stc_sched_aff03_uneven_pinning();
	stc_sched_aff04_spread_across_cpus();
	stc_sched_aff05_idle_rejected();
#else
	printf("[SCN-AFF] skipped, built without CONFIG_SMP\n");
#endif

	return 0;
}
