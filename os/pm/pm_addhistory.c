/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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
#include <stdlib.h>
#include <string.h>
#include <queue.h>
#include <debug.h>
#include <time.h>
#include <assert.h>
#include <errno.h>
#include <tinyara/pm/pm.h>
#include "pm.h"
#include "pm_metrics.h"

#ifdef CONFIG_PM

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: pm_addhistory
 *
 * Description:
 *   This function add history into pm structure.
 *
 * Input parameters:
 *   domain - The PM domain to check
 *   state - Current PM state
 *
 * Returned value:
 *    Zero (OK) on success; otherwise a negated errno value is returned.
 *
 ****************************************************************************/
#ifdef CONFIG_PM_METRICS
int pm_addhistory(int domain, int state)
{
	int ret = OK;
	DEBUGASSERT(domain >= 0 && domain < CONFIG_PM_NDOMAINS && state >= PM_NORMAL && state <= PM_SLEEP);
	FAR struct pm_domain_s *pdom = &g_pmglobals.domain[domain];
	struct timespec cur_time;
	struct pm_statechange_s *node = NULL;
	struct pm_statechange_s *tail = NULL;

	/* Create an initial state change node with NORMAL state and bootup time */

	node = (struct pm_statechange_s *)pm_alloc(1, sizeof(struct pm_statechange_s));
	if (node == NULL) {
		ret = ENOMEM;
	} else {
		/* Get current time */
		clock_gettime(CLOCK_REALTIME, &cur_time);

		node->state = state;
		node->timestamp = cur_time.tv_sec;
		strncpy(node->wakeup_reason, WAKEUP_REASON_INIT, strlen(WAKEUP_REASON_INIT) + 1);

		/* Get tail of history */
		tail = (struct pm_statechange_s *)dq_tail(&(pdom->history));
		if (tail != NULL) {
			pdom->state_stay[tail->state] += node->timestamp - tail->timestamp;
		}

		sq_addlast((&node->entry), &pdom->history);
		pdom->history_cnt++;

		if (pdom->history_cnt > CONFIG_PM_HISTORY_COUNT) {
			node = (struct pm_statechange_s *)sq_remfirst(&(pdom->history));
			if (node != NULL) {
				pdom->history_cnt--;
				free(node);
			}
		}
	}
	return ret;
}
#endif
#endif							/* CONFIG_PM */
