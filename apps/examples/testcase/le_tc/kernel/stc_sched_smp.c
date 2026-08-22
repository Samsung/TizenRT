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

/// @file stc_sched_smp.c

/// @brief Scenario test cases for SMP structural invariants (family H)
///
/// Where the round-robin scenarios each target one known mechanism, the
/// scenario here targets a property: no runnable task is passed over
/// indefinitely.  It holds no matter which part of the scheduler is at
/// fault, so it detects starvation defects that no specific scenario was
/// written for.

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdio.h>
#include <sched.h>

#include "tc_internal.h"
#include "stc_sched_common.h"
#include "stc_sched_monitor.h"

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stc_sched_smp01_starvation_invariant
 *
 * @testcase             :stc_sched_smp01_starvation_invariant
 * @brief                :SCN-SMP-01, starvation invariant under steady load
 * @scenario             :Start the background monitor over the priority band
 *                        under test, then apply an oversubscribed workload
 *                        of ncpus + 1 equal priority CPU bound tasks.  The
 *                        monitor samples the task lists every 10 ms and
 *                        measures, for each task, how long it stays
 *                        continuously ready to run without being given a
 *                        CPU.  Any task that exceeds the grace period is a
 *                        violation.
 *                        This is deliberately stated as a property rather
 *                        than as a mechanism.  It does not ask why a task
 *                        was passed over, only whether it was, so it holds
 *                        against defects in timeslice handling, CPU
 *                        selection, affinity filtering or list management
 *                        alike.
 * @apicovered           :sched_foreach via the OS API test driver,
 *                        task_create, sched_setscheduler
 * @precondition         :CONFIG_RR_INTERVAL greater than zero
 * @postcondition        :monitor stopped, all workers exited
 * @return               :void
 *
 ****************************************************************************/

static void stc_sched_smp01_starvation_invariant(void)
{
	struct stc_workload_s wl;
	int ncpus = stc_ncpus();
	int nworkers = ncpus + 1;
	int mon_started;
	int run_ret;
	int stop_ret;

	stc_workload_init(&wl, nworkers, SCHED_PRIORITY_DEFAULT, SCHED_RR);

	mon_started = stc_monitor_start(SCHED_PRIORITY_DEFAULT);

	run_ret = stc_run_workload(&wl);

	/* Stop the monitor before any assertion.  An assertion returns from
	 * this function immediately, so asserting first would leave the
	 * monitor task running and poison every scenario after this one.
	 */

	stop_ret = (mon_started == OK) ? stc_monitor_stop() : ERROR;

	stc_report("SCN-SMP-01", nworkers);
	stc_monitor_report("SCN-SMP-01");

	TC_ASSERT_EQ("smp01_monitor_start", mon_started, OK);
	TC_ASSERT_EQ("smp01_workload", run_ret, OK);
	TC_ASSERT_EQ("smp01_monitor_stop", stop_ret, OK);

	/* The monitor has to have actually observed something, otherwise a
	 * zero violation count would be vacuous.
	 */

	TC_ASSERT_GT("smp01_monitor_sampled", stc_monitor_samples(), 0);

	/* The invariant itself. */

	TC_ASSERT_EQ("smp01_no_starvation", stc_monitor_violations(), 0);

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stc_sched_smp_main
 ****************************************************************************/

int stc_sched_smp_main(void)
{
	printf("[SCN-SMP] structural invariant scenarios, ncpus=%d\n",
	       stc_ncpus());

#ifdef CONFIG_SMP
	stc_sched_smp01_starvation_invariant();
#else
	printf("[SCN-SMP] skipped, single CPU configuration\n");
#endif

	return 0;
}
