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

#include <apps/shell/tash.h>
#include <pm_metrics.h>
#include <tinyara/config.h>
#include <debug.h>
#include <unistd.h>
#include <tinyara/kthread.h>
#include <sched.h>
#include <time.h>
#include <pm.h>
#include "pm.h"
#include "pm_debug.h"
#include "pm_metrics.h"
#include "pm_test.h"

/* Global Variables */

struct pm_callback_s pmtest_cbarray[PMTEST_DEVICES];

/* Local functions */

void pmtest_dev_notify(struct pm_callback_s *cb, int domain, enum pm_state_e state)
{
	pmvdbg("Recevied notify call for %s device in %d domain for %d state \n", cb->name, domain, state);
}

int pmtest_dev_prepare(struct pm_callback_s *cb, int domain, enum pm_state_e state)
{
	pmvdbg("Recevied prepare call for %s device in %d domain for %d state \n", cb->name, domain, state);
	return OK;
}

/* PM test routine */

static int pmtest_kthread(int argc, char *argv[])
{
	enum pm_state_e s = PM_IDLE;
#ifdef CONFIG_PM_METRICS
	struct pm_time_in_each_s m;
#endif
	struct timespec start_time;
	struct timespec elapsed_time;

	pmvdbg("This test runs for 7200 secs\n");
	clock_gettime(CLOCK_REALTIME, &start_time);
	elapsed_time.tv_sec = start_time.tv_sec;

	while (elapsed_time.tv_sec <= (start_time.tv_sec + PMTEST_DURATION_IN_SECS)) {
		sleep(PMTEST_THREAD_SLEEP_TIME);
		pm_changestate(PMTEST_DOMAIN, s);
		pm_dumpstates();
#ifdef CONFIG_PM_METRICS
		pm_get_domainmetrics(0, &m);
		pmvdbg("Normal:%d Idle:%d standby:%d sleep:%d \n", m.normal, m.idle, m.standby, m.sleep);
#endif
		clock_gettime(CLOCK_REALTIME, &elapsed_time);
	}
	return 0;
}

/* Launch pm test thread */

void pmtest_launch_kthread(void)
{
	int i = 0;
	if (kernel_thread("pmtest", PMTEST_THREAD_PRIORITY, PMTEST_THREAD_STACKSIZE, pmtest_kthread, (char *const *)NULL) < 0) {
		pmvdbg("pmtest kthread launch failed\n");
		for (i = 0; i < PMTEST_DEVICES; i++) {
			pm_unregister(0, &pmtest_cbarray[i]);
		}
	}
}

/* init function  */

void pmtest_init(void)
{
	int i = 0;
	char pmtest_dev_names[PMTEST_DEVICES][CONFIG_PM_DEVNAME_LEN] = { "dev0", "dev1", "dev2" };

	for (i = 0; i < PMTEST_DEVICES; i++) {
		pmtest_cbarray[i].notify = &pmtest_dev_notify;
		pmtest_cbarray[i].prepare = &pmtest_dev_prepare;
		strncpy(pmtest_cbarray[i].name, pmtest_dev_names[i], CONFIG_PM_DEVNAME_LEN - 1);
	}
	for (i = 0; i < PMTEST_DEVICES; i++) {
		pm_register(0, &pmtest_cbarray[i]);
	}
	/* Launch test thead */
	pmtest_launch_kthread();
}
