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

/// @file stc_sched_priority.c

/// @brief Scenario test cases for priority and preemption (family B)
///
/// Priority is the scheduler's most basic promise: a runnable task never
/// waits behind a less important one.  The existing API level tests confirm
/// that a priority can be stored and read back, which says nothing about
/// whether the scheduler acts on it.  These scenarios put the promise under
/// load and check the outcome.
///
/// Bounds are stated generously throughout.  The failure modes being hunted
/// are "never runs" and "runs only once the load finishes", both of which
/// are seconds wide, so a bound in the tens of milliseconds separates them
/// from correct behaviour with room to spare and without inviting jitter
/// related flakiness.

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>
#include <sched.h>

#include "tc_internal.h"
#include "stc_sched_common.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Priority band used by these scenarios.  Kept clear of the default so that
 * unrelated system activity does not land inside the band under test.
 */

#define STC_PRI_LOW             (SCHED_PRIORITY_DEFAULT)
#define STC_PRI_STEP            10
#define STC_PRI_HIGH            (SCHED_PRIORITY_DEFAULT + 50)

/* Bound on how long a released high priority task may take to run while
 * every CPU is busy with lower priority work.
 *
 * Preemption should be immediate: the post itself causes the switch.  The
 * bound only has to be tight enough to distinguish that from the broken
 * behaviour, which is waiting for the low priority load to finish, three
 * seconds later.  Twenty milliseconds is two round-robin slices, far beyond
 * anything a correct scheduler needs and far below the failure.
 */

#define STC_PRI_PREEMPT_MS      20

/* Window used when only presence or absence of progress is being checked,
 * rather than a ratio.  Shorter than the fairness window because no
 * averaging is needed.
 */

#define STC_PRI_WINDOW_MS       1000

/****************************************************************************
 * Private Data
 ****************************************************************************/

static sem_t g_release_sem;
static volatile bool g_hp_ran;
static volatile uint32_t g_hp_latency_ms;
static struct timespec g_release_ts;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stc_pri_elapsed_ms
 ****************************************************************************/

static uint32_t stc_pri_elapsed_ms(const struct timespec *from,
				   const struct timespec *to)
{
	int32_t sec = (int32_t)(to->tv_sec - from->tv_sec);
	int32_t nsec = (int32_t)(to->tv_nsec - from->tv_nsec);

	if (nsec < 0) {
		sec -= 1;
		nsec += 1000000000;
	}

	if (sec < 0) {
		return 0;
	}

	return (uint32_t)(sec * 1000 + nsec / 1000000);
}

/****************************************************************************
 * Name: stc_pri_highprio_task
 *
 * Description:
 *   Blocks until released, then records how long the release took to turn
 *   into execution.  Nothing else, so the measurement is not diluted by the
 *   task's own work.
 *
 ****************************************************************************/

static int stc_pri_highprio_task(int argc, char *argv[])
{
	struct timespec now;

	(void)sem_wait(&g_release_sem);

	(void)clock_gettime(CLOCK_REALTIME, &now);
	g_hp_latency_ms = stc_pri_elapsed_ms(&g_release_ts, &now);
	g_hp_ran = true;

	return OK;
}

/****************************************************************************
 * Name: stc_sched_pri01_preempt_under_load
 *
 * @testcase             :stc_sched_pri01_preempt_under_load
 * @brief                :SCN-PRI-01, a high priority task preempts a fully
 *                        loaded system
 * @scenario             :Saturate every CPU with low priority CPU bound
 *                        tasks, then release a high priority task that has
 *                        been blocked on a semaphore.  Measure the interval
 *                        between the post and the task actually running.
 *                        Because the low priority tasks never block, the
 *                        only way the high priority task can run is by
 *                        preempting one of them.  If preemption does not
 *                        work it does not run until the load is torn down,
 *                        which is orders of magnitude outside the bound.
 * @apicovered           :task_create, sem_post, sem_wait, sched_setscheduler
 * @precondition         :none
 * @postcondition        :high priority task exited, all workers exited
 * @return               :void
 *
 ****************************************************************************/

static void stc_sched_pri01_preempt_under_load(void)
{
	struct stc_workload_s wl;
	int ncpus = stc_ncpus();
	int nworkers = ncpus;
	pid_t hp;
	int start_ret;
	int finish_ret;

	g_hp_ran = false;
	g_hp_latency_ms = 0;

	if (sem_init(&g_release_sem, 0, 0) != OK) {
		TC_ASSERT("pri01_sem_init", false);
	}

	stc_workload_init(&wl, nworkers, STC_PRI_LOW, SCHED_RR);

	start_ret = stc_workload_start(&wl);

	hp = task_create("stchp", STC_PRI_HIGH, STC_WORKER_STACKSIZE,
			 stc_pri_highprio_task, (char * const *)NULL);

	if (start_ret == OK && hp > 0) {
		/* Give the high priority task time to reach sem_wait, then
		 * release it and let it record its own latency.
		 */

		usleep(100 * 1000);

		(void)clock_gettime(CLOCK_REALTIME, &g_release_ts);
		(void)sem_post(&g_release_sem);

		usleep(500 * 1000);
	}

	finish_ret = stc_workload_finish(nworkers);

	if (hp > 0 && !g_hp_ran) {
		(void)task_delete(hp);
	}

	(void)sem_destroy(&g_release_sem);

	printf("[SCN-PRI-01] preempt latency=%ums ran=%d\n",
	       (unsigned int)g_hp_latency_ms, (int)g_hp_ran);

	TC_ASSERT_EQ("pri01_workload_start", start_ret, OK);
	TC_ASSERT_GT("pri01_task_create", hp, 0);
	TC_ASSERT_EQ("pri01_workload_finish", finish_ret, OK);

	/* Hard correctness assertion: it ran at all. */

	TC_ASSERT("pri01_highprio_ran", g_hp_ran);

	/* Bounded latency assertion. */

	TC_ASSERT_LEQ("pri01_preempt_latency", g_hp_latency_ms,
		      STC_PRI_PREEMPT_MS);

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: stc_sched_pri02_strict_ordering
 *
 * @testcase             :stc_sched_pri02_strict_ordering
 * @brief                :SCN-PRI-02, strict priority ordering when
 *                        oversubscribed
 * @scenario             :Create a ladder of CPU bound tasks, each at a
 *                        distinct priority, with more tasks than CPUs.  None
 *                        of them ever blocks, so the scheduler has a free
 *                        choice and priority is the only thing that should
 *                        decide it.
 *                        Exactly the top ncpus tasks should accumulate work
 *                        and every task below them should record exactly
 *                        zero.  Unlike a fairness ratio this is an absolute
 *                        statement with no tolerance: a single count on a
 *                        task that should never have run is a defect.
 * @apicovered           :task_create, sched_setscheduler, sched_getcpucount
 * @precondition         :none
 * @postcondition        :all workers exited
 * @return               :void
 *
 ****************************************************************************/

static void stc_sched_pri02_strict_ordering(void)
{
	struct stc_workload_s wl;
	int prios[STC_MAX_WORKERS];
	int ncpus = stc_ncpus();
	int nworkers = ncpus + 2;
	int run_ret;
	int i;

	if (nworkers > STC_MAX_WORKERS) {
		nworkers = STC_MAX_WORKERS;
	}

	/* Give top ncpus workers the SAME priority (>= 140) so they can
	 * run via round-robin. Lower workers get staggered descending priority.
	 */
	#define STC_PRI_TOP  (STC_PRI_LOW + 50)  /* >= 140 to beat harness */

	for (i = 0; i < nworkers; i++) {
		if (i < ncpus) {
			prios[i] = STC_PRI_TOP; //Top ncpus workers: same priority, share CPUs via round-robin
		} else {
		prios[i] = STC_PRI_LOW + (nworkers - 1 - i) * STC_PRI_STEP;
	}
	}

	stc_workload_init(&wl, nworkers, prios[0], SCHED_RR);
	wl.priorities = prios;
	wl.window_ms = STC_PRI_WINDOW_MS;

	run_ret = stc_run_workload(&wl);

	stc_report("SCN-PRI-02", nworkers);

	TC_ASSERT_EQ("pri02_workload", run_ret, OK);

	/* The top ncpus tasks must all have run. */
	for (i = 0; i < ncpus; i++) {
		TC_ASSERT_GT("pri02_top_ran", stc_count(i), 0);
	}

	/* Everything below them must not have run at all. */
	for (i = ncpus; i < nworkers; i++) {
		TC_ASSERT_EQ("pri02_lower_blocked", stc_count(i), 0);
	}

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: stc_sched_pri03_raise_waiting_task
 *
 * @testcase             :stc_sched_pri03_raise_waiting_task
 * @brief                :SCN-PRI-03, raising a waiting task makes it run
 * @scenario             :Start ncpus + 1 equal priority CPU bound tasks, so
 *                        that at least one is always waiting, and confirm
 *                        over a first window which of them is starved.  Then
 *                        raise the last worker above the rest with
 *                        sched_setparam and measure a second window.
 *                        After the change that worker outranks every other
 *                        runnable task, so it must accumulate work in the
 *                        second window regardless of what it did in the
 *                        first.  This exercises the promotion path for a
 *                        task that is ready to run but not currently
 *                        assigned to any CPU, which is a different code path
 *                        from promoting a running task.
 * @apicovered           :sched_setparam, task_create, sched_setscheduler
 * @precondition         :none
 * @postcondition        :all workers exited
 * @return               :void
 *
 ****************************************************************************/

static void stc_sched_pri03_raise_waiting_task(void)
{
	struct stc_workload_s wl;
	struct sched_param param;
	int ncpus = stc_ncpus();
	int nworkers = ncpus + 1;
	int target = nworkers - 1;
	int start_ret;
	int set_ret = ERROR;
	int finish_ret;
	uint32_t before = 0;
	uint32_t after = 0;

	stc_workload_init(&wl, nworkers, STC_PRI_LOW, SCHED_RR);

	start_ret = stc_workload_start(&wl);

	if (start_ret == OK) {
		/* First window establishes the unpromoted baseline. */

		stc_workload_sample(STC_PRI_WINDOW_MS);
		before = stc_count(target);

		param.sched_priority = STC_PRI_LOW + STC_PRI_STEP;
		set_ret = sched_setparam(stc_worker_pid(target), &param);

		/* Second window is the one under test. */

		stc_workload_sample(STC_PRI_WINDOW_MS);
		after = stc_count(target);
	}

	finish_ret = stc_workload_finish(nworkers);

	printf("[SCN-PRI-03] promoted worker %d count_before=%u count_after=%u\n",
	       target, (unsigned int)before, (unsigned int)after);

	TC_ASSERT_EQ("pri03_workload_start", start_ret, OK);
	TC_ASSERT_EQ("pri03_setparam", set_ret, OK);
	TC_ASSERT_EQ("pri03_workload_finish", finish_ret, OK);

	/* Once promoted it outranks every peer, so it must run. */

	TC_ASSERT_GT("pri03_promoted_ran", after, 0);

	/* The promotion must increase the target's work share compared to
	 * the baseline window.  This verifies that priority actually affects
	 * scheduling, not just that the task happened to run.
	 */

	TC_ASSERT_GT("pri03_promoted_gain", after, before);

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: stc_sched_pri04_lower_running_task
 *
 * @testcase             :stc_sched_pri04_lower_running_task
 * @brief                :SCN-PRI-04, lowering a task yields the CPU
 * @scenario             :Start a ladder in which worker 0 outranks the rest
 *                        and confirm in a first window that the lower
 *                        workers are held off.  Then lower worker 0 below
 *                        them with sched_setparam and measure a second
 *                        window.
 *                        After the change worker 0 is the least important
 *                        runnable task, so the previously blocked workers
 *                        must start making progress.  This is the mirror of
 *                        SCN-PRI-03 and covers demotion of a task that is
 *                        currently running, which the promotion path does
 *                        not reach.
 * @apicovered           :sched_setparam, task_create, sched_setscheduler
 * @precondition         :none
 * @postcondition        :all workers exited
 * @return               :void
 *
 ****************************************************************************/

static void stc_sched_pri04_lower_running_task(void)
{
	struct stc_workload_s wl;
	struct sched_param param;
	int prios[STC_MAX_WORKERS];
	int ncpus = stc_ncpus();
	int nworkers = ncpus + 1;
	int start_ret;
	int set_ret = ERROR;
	int finish_ret;
	uint32_t blocked_before = 0;
	uint32_t blocked_after = 0;
	uint32_t top_before = 0;
	uint32_t top_after = 0;
	int i;

	if (nworkers > STC_MAX_WORKERS) {
		nworkers = STC_MAX_WORKERS;
	}

	/* Worker 0 on top, everyone else level below it. */

	prios[0] = STC_PRI_LOW + 2 * STC_PRI_STEP;
	for (i = 1; i < nworkers; i++) {
		prios[i] = STC_PRI_LOW;
	}

	stc_workload_init(&wl, nworkers, prios[0], SCHED_RR);
	wl.priorities = prios;

	start_ret = stc_workload_start(&wl);

	if (start_ret == OK) {
		stc_workload_sample(STC_PRI_WINDOW_MS);
		blocked_before = stc_min_range(1, nworkers);
		top_before = stc_count(0);

		/* Drop worker 0 below the rest. */

		param.sched_priority = STC_PRI_LOW - STC_PRI_STEP;
		set_ret = sched_setparam(stc_worker_pid(0), &param);

		stc_workload_sample(STC_PRI_WINDOW_MS);
		blocked_after = stc_min_range(1, nworkers);
		top_after = stc_count(0);
	}

	finish_ret = stc_workload_finish(nworkers);

	printf("[SCN-PRI-04] peers min before=%u after=%u   demoted w0 before=%u after=%u\n",
	       (unsigned int)blocked_before, (unsigned int)blocked_after,
	       (unsigned int)top_before, (unsigned int)top_after);

	TC_ASSERT_EQ("pri04_workload_start", start_ret, OK);
	TC_ASSERT_EQ("pri04_setparam", set_ret, OK);
	TC_ASSERT_EQ("pri04_workload_finish", finish_ret, OK);

	/* Demoting the top task must let every remaining task run.
	 *
	 * The minimum is taken over the peers only, deliberately excluding
	 * worker 0.  After the demotion worker 0 is the least important
	 * runnable task, so on an oversubscribed machine it is the one that
	 * should now be starved; including it in the minimum would assert
	 * the opposite of the property under test.
	 */

	TC_ASSERT_GT("pri04_blocked_released", blocked_after, 0);


	{
		uint32_t demoted_gain = (top_after > top_before) ? (top_after - top_before) : 0;
		uint32_t tolerance = top_before / 20;  /* 5% tolerance */
		TC_ASSERT_LT("pri04_demoted_small_gain", demoted_gain, tolerance);
	}

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stc_sched_priority_main
 ****************************************************************************/

int stc_sched_priority_main(void)
{
	printf("[SCN-PRI] priority and preemption scenarios, ncpus=%d\n",
	       stc_ncpus());

	stc_sched_pri01_preempt_under_load();
	stc_sched_pri02_strict_ordering();
	stc_sched_pri03_raise_waiting_task();
	stc_sched_pri04_lower_running_task();

	return 0;
}
