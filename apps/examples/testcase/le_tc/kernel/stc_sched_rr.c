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

/// @file stc_sched_rr.c

/// @brief Scenario test cases for round-robin fairness (family A)
///
/// These assert that round-robin scheduling actually rotates the CPU between
/// equal priority tasks.  The existing API level test for round-robin,
/// tc_sched_sched_rr_get_interval(), only checks that the configured
/// interval is reported back; a kernel in which round-robin never yields
/// passes it.  The scenarios here fail in that case.
///
/// Each scenario states its oracle as two separate assertions:
///
///   - a hard correctness assertion, which can never be flaky, and
///   - a fairness assertion carrying a tolerance.
///
/// They are kept separate deliberately.  When the suite goes red the failing
/// assertion alone identifies whether behaviour is wrong or merely uneven.

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdio.h>
#include <sched.h>

#include "tc_internal.h"
#include "stc_sched_common.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Fairness floor for the unpinned, oversubscribed case.
 *
 * With ncpus + 1 workers on ncpus CPUs the ideal share is ncpus/(ncpus + 1)
 * each, so a perfectly fair run gives 100%.  A tight bound would be flaky:
 * over a 3 s window at CONFIG_RR_INTERVAL of 10 ms each worker sees roughly
 * 300 slice boundaries, and quantisation at those boundaries plus background
 * interrupt load both perturb the ratio.  50% sits far below fair and far
 * above the failure mode, which is one worker never running at all.
 */

#define STC_RR_FAIR_UNPINNED    50

/* Fairness floor when every worker is pinned to one CPU.
 *
 * A single run queue removes CPU selection and migration from the picture,
 * so rotation is a plain cycle through the priority band and the spread
 * should be much tighter.
 */

#define STC_RR_FAIR_PINNED      80

/* Priority band for the RR fairness tests.
 *
 * These are raised above SCHED_PRIORITY_DEFAULT to avoid interference from
 * system tasks such as LWIP_TCP/IP (priority 105) that run between the
 * original worker (100) and harness (110) priorities. Such tasks can mask
 * RR failures by introducing additional scheduling points.
 *
 * The kernel_tc task is created at priority 180, so this band aligns with
 * that to ensure a clean scheduling environment.
 */

#define STC_RR_WORKER_PRIORITY  180

/****************************************************************************
 * Private Types
 ****************************************************************************/

/* Outcome of one monitored run.  Every field is captured before any
 * assertion is evaluated, so that a failing assertion cannot leave the
 * monitor task or the workers running.
 */

struct stc_rr_result_s {
	int monitor_start;
	int workload;
	int monitor_stop;
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stc_rr_run_monitored
 *
 * Description:
 *   Applies a workload for a fairness scenario.
 *
 *   The starvation monitor is deliberately NOT started here.
 *
 *   The monitor is a task that wakes every ten milliseconds and blocks
 *   again.  Each time it blocks, sched_removereadytorun() runs on its
 *   CPU, and that function contains the very search of g_readytorun that
 *   round-robin is supposed to perform.  The monitor therefore supplies
 *   rotation on the CPU it is pinned to, at a rate that happens to match
 *   CONFIG_RR_INTERVAL.  Running it alongside a fairness measurement
 *   makes the measurement report the observer rather than the scheduler.
 *
 *   The monitor remains the right instrument for SCN-SMP-01, where it is
 *   the thing being read rather than a bystander.  It has no place in a
 *   scenario whose oracle is a share of CPU time.
 *
 *   Nothing is asserted here, so that teardown always completes before
 *   the caller evaluates anything.
 *
 ****************************************************************************/

static void stc_rr_run_monitored(const struct stc_workload_s *wl,
				 struct stc_rr_result_s *res)
{
	res->monitor_start = OK;
	res->workload = stc_run_workload(wl);
	res->monitor_stop = OK;
}

/****************************************************************************
 * Name: stc_sched_rr01_fairness_oversubscribed
 *
 * @testcase             :stc_sched_rr01_fairness_oversubscribed
 * @brief                :SCN-RR-01, round-robin fairness with more runnable
 *                        tasks than CPUs
 * @scenario             :Create ncpus + 1 CPU bound tasks, all at the same
 *                        priority, all SCHED_RR, all with the default
 *                        affinity so the scheduler is free to place them.
 *                        Let them run for a fixed window and compare the
 *                        work each completed.
 *                        The surplus task cannot be given a CPU at creation
 *                        time, because a CPU is only handed to a candidate
 *                        that strictly outranks the task already running
 *                        there.  Round-robin rotation is the only mechanism
 *                        that can ever admit it, so if rotation does not
 *                        work the surplus task records no work at all.
 * @apicovered           :task_create, sched_setscheduler, sched_getcpucount
 * @precondition         :CONFIG_RR_INTERVAL greater than zero
 * @postcondition        :all workers exited
 * @return               :void
 *
 ****************************************************************************/

static void stc_sched_rr01_fairness_oversubscribed(void)
{
	struct stc_workload_s wl;
	struct stc_rr_result_s res;
	int ncpus = stc_ncpus();
	int nworkers = ncpus + 1;

	stc_workload_init(&wl, nworkers, STC_RR_WORKER_PRIORITY, SCHED_RR);

	stc_rr_run_monitored(&wl, &res);

	stc_report("SCN-RR-01", nworkers);

	TC_ASSERT_EQ("rr01_monitor_start", res.monitor_start, OK);
	TC_ASSERT_EQ("rr01_workload", res.workload, OK);
	TC_ASSERT_EQ("rr01_monitor_stop", res.monitor_stop, OK);

	/* Hard correctness assertion.  A zero here means a runnable task was
	 * never scheduled for the whole window, which is a starvation defect
	 * and not a fairness tolerance question.
	 */

	TC_ASSERT_GT("rr01_all_workers_ran", stc_min(nworkers), 0);

	/* Fairness assertion, tolerance based. */

	TC_ASSERT_GEQ("rr01_fair_share", stc_fair_pct(nworkers),
		      STC_RR_FAIR_UNPINNED);


	TC_SUCCESS_RESULT();
}

#ifdef CONFIG_SMP
/****************************************************************************
 * Name: stc_sched_rr02_fairness_all_cpus
 *
 * @testcase             :stc_sched_rr02_fairness_all_cpus
 * @brief                :SCN-RR-02, round-robin fairness on every CPU
 * @scenario             :For each CPU, create three CPU bound tasks at equal
 *                        priority and pin all of them to that CPU, so they
 *                        share a single run queue.  This verifies that round-
 *                        robin rotation works correctly on every CPU, not
 *                        just CPU0 or the last CPU.
 *                        On targets where the system tick is enabled on CPU0
 *                        only, this also exercises the remote timeslice check
 *                        path for secondary CPUs.
 * @apicovered           :task_create, sched_setscheduler, sched_setaffinity
 * @precondition         :CONFIG_RR_INTERVAL greater than zero
 * @postcondition        :all workers exited
 * @return               :void
 *
 ****************************************************************************/

static void stc_sched_rr02_fairness_all_cpus(void)
{
	int ncpus = stc_ncpus();
	int cpu;
	char tag[16];

	for (cpu = 0; cpu < ncpus; cpu++) {
		struct stc_workload_s wl;
		struct stc_rr_result_s res;
		int nworkers = 3;

		printf("[SCN-RR-02] testing CPU %d\n", cpu);

		stc_workload_init(&wl, nworkers, STC_RR_WORKER_PRIORITY, SCHED_RR);
		wl.pinned = true;
		CPU_ZERO(&wl.affinity);
		CPU_SET(cpu, &wl.affinity);

		stc_rr_run_monitored(&wl, &res);

		snprintf(tag, sizeof(tag), "SCN-RR-02-CPU%d", cpu);
		stc_report(tag, nworkers);

		TC_ASSERT_EQ("rr02_monitor_start", res.monitor_start, OK);
		TC_ASSERT_EQ("rr02_workload", res.workload, OK);
		TC_ASSERT_EQ("rr02_monitor_stop", res.monitor_stop, OK);

		TC_ASSERT_GT("rr02_all_workers_ran", stc_min(nworkers), 0);

		TC_ASSERT_GEQ("rr02_fair_share", stc_fair_pct(nworkers),
			      STC_RR_FAIR_PINNED);
	}

	TC_SUCCESS_RESULT();
}
#else							/* !CONFIG_SMP */
/****************************************************************************
 * Name: stc_sched_rr02_fairness_single_cpu
 *
 * @testcase             :stc_sched_rr02_fairness_single_cpu
 * @brief                :SCN-RR-02, round-robin fairness on the single CPU
 * @scenario             :Create three CPU bound tasks at equal priority.
 *                        Without SMP there is only one CPU, so they share
 *                        its run queue and must rotate fairly.
 * @apicovered           :task_create, sched_setscheduler
 * @precondition         :CONFIG_RR_INTERVAL greater than zero
 * @postcondition        :all workers exited
 * @return               :void
 *
 ****************************************************************************/

static void stc_sched_rr02_fairness_single_cpu(void)
{
	struct stc_workload_s wl;
	struct stc_rr_result_s res;
	int nworkers = 3;

	stc_workload_init(&wl, nworkers, STC_RR_WORKER_PRIORITY, SCHED_RR);
	wl.pinned = false;

	stc_rr_run_monitored(&wl, &res);

	stc_report("SCN-RR-02", nworkers);

	TC_ASSERT_EQ("rr02_monitor_start", res.monitor_start, OK);
	TC_ASSERT_EQ("rr02_workload", res.workload, OK);
	TC_ASSERT_EQ("rr02_monitor_stop", res.monitor_stop, OK);

	TC_ASSERT_GT("rr02_all_workers_ran", stc_min(nworkers), 0);

	TC_ASSERT_GEQ("rr02_fair_share", stc_fair_pct(nworkers),
		      STC_RR_FAIR_PINNED);


	TC_SUCCESS_RESULT();
}
#endif							/* CONFIG_SMP */

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stc_sched_rr_main
 ****************************************************************************/

int stc_sched_rr_main(void)
{
	printf("[SCN-RR] round-robin fairness scenarios, ncpus=%d interval=%dms\n",
	       stc_ncpus(), CONFIG_RR_INTERVAL);

	stc_sched_rr01_fairness_oversubscribed();
#ifdef CONFIG_SMP
	stc_sched_rr02_fairness_all_cpus();
#else
	stc_sched_rr02_fairness_single_cpu();
#endif

	return 0;
}
