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

#include <pm_metrics.h>
#include <tinyara/config.h>
#include <debug.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>
#include <pm.h>
#include <tinyara/pm/pm_test.h>

#include "pm.h"
#include "pm_debug.h"
#include "pm_metrics.h"
#include <sys/types.h>

/* Global Variables */

struct pm_callback_s pmtest_cbarray[PMTEST_DEVICES];

/* Local functions */

void pmtest_dev_notify(struct pm_callback_s *cb, int domain, int state)
{
	pmvdbg("Recevied notify call for %s device in %d domain for %d state \n", cb->name, domain, state);
}

int pmtest_dev_prepare(struct pm_callback_s *cb, int domain, int state)
{
	pmvdbg("Recevied prepare call for %s device in %d domain for %d state \n", cb->name, domain, state);
	return OK;
}

/* PM test routine */
/**
* @fn                   :pm_test_thread
* @brief                :This tc tests the different  power states provided
*                        by the PM.
* @Scenario             :The function shall tests the power states provided
*                        by PM by sleeping for a certain period & recording
*                        the sleeping time in each sleep state.
* @API'scovered         :pm_changestate(), pm_dumpstates(), pm_get_domainmetrics()
* @Preconditions        :Device should have entered different sleep states
* @Postconditions       :none
* @return               :void
*/
static int pm_test_thread(int argc, char *argv[])
{
	int pm_state;
	int ret = -EINVAL;
	struct timespec start_time;
	struct timespec elapsed_time;

	pmvdbg("This test runs for 7200 secs\n");
	clock_gettime(CLOCK_REALTIME, &start_time);
	elapsed_time.tv_sec = start_time.tv_sec;

	for (pm_state = PM_NORMAL; pm_state < CONFIG_PM_NSTATE; pm_state++) {
		while (elapsed_time.tv_sec <= (start_time.tv_sec + PMTEST_DURATION_IN_SECS)) {
			sleep(PMTEST_THREAD_SLEEP_TIME);
			pm_changestate(PMTEST_DOMAIN, pm_state);
			pm_dumpstates();
#ifdef CONFIG_PM_METRICS
			int s = 0;
			pm_get_domainmetrics(0);
			for (s = 0; s < CONFIG_PM_NSTATE; s++) {
				pmvdbg("Time spent in state %d : %d\n", s, pm_time_in_each_s[s]);
			}
#endif
			clock_gettime(CLOCK_REALTIME, &elapsed_time);
		}
	}

	/* Return to normal state */
	pm_state = PM_NORMAL;
	ret = pm_changestate(PMTEST_DOMAIN, pm_state);

	return 0;
}

/* Launch pm test thread */

void pmtest_launch_thread(void)
{
	int i = 0;
	pthread_t pid;

	if (pthread_create(&pid, NULL, (pthread_addr_t)pm_test_thread, (pthread_addr_t)NULL) < 0) {
		pmdbg("pm_test_thread launch failed\n");
		for (i = 0; i < PMTEST_DEVICES; i++) {
			pm_unregister(0, &pmtest_cbarray[i]);
		}
	}
	pthread_join(pid, NULL);
}

/****************************************************************************
 * pmtest_init function
 ****************************************************************************/

int pmtest_init(void)
{
	int i = 0;
	char pmtest_dev_names[PMTEST_DEVICES][CONFIG_PM_DEVNAME_LEN] = { "dev0", "dev1", "dev2" };
	pmvdbg("\n%s: 'Begin' Test\n", __func__);

	for (i = 0; i < PMTEST_DEVICES; i++) {
		pmtest_cbarray[i].notify = &pmtest_dev_notify;
		pmtest_cbarray[i].prepare = &pmtest_dev_prepare;
		strncpy(pmtest_cbarray[i].name, pmtest_dev_names[i], CONFIG_PM_DEVNAME_LEN - 1);
	}
	for (i = 0; i < PMTEST_DEVICES; i++) {
		pm_register(0, &pmtest_cbarray[i]);
	}
	/* Launch test thread */
	pmtest_launch_thread();

	for (i = 0; i < PMTEST_DEVICES; i++) {
		pm_unregister(0, &pmtest_cbarray[i]);
	}

	pmvdbg("\n%s: 'End' Test\n", __func__);

	return 0;
}
