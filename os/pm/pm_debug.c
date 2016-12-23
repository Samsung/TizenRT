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
#include <debug.h>
#include <queue.h>
#include "pm_metrics.h"

#ifdef CONFIG_PM_METRICS
void pm_dumpstates(void)
{
	pm_lock(0);
	sq_entry_t *node = NULL;
	struct pm_statechange_s *sc = NULL;

	sq_queue_t *q = &g_pmglobals.domain[0].history;

	pmvdbg("#########################################################\n");

	for (node = sq_peek(q); node; node = node->flink) {
		sc = (struct pm_statechange_s *)node;
		pmvdbg("state %d and Node address: 0x%x \n", sc->state, sc);
	}

	pmvdbg("#########################################################\n");
	pm_unlock(0);
}
#endif

/* Add more PM debug functions */
