/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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

#include <tinyara/config.h>
#include <tinyara/pm/pm.h>
#include <tinyara/sched.h>
#include <time.h>
#include <queue.h>
#include <debug.h>
#include <errno.h>
#include "pm.h"

struct pm_metric_domain_s {
	clock_t stime[CONFIG_PM_NDOMAINS];                           /* Last suspended time stamp of domain */
	uint32_t blocking_board_sleep_ticks[CONFIG_PM_NDOMAINS];     /* Tick time of the suspended domain inside idle thread that prevent board sleep */
	uint32_t suspend_ticks[CONFIG_PM_NDOMAINS];                  /* Time (in ticks) domain suspended */
};

typedef struct pm_metric_domain_s pm_metric_domain_t;

struct pm_metric_state_s {
	clock_t stime;                                               /* Last PM change state time stamp */
	uint32_t state_accum_ticks[PM_COUNT];                        /* PM State Time (in ticks)*/
};

typedef struct pm_metric_state_s pm_metric_state_t;

struct pm_metric_thread_s {
	pm_wakeup_reason_code_t wakeup_src;                          /* The wakeup source */
	uint32_t wakeup_counts[CONFIG_MAX_TASKS];                    /* Wakeup timer counts for each thread */
	uint32_t min_sleep_ticks[CONFIG_MAX_TASKS];                  /* Minimum actual sleep time allowed by each thread */
	uint32_t delay;                                              /* Wakeup timer delay (in ticks) */
};

typedef struct pm_metric_thread_s pm_metric_thread_t;

struct pm_metric_s {
	pm_metric_domain_t domain_metrics;                          /* The domain metrics */
	pm_metric_state_t state_metrics;                            /* The power management state metrics */
	pm_metric_thread_t thread_metrics;                          /* The power management thread metrics */
	uint32_t board_sleep_ticks;                                 /* The amount of time (in ticks) board was in sleep */
	uint32_t wakeup_src_counts[PM_WAKEUP_SRC_COUNT];            /* It counts the frequency of wakeup sources */
	uint32_t total_try_ticks;                                   /* Total duration of time pm tries to make board sleep */
};

typedef struct pm_metric_s pm_metric_t;

static pm_metric_t *g_pm_metrics;
static bool g_pm_metrics_running = false;

static void pm_print_metrics(double total_time, int n_domains)
{
	int index;
	pmdbg("\n");
	pmdbg("TOTAL METRICS TIME [1] = %dms\n", TICK2MSEC((int)total_time));
	pmdbg("TOTAL SLEEP TRY TIME [2] = %dms\n", TICK2MSEC(g_pm_metrics->total_try_ticks));
	pmdbg("*[2] = represents total duration of time, PM tries to make board sleep.\n");
	pmdbg("\n");
	pmdbg("\n");
	pmdbg("              DOMAIN              | TOTAL PM SUSPEND TIME [3] | TOTAL SLEEP BLOCKING TIME [4] \n");
	pmdbg("----------------------------------|---------------------------|-------------------------------\n");
	for (index = 0; index < n_domains; index++) {
		pmdbg(" %32s | %13dms (%6.2f%%) | %17dms (%6.2f%%) \n", pm_domain_map[index], TICK2MSEC(g_pm_metrics->domain_metrics.suspend_ticks[index]),\
		((double)g_pm_metrics->domain_metrics.suspend_ticks[index]) * 100.0 / total_time, g_pm_metrics->domain_metrics.blocking_board_sleep_ticks[index],\
		((double)g_pm_metrics->domain_metrics.blocking_board_sleep_ticks[index]) * 100.0 / ((double)g_pm_metrics->total_try_ticks));
	}
	pmdbg("\n");
	pmdbg("*[3] = total time pm domain was suspended.\n");
	pmdbg("*[4] = represents the time board was not able to sleep due to suspended domain.\n");
	pmdbg("*[3] (%) = [3] / [1] * 100\n");
	pmdbg("*[4] (%) = [4] / [2] * 100\n");
	pmdbg("\n");
	pmdbg("\n");
	pmdbg(" WAKEUP SOURCES | COUNTS \n");
	pmdbg("----------------|--------\n");
	for (index = 0; index < PM_WAKEUP_SRC_COUNT; index++) {
		pmdbg(" %14s | %6d \n", wakeup_src_name[index], g_pm_metrics->wakeup_src_counts[index]);
	}
	if (g_pm_metrics->wakeup_src_counts[PM_WAKEUP_HW_TIMER]) {
		pmdbg("\n");
		pmdbg("\n");
		pmdbg(" PROCESS NAME | WAKEUP COUNTS | MIN SLEEP TIME \n");
		pmdbg("--------------|---------------|----------------\n");
		for (index = 0; index < CONFIG_MAX_TASKS; index++) {
			if (g_pm_metrics->thread_metrics.wakeup_counts[index]) {
				pmdbg(" %12s | %13d | %12dms \n", sched_gettcb(index)->name, g_pm_metrics->thread_metrics.wakeup_counts[index], TICK2MSEC(g_pm_metrics->thread_metrics.min_sleep_ticks[index]));
			}
		}
	}
	pmdbg("\n");
	pmdbg("\n");
	pmdbg(" BOARD STATE | PM STATE |          TIME          \n");
	pmdbg("-------------|----------|------------------------\n");
	pmdbg(" %11s | %8s | %10dms (%6.2f%%) \n", "WAKEUP", "NORMAL", TICK2MSEC(g_pm_metrics->state_metrics.state_accum_ticks[0]),\
		((double)g_pm_metrics->state_metrics.state_accum_ticks[0]) * 100.0 / total_time);
	pmdbg(" %11s | %8s | %10dms (%6.2f%%) \n", "", "IDLE", TICK2MSEC(g_pm_metrics->state_metrics.state_accum_ticks[1]),\
		((double)g_pm_metrics->state_metrics.state_accum_ticks[1]) * 100.0 / total_time);
	pmdbg(" %11s | %8s | %10dms (%6.2f%%) \n", "", "STANDBY", TICK2MSEC(g_pm_metrics->state_metrics.state_accum_ticks[2]),\
		((double)g_pm_metrics->state_metrics.state_accum_ticks[2]) * 100.0 / total_time);
	pmdbg("-------------|----------|------------------------\n");
	pmdbg(" %11s | %8s | %10dms (%6.2f%%) \n", "SLEEP", "SLEEP", TICK2MSEC(g_pm_metrics->board_sleep_ticks),\
	((double)g_pm_metrics->board_sleep_ticks) * 100.0 / total_time);
}
/************************************************************************************
 * Public Functions
 ************************************************************************************/

/****************************************************************************
 * Name: pm_metrics_update_domain
 *
 * Description:
 *   This function is called when new domain got registered during pm_monitoring
 *   or during pm_metrics initialization. It initialize the PM Metrics for given
 *   domain.
 *
 * Input parameters:
 *   domain_id - the ID of domain registered with PM.
 *
 * Returned value:
 *   None
 *
 ****************************************************************************/
void pm_metrics_update_domain(int domain_id)
{
	if (g_pm_metrics_running) {
		g_pm_metrics->domain_metrics.stime[domain_id] = clock_systimer();
	}
}

/****************************************************************************
 * Name: pm_metrics_update_suspend
 *
 * Description:
 *   This function is called inside pm_suspend. It note the timestamp (in ticks) of
 *   suspended domain.
 *
 * Input parameters:
 *   domain_id - the ID of domain registered with PM.
 *
 * Returned value:
 *   None
 *
 ****************************************************************************/
void pm_metrics_update_suspend(int domain_id)
{
	if (g_pm_metrics_running && (g_pmglobals.suspend_count[domain_id] == 0)) {
		g_pm_metrics->domain_metrics.stime[domain_id] = clock_systimer();
	}
}

/****************************************************************************
 * Name: pm_metrics_update_resume
 *
 * Description:
 *   This function is called inside pm_resume. Before resuming domain, it counts
 *   amount of time (in ticks) the given domain was suspended.
 * 
 * Input parameters:
 *   domain_id - the ID of domain registered with PM.
 *
 * Returned value:
 *   None
 *
 ****************************************************************************/
void pm_metrics_update_resume(int domain_id)
{
	if (g_pm_metrics_running && (g_pmglobals.suspend_count[domain_id] == 1)) {
		g_pm_metrics->domain_metrics.suspend_ticks[domain_id] += clock_systimer() - g_pm_metrics->domain_metrics.stime[domain_id];
	}
}

/****************************************************************************
 * Name: pm_metrics_update_idle
 *
 * Description:
 *   This function is called inside pm_idle. It counts the frequency of domain, which
 *   make board unable to go into sleep during idle cpu time.
 * 
 * Input parameters:
 *   None
 *
 * Returned value:
 *   None
 *
 ****************************************************************************/
void pm_metrics_update_idle(void)
{
	int index;
	if (g_pm_metrics_running && (g_pmglobals.recommended != PM_SLEEP)) {
		g_pm_metrics->total_try_ticks++;
		for (index = 0; index < CONFIG_PM_NDOMAINS; index++) {
			if (g_pmglobals.suspend_count[index] != 0) {
				g_pm_metrics->domain_metrics.blocking_board_sleep_ticks[index]++;
			}
		}
	}
}

/****************************************************************************
 * Name: pm_metrics_update_changestate
 *
 * Description:
 *   This function is called inside pm_changestate. Before changing state, it counts
 *   amount of time (in ticks) was in that state.
 * 
 * Input parameters:
 *   None
 *
 * Returned value:
 *   None
 *
 ****************************************************************************/
void pm_metrics_update_changestate(void)
{
	clock_t now;
	if (g_pm_metrics_running) {
		now = clock_systimer();
		g_pm_metrics->state_metrics.state_accum_ticks[g_pmglobals.state] += now - g_pm_metrics->state_metrics.stime;
		g_pm_metrics->state_metrics.stime = now;
	}
}

/****************************************************************************
 * Name: pm_metrics_update_wakehandler
 *
 * Description:
 *   This function is called inside pm_wakehandler. It counts the frequency of wakeup
 *   sources, which are waking up the board. It also checks the amount of time board
 *   was in sleep.
 * 
 * Input parameters:
 *   missing_tick - the amount of time the board was in sleep.
 *   wakeup_src   - the wakeup reason code.
 *
 * Returned value:
 *   None
 *
 ****************************************************************************/
void pm_metrics_update_wakehandler(uint32_t missing_tick, pm_wakeup_reason_code_t wakeup_src)
{
	if (g_pm_metrics_running) {
		g_pm_metrics->thread_metrics.wakeup_src = wakeup_src;
		g_pm_metrics->thread_metrics.delay = missing_tick;
		g_pm_metrics->wakeup_src_counts[wakeup_src]++;
		g_pm_metrics->board_sleep_ticks += missing_tick;
	}
}

/****************************************************************************
 * Name: pm_metrics_update_sleep
 *
 * Description:
 *   This function is called inside pm_sleep's callback. It counts the frequency of
 *   thread which wakeup the board. It also checks the minimum amount of time board
 *   was in sleep because of given thread.
 * 
 * Input parameters:
 *   pid - the ID of thread
 *
 * Returned value:
 *   None
 *
 ****************************************************************************/
void pm_metrics_update_sleep(pid_t pid)
{
	int hash_pid;
	if (g_pm_metrics_running && (g_pm_metrics->thread_metrics.wakeup_src == PM_WAKEUP_HW_TIMER)) {
		hash_pid = PIDHASH(pid);
		if (g_pm_metrics->thread_metrics.delay < g_pm_metrics->thread_metrics.min_sleep_ticks[hash_pid]) {
			g_pm_metrics->thread_metrics.min_sleep_ticks[hash_pid] = g_pm_metrics->thread_metrics.delay;
		}
		g_pm_metrics->thread_metrics.wakeup_counts[hash_pid]++;
		g_pm_metrics->thread_metrics.wakeup_src = PM_WAKEUP_UNKNOWN;
	}
}

/****************************************************************************
 * Name: pm_metrics_update_recover
 *
 * Description:
 *   This function is called inside pm_recover. It resets the wakeup_counts and 
 *   sleep_ticks of given thread for consistent PM Metrics result.
 * 
 * Input parameters:
 *   pid - the ID of thread
 *
 * Returned value:
 *   None
 *
 ****************************************************************************/
void pm_metrics_update_recover(pid_t pid)
{
	int hash_pid;
	if (g_pm_metrics_running) {
		hash_pid = PIDHASH(pid);
		g_pm_metrics->thread_metrics.min_sleep_ticks[hash_pid] = UINT32_MAX;
		g_pm_metrics->thread_metrics.wakeup_counts[hash_pid] = 0;
	}
}

/****************************************************************************
 * Name: pm_metrics
 *
 * Description:
 *   This internal function is called to analyze the PM suspend and sleep behaviour.
 *   It gathers the pm metrics statistics for provided time (in msec) to provide the
 *   domain specific pm suspend information.
 *
 * Input Parameters:
 *   milliseconds - the monitoring duration in milliseconds
 *
 * Returned Value:
 *   OK (0)     - On Success
 *   ERROR (-1) - On Error
 *
 ****************************************************************************/
int pm_metrics(int milliseconds)
{
	clock_t start_time, end_time;
	irqstate_t flags;
	int index;
	int n_domains;
	int pm_suspended = -1;
	int pm_resumed = -1;
	/* If PM Metrics already running then notify other thread */
	if (g_pm_metrics) {
		pmdbg("PM Metrics already running\n");
		return OK;
	}
	/* Lock PM so that no two thread can run PM Metrics simultaneously */
	pm_lock();
	/* Avoid board sleep during PM Metrics initialization */
	pm_suspended = pm_suspend(PM_IDLE_DOMAIN);
	/* Allocate memory for initializing PM Metrics measurements */
	g_pm_metrics = pm_alloc(1, sizeof(pm_metric_t));
	if (g_pm_metrics == NULL) {
		set_errno(ENOMEM);
		pmdbg("Unable to initialize pm_metrics, error = %d\n", get_errno());
		pm_unlock();
		return ERROR;
	}
	/* PM Metrics Initialization */
	flags = enter_critical_section();
	start_time = clock_systimer();
	g_pm_metrics->state_metrics.stime = start_time;
	for (index = 0; (index < CONFIG_PM_NDOMAINS) && pm_domain_map[index]; index++) {
		g_pm_metrics->domain_metrics.stime[index] = start_time;
	}
	for (index = 0; index < CONFIG_MAX_TASKS; index++) {
		g_pm_metrics->thread_metrics.min_sleep_ticks[index] = UINT32_MAX;
	}
	g_pm_metrics_running = true;
	leave_critical_section(flags);
	/* Resume Board Sleep */
	if (pm_suspended == OK) {
		pm_resumed = pm_resume(PM_IDLE_DOMAIN);
	} else {
		pm_resumed = -1;
		pmdbg("Unable to resume IDLE Domain\n");
	}
	/* Suspend for given time interval */
	pm_sleep(TICK2MSEC(MSEC2TICK(milliseconds) - (clock_systimer() - start_time)));
	/* Avoid board sleep during PM Metrics post processing */
	if (pm_resumed == OK) {
		pm_suspended = pm_suspend(PM_IDLE_DOMAIN);
	} else {
		pm_suspended = -1;
		pmdbg("Unable to suspend IDLE Domain\n");
	}
	/* PM Metrics post calculations for consistent result */
	flags = enter_critical_section();
	g_pm_metrics_running = false;
	end_time = clock_systimer();
	for (index = 0; (index < CONFIG_PM_NDOMAINS) && pm_domain_map[index]; index++) {
		if (g_pmglobals.suspend_count[index]) {
			g_pm_metrics->domain_metrics.suspend_ticks[index] += end_time - g_pm_metrics->domain_metrics.stime[index];
		}
	}
	n_domains = index;
	g_pm_metrics->state_metrics.state_accum_ticks[g_pmglobals.state] += end_time - g_pm_metrics->state_metrics.stime;
	/* Show PM Metrics Results */
	pm_print_metrics((double)(end_time - start_time), n_domains);
	/* Free allocated memory */
	leave_critical_section(flags);
	free(g_pm_metrics);
	g_pm_metrics = NULL;
	/* Resume Board Sleep */
	if (pm_suspended == OK) {
		pm_resumed = pm_resume(PM_IDLE_DOMAIN);
	} else {
		pm_resumed = -1;
		pmdbg("Unable to resume IDLE Domain\n");
	}
	/* Unlock PM Metrics for other threads */
	pm_unlock();
	return OK;
}
