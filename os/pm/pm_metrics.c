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
#include <time.h>
#include <queue.h>
#include <debug.h>
#include "pm_metrics.h"
#include "pm.h"

time_t time_diff(time_t time1, time_t time2)
{
	if (time1 > time2) {
		return time1 - time2;
	} else {
		return time2 - time1;
	}

}

void pm_prune_history(sq_queue_t *q)
{
	sq_queue_t *queue = q;
	struct pm_statechange_s *sc = NULL;
	struct timespec ts;
	time_t cur_time;
	clock_gettime(CLOCK_REALTIME, &ts);
	cur_time = ts.tv_sec;

	sq_entry_t *head = sq_peek(q);

	for (;;) {
		/* REmove all but one expired node */
		if (head && head->flink) {
			sc = (struct pm_statechange_s *)head->flink;
			if (sc->timestamp < (cur_time - CONFIG_PM_METRICS_DURATION)) {
				sq_rem(head, queue);
				free(head);
				head = sq_peek(queue);
			} else {
				break;
			}
		} else {
			break;
		}
	}
}

void pm_get_domainmetrics(int indx, struct pm_time_in_each_s *mtrics)
{
	pm_lock(indx);
	sq_entry_t *curnode = NULL;
	time_t normal_time = 0;
	time_t idle_time = 0;
	time_t standby_time = 0;
	time_t sleep_time = 0;
	struct timespec ts;
	time_t timetoadd = 0;
	struct pm_statechange_s *sc = NULL;
	struct pm_statechange_s *nsc = NULL;
	enum pm_state_e s = PM_NORMAL;
	int isfirst = 1;
	clock_gettime(CLOCK_REALTIME, &ts);
	time_t cur_time = ts.tv_sec;
	time_t ref_time = cur_time - CONFIG_PM_METRICS_DURATION;

	pmvdbg(" curtime  %d &&&&& reftime %d\n", cur_time, ref_time);
	sq_queue_t *q = &g_pmglobals.domain[indx].history;
	pm_prune_history(q);

	for (curnode = q->head; curnode != NULL; curnode = curnode->flink) {
		sc = (struct pm_statechange_s *)curnode;
		nsc = (struct pm_statechange_s *)(curnode->flink);
		pmvdbg(" sc addr 0x%x &&&&& nsc addr 0x%x\n", sc, nsc);

		/* Calculate metrics */
		if (isfirst && (!nsc)) {
			/* first node and only one node */
			timetoadd = time_diff(cur_time, ref_time);
			isfirst = 0;
		} else if (!nsc) {
			/* Last node */
			timetoadd = time_diff(cur_time, sc->timestamp);
		} else if (ref_time >= sc->timestamp && (nsc->timestamp >= ref_time)) {
			/* sc--reftime--nsc */
			timetoadd = nsc->timestamp - ref_time;
		} else {
			timetoadd = nsc->timestamp - sc->timestamp;
		}

		s = sc->state;

		pmvdbg(" Timetoadd is %d to state:%d\n", timetoadd, s);
		switch (s) {
		case PM_NORMAL:
			normal_time += timetoadd;
			break;
		case PM_IDLE:
			idle_time += timetoadd;
			break;
		case PM_STANDBY:
			standby_time += timetoadd;
			break;
		case PM_SLEEP:
			sleep_time += timetoadd;
			break;
		default:
			printf("Invalid state\n");
		}
	}
	pm_unlock(indx);

	/* Write back metrics */
	mtrics->normal = normal_time;
	mtrics->idle = idle_time;
	mtrics->standby = standby_time;
	mtrics->sleep = sleep_time;
}
