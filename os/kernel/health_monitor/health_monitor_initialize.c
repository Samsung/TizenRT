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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <debug.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <tinyara/clock.h>
#include <tinyara/irq.h>
#include <tinyara/kthread.h>
#include <tinyara/watchdog.h>
#include "health_monitor/health_monitor.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define HEALTH_MONITOR_THREAD_NAME                "health_monitor_watchdog"
#define HEALTH_MONITOR_WATCHDOG_THREAD_TIMEOUT_MS 60000U
#define HEALTH_MONITOR_WATCHDOG_THREAD_PRIORITY   100
#define HEALTH_MONITOR_WATCHDOG_THREAD_STACKSIZE  2048

/****************************************************************************
 * Global Variables
 ****************************************************************************/

/* The HW watchdog is running and health monitoring registration is allowed. */
bool g_health_watchdog_started;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static bool health_monitor_has_overdue(void)
{
	irqstate_t flags;
	bool overdue;

	flags = spin_lock_irqsave(&g_health_lock);
	overdue = g_health_count > 0 && !health_monitor_is_tick_before(clock_systimer(), health_monitor_get_state(g_health_heap[0])->deadline);
	spin_unlock_irqrestore(&g_health_lock, flags);

	return overdue;
}

/****************************************************************************
 * Name: health_monitor_watchdog
 *
 * Description:
 *   Own watchdog keepalive in thread context and renew the thread's own HM.
 *   The timer ISR checks task deadlines.
 ****************************************************************************/

static int health_monitor_watchdog(int argc, char *argv[])
{
	uint32_t remaining;
	int fd;
	int ret;

	fd = open(CONFIG_WATCHDOG_DEVPATH, O_RDONLY);
	if (fd < 0) {
		lldbg("Watchdog thread open failed: %d\n", get_errno());
		return ERROR;
	}
	ret = health_monitor_start(HEALTH_MONITOR_WATCHDOG_THREAD_TIMEOUT_MS);
	if (ret < 0) {
		lldbg("Watchdog thread HM registration failed: %d\n", ret);
		close(fd);
		return ERROR;
	}

	for (;;) {
		ret = health_monitor_kick();
		if (ret < 0) {
			lldbg("Watchdog thread HM kick failed: %d\n", ret);
			close(fd);
			return ERROR;
		}
		if (health_monitor_has_overdue()) {
			lldbg("Overdue HM target, skip watchdog keepalive\n");
		} else if (ioctl(fd, WDIOC_KEEPALIVE, 0) < 0) {
			lldbg("Watchdog keepalive failed: %d\n", get_errno());
			return ERROR;
		}

		remaining = CONFIG_HEALTH_MONITOR_WATCHDOG_INTERVAL_SEC;
		while (remaining > 0) {
			remaining = sleep(remaining);
		}
	}
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: health_monitor_initialize
 *
 * Description:
 *   Arm the watchdog device registered by board initialization before creating
 *   the monitored feeder thread. Closing the device does not stop WDT.
 *
 * Assumptions:
 *   Called on CPU0 after board initialization.
 ****************************************************************************/

void health_monitor_initialize(void)
{
	int fd;
	int pid;
	int ret;

	fd = open(CONFIG_WATCHDOG_DEVPATH, O_RDONLY);
	if (fd < 0) {
		lldbg("Watchdog open failed: %d\n", get_errno());
		DEBUGASSERT(fd >= 0);
		return;
	}

	ret = ioctl(fd, WDIOC_SETTIMEOUT, CONFIG_HEALTH_MONITOR_WATCHDOG_TIMEOUT_MS);
	if (ret != OK) {
		lldbg("Watchdog set timeout failed: %d\n", get_errno());
		close(fd);
		DEBUGASSERT(ret == OK);
		return;
	}

	ret = ioctl(fd, WDIOC_START, 0);
	if (ret != OK) {
		lldbg("Watchdog start failed: %d\n", get_errno());
		close(fd);
		DEBUGASSERT(ret == OK);
		return;
	}
	close(fd);

	g_health_watchdog_started = true;

	pid = kernel_thread(HEALTH_MONITOR_THREAD_NAME, HEALTH_MONITOR_WATCHDOG_THREAD_PRIORITY, HEALTH_MONITOR_WATCHDOG_THREAD_STACKSIZE, health_monitor_watchdog, NULL);
	ASSERT(pid >= 0);
}
