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
#include <time.h>
#include <queue.h>
#include <debug.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include "pm.h"

struct pm_metric_state_s {
	clock_t stime;						  /* Last PM change state time stamp */
	uint32_t state_accum_ticks[PM_COUNT]; /* PM State Time (in ticks)*/
};

typedef struct pm_metric_state_s pm_metric_state_t;

struct pm_metric_s {
	pm_metric_state_t state_metrics;				 /* The power management state metrics */
	uint32_t board_sleep_ticks;						 /* The amount of time (in ticks) board was in sleep */
	uint32_t wakeup_src_counts[PM_WAKEUP_SRC_COUNT]; /* It counts the frequency of wakeup sources */
	uint32_t total_try_ticks;						 /* Total duration of time pm tries to make board sleep */
};

typedef struct pm_metric_s pm_metric_t;

static pm_metric_t *g_pm_metrics;
static bool g_pm_metrics_running = false;

static void pm_print_metrics(double total_time, int n_domains)
{
	int index;
	struct pm_domain_s *domain;
	dq_entry_t *entry;
	enum pm_state_e pm_state;
	pmdbg("\n");
	pmdbg("TOTAL METRICS TIME [1] = %dms\n", TICK2MSEC((int)total_time));
	pmdbg("TOTAL SLEEP TRY TIME [2] = %dms\n", TICK2MSEC(g_pm_metrics->total_try_ticks));
	pmdbg("*[2] = represents total duration of time, PM tries to make board sleep.\n");
	pmdbg("\n");
	pmdbg("\n");
	pmdbg("              DOMAIN              | TOTAL PM SUSPEND TIME [3] | TOTAL SLEEP BLOCKING TIME [4] \n");
	pmdbg("----------------------------------|---------------------------|-------------------------------\n");
	for (entry = dq_peek(&g_pmglobals.domains); entry != NULL; entry = dq_next(entry)) {
		domain = (FAR struct pm_domain_s *)entry;
		pmdbg(" %32s | %13dms (%6.2f%%) | %17dms (%6.2f%%) \n", domain->name, TICK2MSEC(domain->suspend_ticks),
			  ((double)domain->suspend_ticks) * 100.0 / total_time, domain->blocking_board_sleep_ticks,
			  ((double)domain->blocking_board_sleep_ticks) * 100.0 / ((double)g_pm_metrics->total_try_ticks));
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
	pmdbg("\n");
	pmdbg("\n");
	pmdbg(" BOARD STATE | PM STATE |          TIME          \n");
	pmdbg("-------------|----------|------------------------\n");
	for (pm_state = PM_NORMAL; pm_state < PM_SLEEP; pm_state++) {
		pmdbg(" %11s | %8s | %10dms (%6.2f%%) \n", ((pm_state == PM_NORMAL) ? "WAKEUP" : ""), pm_state_name[pm_state], TICK2MSEC(g_pm_metrics->state_metrics.state_accum_ticks[pm_state]),
			((double)g_pm_metrics->state_metrics.state_accum_ticks[pm_state]) * 100.0 / total_time);
	}
	pmdbg("-------------|----------|------------------------\n");
	pmdbg(" %11s | %8s | %10dms (%6.2f%%) \n", "SLEEP", pm_state_name[PM_SLEEP], TICK2MSEC(g_pm_metrics->board_sleep_ticks),
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
 *   domain - Pointer to the domain structure
 *
 * Returned value:
 *   None
 *
 ****************************************************************************/
void pm_metrics_update_domain(FAR struct pm_domain_s *domain)
{
	if (g_pm_metrics_running && domain) {
		domain->stime = clock_systimer();
		domain->suspend_ticks = 0;
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
 *   domain - Pointer to the domain structure
 *
 * Returned value:
 *   None
 *
 ****************************************************************************/
void pm_metrics_update_suspend(FAR struct pm_domain_s *domain)
{
	if (g_pm_metrics_running && domain && (domain->suspend_count == 0)) {
		domain->stime = clock_systimer();
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
 *   domain - Pointer to the domain structure
 *
 * Returned value:
 *   None
 *
 ****************************************************************************/
void pm_metrics_update_resume(FAR struct pm_domain_s *domain)
{
	if (g_pm_metrics_running && domain && (domain->suspend_count == 1)) {
		domain->suspend_ticks += clock_systimer() - domain->stime;
	}
}

/****************************************************************************
 * Name: pm_metrics_update_idle
 *
 * Description:
 *   This function is called inside pm_idle. It counts the frequency of domain, which
 *   make board unable to go into sleep during low power cpu time.
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
	FAR struct pm_domain_s *domain;
	FAR dq_entry_t *entry;
	irqstate_t flags;

	if (g_pm_metrics_running) {
		g_pm_metrics->total_try_ticks++;
		flags = enter_critical_section(); /* Protect domain list access */
		for (entry = dq_peek(&g_pmglobals.suspended_domains); entry != NULL; entry = dq_next(entry)) {
			domain = (FAR struct pm_domain_s *)entry;
			if (domain->suspend_count != 0) {
				domain->blocking_board_sleep_ticks++;
			}
		}
		leave_critical_section(flags);
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
 * Name: pm_metrics_update_wakeup_reason
 *
 * Description:
 *   This function is called inside enable_and_compensate_systick. It counts
 *   the frequency of wakeup sources, which are waking up the board.
 * 
 * Input parameters:
 *   wakeup_src   - the wakeup reason code.
 *
 * Returned value:
 *   None
 *
 ****************************************************************************/
void pm_metrics_update_wakeup_reason(pm_wakeup_reason_code_t wakeup_src)
{
	if (g_pm_metrics_running) {
		if (wakeup_src < PM_WAKEUP_SRC_COUNT) {
			g_pm_metrics->wakeup_src_counts[wakeup_src]++;
		}
	}
}

/****************************************************************************
 * Name: pm_metrics_update_missing_tick
 *
 * Description:
 *   This function is called inside enable_and_compensate_systick. It checks
 *   the amount of time board was in sleep.
 * 
 * Input parameters:
 *   missing_tick - the amount of time the board was in sleep.
 *
 * Returned value:
 *   None
 *
 ****************************************************************************/
void pm_metrics_update_missing_tick(clock_t missing_tick)
{
	if (g_pm_metrics_running) {
		g_pm_metrics->board_sleep_ticks += missing_tick;
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
	int n_domains = 0;
	FAR struct pm_domain_s *domain;
	FAR dq_entry_t *entry;

	/* If PM Metrics already running then notify other thread */
	if (g_pm_metrics) {
		pmdbg("PM Metrics already running\n");
		return OK;
	}
	/* There is no need to do measurement if PM is not ready to state change */
	if (!g_pmglobals.is_running) {
		pmdbg("Please Start PM to enable PM Metrics\n");
		return OK;
	}
	/* Lock PM so that no two thread can run PM Metrics simultaneously */
	pm_lock();

	/* Allocate memory for initializing PM Metrics measurements */
	g_pm_metrics = (pm_metric_t *)pm_alloc(1, sizeof(pm_metric_t));
	if (g_pm_metrics == NULL) {
		set_errno(ENOMEM);
		pmdbg("Unable to initialize pm_metrics, error = %d\n", get_errno());
		pm_unlock();
		return ERROR;
	}
	/* PM Metrics Initialization */
	for (index = 0; index < PM_COUNT; index++) {
		g_pm_metrics->state_metrics.state_accum_ticks[index] = 0;
	}

	flags = enter_critical_section();
	start_time = clock_systimer();
	g_pm_metrics->state_metrics.stime = start_time;

	/* Initialize metrics for all registered domains */
	for (entry = dq_peek(&g_pmglobals.domains); entry != NULL; entry = dq_next(entry)) {
		domain = (FAR struct pm_domain_s *)entry;
		pm_metrics_update_domain(domain); /* This will set domain->stime */
		n_domains++;
	}
	g_pm_metrics_running = true;
	leave_critical_section(flags);

	/* Suspend for given time interval */
	pm_sleep(TICK2MSEC(MSEC2TICK(milliseconds) - (clock_systimer() - start_time)));

	/* PM Metrics post calculations for consistent result */
	flags = enter_critical_section();
	g_pm_metrics_running = false;
	end_time = clock_systimer();

	/* Finalize suspend ticks for domains that are still suspended */
	for (entry = dq_peek(&g_pmglobals.domains); entry != NULL; entry = dq_next(entry)) {
		domain = (FAR struct pm_domain_s *)entry;
		if (domain->suspend_count > 0) {
			domain->suspend_ticks += end_time - domain->stime;
		}
	}
	n_domains = g_pmglobals.ndomains; /* Get final count of domains */
	g_pm_metrics->state_metrics.state_accum_ticks[g_pmglobals.state] += end_time - g_pm_metrics->state_metrics.stime;
	leave_critical_section(flags);
	/* Show PM Metrics Results */
	pm_print_metrics((double)(end_time - start_time), n_domains);
	/* Free allocated memory */
	free(g_pm_metrics); /* Use pm_free for consistency */
	g_pm_metrics = NULL;

	/* Unlock PM Metrics for other threads */
	pm_unlock();
	return OK;
}
