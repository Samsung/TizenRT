/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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

/************************************************************************
 * Included Files
 ************************************************************************/

#include <tinyara/config.h>
#include <sys/boardctl.h>
#include <tinyara/reboot_reason.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <errno.h>
#include <tinyara/clock.h>
#include <tinyara/wdog.h>

#include <tinyara/silent_reboot.h>

/************************************************************************
 * Definitions
 ************************************************************************/
#define REBOOT_TIME_START_SEC  0	// 0:00 AM
#define REBOOT_TIME_END_SEC    300	// 5:00 AM
#define DAYS_AFTER_BOOT        7	// days after reboot.
#define A_DAY_TO_SEC           86400	// seconds for a day (24 hours * 60 * 60)
#define SEVEN_DAYS_TO_SEC      604800	// seconds for 7 days (DAYS_AFTER_BOOT * A_DAY_TO_SEC)

/************************************************************************
 * Private Variables
 ************************************************************************/
static bool g_is_silent_mode;
static bool g_is_silent_timezone;
static int g_silent_lock_count;
static WDOG_ID g_silent_wdog;
static WDOG_ID g_silent_delaywdog;

enum SILENT_TIMER_CMD {
	SILENT_START = 0,
	SILENT_RESTART = 1,
	SILENT_END = 2,
	SILENT_DELAY = 3,
};

/************************************************************************
 * Private Function Prototypes
 ************************************************************************/
static int silent_reboot_set_timer(int timeout);

/************************************************************************
 * Private Functions
 ************************************************************************/
static void silent_reboot_perform(void)
{
	/* REBOOT with reboot reason, REBOOT_SYSTEM_PERIODIC_REBOOT.
	 * Then board will silent reboot.
	 */
	lldbg("Perform SILENT REBOOT expired by periodic timer!!\n");
	WRITE_REBOOT_REASON(REBOOT_SYSTEM_PERIODIC_REBOOT);
	boardctl(BOARDIOC_RESET, EXIT_SUCCESS);
}

static void silent_reboot_set_mode(void)
{
	int reboot_reason = READ_REBOOT_REASON();

	if (reboot_reason == REBOOT_SYSTEM_HW_RESET ||
	reboot_reason == REBOOT_SYSTEM_USER_INTENDED) {
		// Normal mode (non-silent mode)
		g_is_silent_mode = false;
		dbg("board boot with NORMAL MODE.\n");
	} else {
		// Silent mode
		g_is_silent_mode = true;
		dbg("board boot with SILENT MODE.\n");
	}
}

static int get_random_number(int start, int end)
{
	srand(time(NULL));
	return start + rand() % (end - start + 1);
}

static int silent_reboot_get_time(int cmd)
{
	int timeout_sec;
	time_t curtime;
	struct tm curtm;

	timeout_sec = -1;

	/* Get the current time */
	curtime = time(&curtime);
	if (curtime < 0) {
		return ERROR;
	}

	(void)localtime_r(&curtime, &curtm);

	if (cmd == SILENT_START) {
		/* Intially 7 days */
		timeout_sec = SEVEN_DAYS_TO_SEC;
	} else if (cmd == SILENT_RESTART) {
		/* Calculate the next 00:00 time */
		timeout_sec = (A_DAY_TO_SEC - (curtm.tm_sec + curtm.tm_min * 60 + curtm.tm_hour * 3600));
		/* Apply random time in (REBOOT_TIME_START_SEC, REBOOT_TIME_END_SEC) */
		timeout_sec += get_random_number(REBOOT_TIME_START_SEC, REBOOT_TIME_END_SEC);
	} else if (cmd == SILENT_END) {
		if (curtm.tm_hour < 5) {
			/* Calculate the remaining time from now until 5:00 am */
			timeout_sec = (5 * 60 * 60 - (curtm.tm_sec + curtm.tm_min * 60 + curtm.tm_hour * 3600));
		}
	}

	if (timeout_sec < 0) {
		dbg("Invalid timeout. cmd %d\n", cmd);
		return ERROR;
	}

	return timeout_sec;
}

static void silent_reboot_enter_timezone(void)
{
	/* Set a flag allow reboot */
	g_is_silent_timezone = true;

	/* Time available for silent reboot started.
	 * If reboot is not performed in this function, it means the lock state (g_silent_lock_count > 0).
	 * Then, it will be checked again in unlock function.
	 */
	if (g_silent_lock_count <= 0) {
		silent_reboot_perform();
	}

	/* Set timer for remaining time until the end time available for reboot */
	silent_reboot_set_timer(SILENT_END);
}

static bool silent_reboot_is_timezone(void)
{
	time_t curtime;
	struct tm curtm;

	/* Get the current time */
	curtime = time(&curtime);
	if (curtime != ERROR) {
		(void)localtime_r(&curtime, &curtm);
		if (curtm.tm_hour < 5) {
			return true;
		}
	}

	return false;
}

static void silent_timer_callback(int argc, uint32_t cmd)
{
	switch (cmd) {
	case SILENT_START:
	case SILENT_RESTART:
		/* Check whether current time is available for reboot because time can be changed after timer starts */
		if (silent_reboot_is_timezone()) {
			/* Do silent reboot if no lock. If not, set timer to end timezone */
			silent_reboot_enter_timezone();
		} else {
			/* Set timer for remaining time until the next start time available for reboot */
			silent_reboot_set_timer(SILENT_RESTART);
		}
		break;
	case SILENT_END:
		/* Set a flag prevent reboot */
		g_is_silent_timezone = false;
		/* Set timer for remaining time until the next start time available for reboot */
		silent_reboot_set_timer(SILENT_RESTART);
		break;
	case SILENT_DELAY:
		silent_reboot_unlock();
		break;
	default:
		break;
	}
}

static int silent_reboot_set_timer(int cmd)
{
	int ret;
	int timeout;

	/* Calculate time according to cmd */
	timeout = silent_reboot_get_time(cmd);
	if (timeout < 0) {
		return ERROR;
	}

	ret = wd_start(g_silent_wdog, SEC2TICK(timeout), (wdentry_t)silent_timer_callback, 1, (uint32_t)cmd);
	if (ret != OK) {
		return ERROR;
	}

	return OK;
}

/************************************************************************
 * Public Functions
 ************************************************************************/
/****************************************************************************
 * Name: silent_reboot_lock
 *
 * Description:
 *   Increase lock count.
 *
 ****************************************************************************/
int silent_reboot_lock(void)
{
	g_silent_lock_count++;

	dbg("silent reboot lock count = %d\n", g_silent_lock_count);

	return OK;
}

/****************************************************************************
 * Name: silent_reboot_unlock
 *
 * Description:
 *   Decrease lock count and check whether it is possible to do silent reboot.
 *
 ****************************************************************************/
int silent_reboot_unlock(void)
{
	g_silent_lock_count--;

	/* Check timezone available for reboot */
	if (g_is_silent_timezone) {
		/* Check real time also because it can be changed after reboot timezone started */
		if (silent_reboot_is_timezone()) {
			if (g_silent_lock_count <= 0) {
				silent_reboot_perform();
			}
		} else {
			/* Calibrate timer to set next silent reboot time */
			g_is_silent_timezone = false;
			silent_reboot_set_timer(SILENT_RESTART);
		}
	}

	return OK;
}

/****************************************************************************
 * Name: silent_reboot_delay
 *
 * Description:
 *   Postpone silent reboot.
 *   It gets lock and release lock after delay (seconds).
 *
 ****************************************************************************/
int silent_reboot_delay(int delay)
{
	int ret;
	int tick_remain;

	if (delay <= 0) {
		return ERROR;
	}

	if (g_silent_delaywdog == NULL) {
		g_silent_delaywdog = wd_create();
		if (!g_silent_delaywdog) {
			dbg("Failed to set timer to unlock\n");
			return ERROR;
		}
	}

	tick_remain = wd_gettime(g_silent_delaywdog);
	if (SEC2TICK(delay) <= tick_remain) {
		dbg("Already reboot delayed for %d seconds\n", TICK2SEC(tick_remain));
		return OK;
	}

	if (tick_remain == 0) {
		/* If delay watchdog is stopped, lock first */
		g_silent_lock_count++;
	}

	dbg("Watchdog Start for unlock: timeout %ds\n", delay);

	/* Unlock by timer callback */
	ret = wd_start(g_silent_delaywdog, SEC2TICK(delay), (wdentry_t)silent_timer_callback, 1, SILENT_DELAY);
	if (ret != OK) {
		dbg("Failed to start watchdog %d\n", get_errno());
		return ERROR;
	}

	return OK;
}

/****************************************************************************
 * Name: silent_reboot_force_perform_after_timeout
 *
 * Description:
 *   Perform silent reboot after timeout(seconds).
 *   It performs silent reboot forcedly if there is no lock in timezone available for reboot.
 *
 ****************************************************************************/
int silent_reboot_force_perform_after_timeout(int timeout)
{
	int ret;
	int tick_remain;
	WDOG_ID wdog;

	tick_remain = wd_gettime(g_silent_wdog);
	if (SEC2TICK(timeout) > tick_remain) {
		dbg("Timeout %d is smaller than remaining time of periodic reboot (%d s)\n", timeout, TICK2SEC(tick_remain));
		return OK;
	}

	dbg("Reboot watchdog will be expired after %d seconds\n", timeout);

	ret = wd_start(g_silent_wdog, SEC2TICK(timeout), (wdentry_t)silent_timer_callback, 1, SILENT_RESTART);
	if (ret != OK) {
		dbg("Failed to start watchdog %d\n", get_errno());
		return ERROR;
	}

	return OK;
}

/****************************************************************************
 * Name: silent_reboot_is_silent_mode
 *
 * Description:
 *   Check whether silent mode or not.
 *
 ****************************************************************************/
bool silent_reboot_is_silent_mode(void)
{
	return g_is_silent_mode;
}

/****************************************************************************
 * Name: silent_reboot_get_status
 *
 * Description:
 *   Get status of silent reboot. It includes lock count, remaining time until next reboot and delay.
 *
 ****************************************************************************/
int silent_reboot_get_status(silent_reboot_status_t *status)
{
	if (status == NULL) {
		return ERROR;
	}

	status->lock_count = g_silent_lock_count;
	if (g_is_silent_timezone) {
		// Currently available for reboot
		status->reboot_timezone_left = 0;
	} else {
		// Not available for reboot yet. Get remaining time until next available.
		status->reboot_timezone_left = TICK2SEC(wd_gettime(g_silent_wdog));
	}

	status->reboot_delay_left = TICK2SEC(wd_gettime(g_silent_delaywdog));

	return OK;
}

/****************************************************************************
 * Name: silent_reboot_initialize
 *
 * Description:
 *   Initialize silent reboot module. This function must be called once when system starts.
 *
 ****************************************************************************/
void silent_reboot_initialize(void)
{
	g_silent_lock_count = 0;
	g_is_silent_timezone = false;

	/* Set silent mode by checking reboot reason. */
	silent_reboot_set_mode();

	/* Create timer to check the time zone available for silent reboot. */
	g_silent_wdog = wd_create();
	if (!g_silent_wdog) {
		dbg("Failed to set timer periodic silent reboot\n");
		return;
	}

	/* Create timer to check the time zone available for silent reboot. */
	silent_reboot_set_timer(SILENT_START);
}
